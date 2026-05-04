#!/eng/dvtools/release/ruby/2.4.2/bin/ruby

# Hack the GEM_PATH To Karls Libary 
ENV['GEM_PATH']="/eng/dvtools/release/ruby/mygems"
Gem.clear_paths


require "awesome_print"
require 'optparse'

FILE_BASE_NAME="efuse_bank"

mod_start=0
for i in 0..5
  filename = "#{FILE_BASE_NAME}_#{i}.txt"
  f = File.new(filename, File::CREAT|File::TRUNC|File::RDWR, 0644)
  for addr in 0..8191
     if (addr % 8) == mod_start
        f.write "@#{addr.to_s(16)} 1\n"
     else
        f.write "@#{addr.to_s(16)} 0\n"
     end
  end
  f.close
  mod_start += 1
end

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


