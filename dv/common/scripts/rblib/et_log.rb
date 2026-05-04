# Extend the string class with Colors
class String
  # Create class methods
  class << self; attr_accessor :do_colorize end
  @do_colorize = true

  #  Simple method to print text in a certain color
  #
  #
  def colorize(color, options = {})
    return self unless String.do_colorize
    background = options[:background] || options[:bg] || false
    style = options[:style]
    offsets = ["gray","red", "green", "yellow", "blue", "magenta", "cyan","white"]
    styles = ["normal","bold","dark","italic","underline","xx","xx","underline","xx","strikethrough"]
    start = background ? 40 : 30
    color_code = start + (offsets.index(color) || 8)
    style_code = styles.index(style) || 0
    "\e[#{style_code};#{color_code}m#{self}\e[0m"
  end
end

module ET_LOG

  require 'fileutils'   # basic file manipulation
  require 'logger'      # the ET Logger is based on the built in ruby logger
  require "open3"       # std ruby lib for running system commands and capturing output
  require "pp"
  #require 'ap'

  def error msg,continue_on_error=nil,stack_trace=nil
    str = ''
    if msg.class == Array
      msg.each do |m|
        str += "\t#{m}\n"
      end
    else
      str = msg
    end
    str = "etlog:E: " + str;
    puts str.colorize("red")
    if (stack_trace == true)
      puts "etlog:I: Stack Trace follows:".colorize("red")
      caller.each do | call |
        puts call.colorize("red")
      end
    end
    if (continue_on_error == true)
      return 1
    else
      exit 1
    end
  end


  # Global Method to run a system command and echo it to the STDOUT
  # This method should primarily be used if you need to run a command BEFORE
  # the etLog class is initialized.   once the EtLog is created use the methods of the
  # same name inside the log object and then the output will be logged.
  #
  # @param sys_cmd [String] The string message to be logged
  # @param output  [Symbol] [:info,:debug,:quiet] info and debug are log levels at which to print the message.  :quiet means don't print
  #   the command to the log
  # @param continue_on_error  [Boolean]  continue execution on error if true.  default is to halt on error
  # @param stack_trace        [Boolean]  print stack trace upon error?
  #
  def et_sys (sys_cmd, output=:info,continue_on_error=nil,stack_trace=nil)
    if (output == :info) || (output == :debug)
      puts("running cmd: " + sys_cmd)
    end
    system("#{sys_cmd}")
    rc = $?.exitstatus
    if (rc != 0)
      puts "et_sys() rc=#{rc} cmd=#{sys_cmd}".colorize("red")
      if (stacktrace == true)
        puts "Stack Trace follows:".colorize("red")
        caller.each do | call |
          puts call.colorize("red")
        end
      end
      if (continue_on_error == true)
        return rc
      else
        exit rc
      end
    end
    return rc
  end

  # Global Method to run a system command and return output to the user
  # This method should primarily be used if you need to run a command BEFORE
  # the EtLog class is initialized.   once the EtLog is created use the methods of the
  # same name inside the log object and then the output will be logged.
  #
  # @param  sys_cmd   [String] The string message to be logged
  # @return rc,result [String,Integer] The result of the command and its return code
  #
  def et_backtick (sys_cmd,continue_on_error=nil,stack_trace=nil,quiet=nil)
    #puts "et_backtick: running cmd=#{sys_cmd}"
    result = `#{sys_cmd}`
    rc = $?.exitstatus
    unless ((rc == 0) || (quiet))
      puts "et_backtick(): rc=#{rc} cmd=#{sys_cmd}\n  result=#{result}"
      if (stack_trace == true)
        puts "Stack Trace follows:".colorize("red")
        caller.each do | call |
          puts call.colorize("red")
        end
      end
      if (continue_on_error == true)
        return result,rc
      else
        exit rc
      end
    end
    return result,rc
  end

  # This Method is used to fork output to multi locations
  # For example in the et Log class we often want to output
  # to STDOUT and a file at the same time.
  #
  class MultiDelegator
    # Default Constructor
    #
    # @param targets [List]  List of Targets
    def initialize(*targets)
      @targets = targets
    end

    #
    def self.delegate(*methods)
      methods.each do |m|
        define_method(m) do |*args|
          @targets.map { |t| t.send(m, *args) }
        end
      end
      self
    end

    class << self
      alias to new
    end
  end # end class MultiDelegator

  # This class is a basic message logger and error handler for the et Ruby Applications
  # This is build on top of the logger module
  #
  class EtLog

    # log_file_path = full path of log file along with file name
    # Absolute path must be given to log_file_path. Assigning
    # relative path to log_file_path is not allowed
    attr_accessor :log_file_path,       # Path to logfile
    :log_host,            # hostname on which the job was run
    :enable_stacktrace,   # Global option to enable or suppress stack traces in this class when errors occur
    :log_prefix,           # Prefix to log message
    :continue_on_error,    # Boolean to determine whether program execution continues upon error. Defaults to false
    :errors,
      :warnings,
      :fatals,
      :start_time

    # Method to set the verbosity level
    #
    # @param log_level [Int] verbosity level of the logger
    def level=(log_level)
      case log_level.to_sym
      when :fatal
        @mylog.level = Logger::FATAL
      when :error
        @mylog.level = Logger::ERROR
      when :warn
        @mylog.level = Logger::WARN
      when :info
        @mylog.level = Logger::INFO
      when :debug
        @mylog.level = Logger::DEBUG
        @enable_stacktrace=true
      else
        @mylog.level = Logger::INFO
      end
    end

    # Method to return the verbosity level
    #
    # @return [Symbol] the verbosity level
    def level
      @mylog.level
    end

    # Use subclass so we can undo colorize
    class MyFile < File
      def initialize(fn, perm)
        super(fn, perm)
      end
      def write(s)
        # \e[#{style_code};#{color_code}m#{self}\e[0m
        t = s.gsub(/\e\[\d+\;\d+m/,'').gsub(/\e\[0m/,'')
        super(t)
      end
    end
    # Method to initialize and setup the logger initially
    #
    # @param fname [String] The full path of log file along with file name
    #                       Absolute path must be given to fname. Assigning
    #                       relative path to fname is not allowed
    # @param log_level [String] The initial verbosity level of the message logger
    #
    def initialize fname, log_level,use_host=false,log_prefix="etlog",stack_trace=nil

      @stack_trace=stack_trace
      @log_prefix = log_prefix
      @log_file_path = fname
      @log_host=`/bin/hostname`.chomp
      @continue_on_error = false
      @errors=0
      @warnings=0
      @fatals=0

      log_file = MyFile.open(@log_file_path, "w")
          log_file.sync = true

      @mylog = Logger.new MultiDelegator.delegate(:write, :close).to(STDOUT, log_file)
      level=log_level

      if use_host
        @mylog.formatter = proc { |severity, datetime, progname, msg|
          "#{@log_prefix}:#{severity[0]}: #{datetime.strftime('%H:%M:%S')}: Host: #{log_host} #{msg}\n"
        }
      else
        @mylog.formatter = proc { |severity, datetime, progname, msg|
          "#{@log_prefix}:#{severity[0]}: #{datetime.strftime('%H:%M:%S')}: #{msg}\n"
        }
      end

      @start_time = Time.now
      @mylog.level = Logger::INFO
      debug "etlog Log file opened @ #{@log_file_path} on host #{@log_host}"
    end


    def close delete=false
      info ( "etlog Log File: #{@log_file_path}".colorize("green") )
      delta = Time.now - @start_time;
      hh_mm_ss = Time.at(delta.to_i).utc.strftime("%H:%M:%S")
      info "Total Elapsed Time: #{hh_mm_ss} (HH:MM:SS)".colorize("green")
      @mylog.close
      if (@log_file_path != "/dev/null") && (File.exists? @log_file_path)
        File.chmod(0664,@log_file_path)
        if delete
           File.delete @log_file_path
        end
      end
    end

    # Method to print a fatal message, create permanent record of the fatal in
    # the etlog.log and then exit.
    #
    # @param msg [String] The string message to be logged
    # @param continue_on_error  [Boolean] The return code to set when exiting
    # @param stack_trace [Boolean] if true then a stack trace is added. if false its not
    #    The default is to print the stack trace
    # @return [Integer] The return code. (aka 1)
    def fatal msg,continue_on_error=@continue_on_error,stack_trace=@stack_trace
      str = ''
      if msg.class == Array
        msg.each do |m|
          str += "\t#{m}\n"
        end
      else
        str = msg
      end
      @fatals+=1
      @mylog.fatal str.colorize("red")
      if (stack_trace)
        info "Stack Trace follows:".colorize("red")
        caller.each do | call |
          info call.colorize("red")
        end
      end
      if (continue_on_error == true)
        return 1
      else
        exit 1
      end
    end

    # Method to print an error message, create permanent record of the error in
    # the etlog.log and then exit.
    #
    # @param msg [String] The string message to be logged
    # @param continue_on_error  [Boolean]  Allow program to continue execution upon error
    # @param stack_trace [Boolean] if true then a stack trace is added. if false its not
    #    The default is to print the stack trace
    #
    def error msg,continue_on_error=@continue_on_error,stack_trace=@stack_trace
      str = ''
      if msg.class == Array
        msg.each do |m|
          str += "\t#{m}\n"
        end
      else
        str = msg
      end
      @errors+=1
      @mylog.error str.colorize("red")
      if (stack_trace)
        info "Stack Trace follows:".colorize("red")
        caller.each do | call |
          info call.colorize("red")
        end
      end
      if (continue_on_error == true)
        return 1
      else
        exit 1
      end
    end

    def apd obj
      debug "#{obj}"
    end

    def ap obj
      info "#{obj}"
    end

    # Method to print a warning message and then continue execution
    #
    # @param msg [String] The string message to be logged
    #
    def warn msg
      @warnings+=1
      @mylog.warn msg.colorize "cyan"
    end

    # Method to print an info message and then continue execution
    #
    # @param msg [String] The string message to be logged
    #
    def info msg
      @mylog.info msg
    end

    # Method to print an info message and then continue execution
    #
    # @param msg [String] The string message to be logged
    #
    def debug msg
      @mylog.debug msg
    end

    # Method to run a system command and echo it to the log
    #   more stuff
    #   here to test yard server
    #
    # @param sys_cmd [String] The string message to be logged
    # @param continue_on_error  [Boolean]  Allow program to continue execution upon error
    # @param output  [Symbol] [:info,:debug,:quiet] info and debug are log levels at which to print the message.  :quiet means don't print the command to the log
    # @return rc,result [String,Integer] The result of the command and its return code
    def et_sys (sys_cmd,continue_on_error=@continue_on_error,output=:info)
      rc=0
      if (output == :info)
        info("EtLog::et_sys() cmd: " + sys_cmd)
      elsif (output == :debug)
        debug("EtLog::et_sys() cmd: " + sys_cmd)
      end
      Open3.popen2e(sys_cmd) {|i,oe,t|
        oe.each { |line|
          if (output == :info)
            @mylog.info  line.chomp
          else
            @mylog.debug line.chomp
          end
        }
        rc = t.value.exitstatus # Process::Status object returned.
      }
      error "EtLog::et_sys() rc=#{rc} cmd=#{sys_cmd}", continue_on_error unless (rc==0)
      return rc
    end

    # Global Method to run a system command and return output to the user
    #
    # @param sys_cmd [String] The string message to be logged
    # @param continue_on_error  [Boolean]  Allow program to continue execution upon error
    # @return rc,result [String,Integer] The result of the command and its return code
    def et_backtick (sys_cmd,continue_on_error=@continue_on_error,quiet=nil)
      output=""
      rc=0
      Open3.popen2e(sys_cmd) {|i,oe,t|
        oe.each { |line|
          output += line
        }
        rc = t.value.exitstatus # Process::Status object returned.
      }
      error "EtLog::et_backtick() rc=#{rc} cmd=#{sys_cmd}", continue_on_error unless rc==0 || continue_on_error==true || quiet
      return output,rc
    end

    def print_summary
      @log.info "Total Warnings: #{@warnings}"
      @log.info "Total Errors  : #{@errors}"
      @log.info "Total Fatals  : #{@fatals}"
      if (!@errors && !@fatals)
        smiley()
      end
    end

    # Method to print a smiley for test status
    #
    def smiley
      str = <<-EOS

      o   o
        |
      \\___/

      EOS
      info str.colorize("green")
    end

    # Method to print a RED X for test status
    #
    def cross
      str = <<-EOS

      xx   xx
       xx xx
        xxx
       xx xx
      xx   xx

      EOS
      info str.colorize("red")
    end

  end # end class Et Log Cov

end # end module
