require 'fileutils'

MRuby::Gem::Specification.new('mruby-argtable') do |spec|
  spec.license = 'BSD'
  spec.authors = ["Uchio Kondo", "Stewart Heitmann"]

  def spec.bundle_argtable
    argtable_dir = "#{build.build_dir}/vendor/argtable3"
    argtable_lib = libfile "#{argtable_dir}/.libs"
    argtable_objs_dir = "#{argtable_dir}/.objs"
    header = "#{argtable_dir}/argtable3.h"

    task :clean do
      FileUtils.rm_rf [argtable_dir]
    end

    file header do
      sh "mkdir -p #{File.dirname(argtable_dir)}"
      sh "git clone git@github.com:argtable/argtable3.git #{argtable_dir}"
    end

    file "#{argtable_objs_dir}/argtable3.o" => header do
      sh "mkdir -p #{argtable_objs_dir}"
      Dir.chdir argtable_dir do
        puts "Command: #{build.cc.command} -c #{build.cc.flags.join(' ')} *.c -o #{argtable_objs_dir}/argtable3.o"
        sh "#{build.cc.command} -c #{build.cc.flags.join(' ')} *.c -o #{argtable_objs_dir}/argtable3.o"
      end
    end

    libmruby_a = libfile("#{build.build_dir}/lib/libmruby")
    file libmruby_a => "#{argtable_objs_dir}/argtable3.o"

    self.objs << "#{argtable_objs_dir}/argtable3.o"
    self.cc.include_paths << argtable_dir
  end

  spec.bundle_argtable
end
