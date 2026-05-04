#!/eng/dvtools/release/ruby/2.4.2/bin/ruby

# Hack the GEM_PATH To Karls Libary 
ENV['GEM_PATH']="/eng/dvtools/release/ruby/mygems"
Gem.clear_paths


require "awesome_print"
require 'optparse'

FILE_BASE_NAME="efuse_input"

load_num=0
filename = "#{FILE_BASE_NAME}.txt"
f = File.new(filename, File::CREAT|File::TRUNC|File::RDWR, 0644)
for addr in 0..8191
  shft = 3 - (addr % 4)
  f.write "#{addr} #{(load_num>>shft) & 0x1}\n"
  if(addr%4==3)
    load_num += 1
  end
end
f.close
