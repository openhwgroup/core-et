require 'zlib'

# Save any ruby object to disk!
# Objects are stored as gzipped marshal dumps.
#
# Example
#
# # First ruby process
# hash = {1 => "Entry1", 2 => "Entry2"}
# ObjectStash.sotre hash, 'hash.stash'
#
# ...
#
# # Another ruby process - same_hash will be identical to the original hash
# same_hash = ObjectStash.load 'hash.stash'
class ObjectStash

  # Store an object as a gzipped file to disk
  # 
  # Example
  #
  # hash = {1 => "Entry1", 2 => "Entry2"}
  # ObjectStore.store hash, 'hash.stash.gz'
  # ObjectStore.store hash, 'hash.stash', :gzip => false
  def self.store obj, file_name, options={}
    marshal_dump = Marshal.dump(obj)
    file = File.new(file_name,'w')
    file = Zlib::GzipWriter.new(file) unless options[:gzip] == false
    file.write marshal_dump
    file.close
    return obj
  end
  
  # Read a marshal dump from file and load it as an object
  #
  # Example
  #
  # hash = ObjectStore.get 'hash.dump.gz'
  # hash_no_gzip = ObjectStore.get 'hash.dump.gz'
  def self.load file_name
    begin
      file = Zlib::GzipReader.open(file_name)
    rescue Zlib::GzipFile::Error
      file = File.open(file_name, 'r')
    ensure
      obj = Marshal.load file.read
      file.close
      return obj
    end
  end
end

if $0 == __FILE__
  require 'test/unit'
  class TestObjectStash < Test::Unit::TestCase
    @@tmp = '/tmp/TestObjectStash.stash'
    def test_hash_store_load
      hash1 = {:test=>'test'}
      ObjectStash.store hash1, @@tmp
      hash2 = ObjectStash.load @@tmp
      assert hash1 == hash2
    end
    def test_hash_store_load_no_gzip
      hash1 = {:test=>'test'}
      ObjectStash.store hash1, @@tmp, :gzip => false
      hash2 = ObjectStash.load @@tmp
      assert hash1 == hash2
    end
    def test_self_stash
      ObjectStash.store ObjectStash, @@tmp
      assert ObjectStash == ObjectStash.load(@@tmp)
    end
    def test_self_stash_no_gzip
      ObjectStash.store ObjectStash, @@tmp, :gzip => false
      assert ObjectStash == ObjectStash.load(@@tmp)
    end
  end
end


# Example
#
## Here's our theory of everything - make it heavy
#theory_of_everything = { :question => "Life, Universe & Everything", :answer => 42 }
#1000.times {|i| theory_of_everything[i] = "rubbish#{i}" }
#
## Stash it
#ObjectStash.store theory_of_everything, './theory_of_everything.stash'
#
## Let's see what it did
#File.stat('./theory_of_everything.stash').size # => 4206
#
## Reload it
#loaded_theory_of_everything = ObjectStash.load 'theory_of_everything.stash'
#question = loaded_theory_of_everything[:question]
#answer = loaded_theory_of_everything[:answer]
#puts "The answer to #{question} is #{answer}" # => The answer to Life, Universe & Everything is 42
#


#Test
#def report msg
# puts msg
# t=Time.now
# yield
# puts "  -> #{Time.now-t}s"
#end
#
#def test_hash_equality hash1, hash2
# throw :HashesNotEqual unless hash1 == hash2
# throw :HashesNotTreeEqual unless hash1 === hash2
# hash1.each do |key,value|
#   throw :HashCopyDoesNotHaveKey unless hash2.has_key? key
#   throw :HashCopyDoesNotHaveValue unless hash2.has_value? value
# end
#end
#
#original_hash = Hash.new
#loaded_hash = nil
#marshal_dump = nil
#
#report "Build a big hash to be marshalled" do
# (0..1000).each do |i|
#   original_hash[i] = "Entry #{i}"
# end
#end
#
#report "Store the hash" do
# marshal_dump = Marshal.dump(original_hash)
#end
#
#report "Load a hash from the dump" do
# loaded_hash = Marshal.load(marshal_dump)
#end
#
#report "Assert hashes are equal" do
# test_hash_equality original_hash, loaded_hash
#end
#
#report "Write the marshal dump to file" do
# file_out = File.new("marshal_test.tmp",'w')
# file_out.write(marshal_dump)
# file_out.close
#end
#
#report "Construct hash from the marshal file dump" do
# file_in = File.new('marshal_test.tmp','r')
# loaded_hash = Marshal.load(file_in.read)
# file_in.close
#end
#
#report "Assert hashes are equal" do
# test_hash_equality original_hash, loaded_hash
#end
#
#require 'zlib'
#report "Gzip the marshaled dump" do
# file = File.new('marshal_test.tmp.gz','w')
# gz = Zlib::GzipWriter.new(file)
# gz.write marshal_dump
# gz.close
#end
#
#report "Gunzip the marshaled dump and load it" do
# gz = Zlib::GzipReader.open('marshal_test.tmp.gz')
# loaded_hash = Marshal.load gz.read
# gz.close
#end
#
#report "Assert hashes are equal" do
# test_hash_equality original_hash, loaded_hash
#end
#
#puts "Success!"