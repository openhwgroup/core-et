#!/eng/dvtools/release/ruby/2.4.2/bin/ruby

# Hack the GEM_PATH To Karls Libary 
ENV['GEM_PATH']="/eng/dvtools/release/ruby/mygems"
Gem.clear_paths


require "awesome_print"
require 'optparse'

FILE_BASE_NAME="efuse_bank"
STARTING_ADDR=0xf5b
STARTING_BANK=0x3
NUM_FUSES=8192

mod_start=0
num_loaded=0
finp = File.new("efuse_input.txt")
for i in 0..5
  filename = "#{FILE_BASE_NAME}_#{i}.txt"
  f = File.new(filename, File::CREAT|File::TRUNC|File::RDWR, 0644)
  for addr in 0..4095
    if (addr<16)
      if((addr<STARTING_ADDR && i==STARTING_BANK) || i<STARTING_BANK || num_loaded>=NUM_FUSES)
        f.write "@0#{addr.to_s(16)[-1]}00 0\n"
        f.write "@1#{addr.to_s(16)[-1]}00 0\n"
      else
        line = finp.gets
	args = line.split(' ')
        f.write "@0#{addr.to_s(16)[-1]}00 #{args[1]}\n"
        f.write "@1#{addr.to_s(16)[-1]}00 #{args[1]}\n"
	num_loaded += 1
      end
    elsif (addr<256)
      if((addr<STARTING_ADDR && i==STARTING_BANK) || i<STARTING_BANK || num_loaded>=NUM_FUSES)
        f.write "@0#{addr.to_s(16)[-1]}0#{addr.to_s(16)[-2]} 0\n"
        f.write "@1#{addr.to_s(16)[-1]}0#{addr.to_s(16)[-2]} 0\n"
      else
        line = finp.gets
	args = line.split(' ')
        f.write "@0#{addr.to_s(16)[-1]}0#{addr.to_s(16)[-2]} #{args[1]}\n"
        f.write "@1#{addr.to_s(16)[-1]}0#{addr.to_s(16)[-2]} #{args[1]}\n"
	num_loaded += 1
      end
    else
      if((addr<STARTING_ADDR && i==STARTING_BANK) || i<STARTING_BANK || num_loaded>=NUM_FUSES)
        f.write "@0#{addr.to_s(16)[-1]}#{addr.to_s(16)[-3..-2]} 0\n"
        f.write "@1#{addr.to_s(16)[-1]}#{addr.to_s(16)[-3..-2]} 0\n"
      else
        line = finp.gets
	args = line.split(' ')
        f.write "@0#{addr.to_s(16)[-1]}#{addr.to_s(16)[-3..-2]} #{args[1]}\n"
        f.write "@1#{addr.to_s(16)[-1]}#{addr.to_s(16)[-3..-2]} #{args[1]}\n"
	num_loaded += 1
      end
    end
  end
  f.close
  mod_start += 1
end
finp.close

#options = {}
#OptionParser.new do |opts|
#   opts.banner = "Usage: json_2_csr [options]"
#   opts.on("-f filename", "--file filename", "name of file to convert") do |arg|
#     options[:filename] = arg
#   end
#   opts.on("-o filename", "--output filename", "name of file to convert") do |arg|
#     options[:out_filename] = arg
#   end
#end.parse!

#nless options[:filename]
# puts "ERROR: -f filename is a required flag"
#  exit
#end
#
#unless options[:out_filename]
#  options[:out_filename]="#{File.basename(options[:filename],".*")}.csr"
#end


