require 'fileutils'

MRuby::Gem::Specification.new('mruby-argtable') do |spec|
  spec.license = 'BSD'
  spec.authors = ["Uchio Kondo", "Stewart Heitmann"]

  def spec.bundle_argtable
    def argtable_dir(b); "#{b.build_dir}/vendor/argtable3"; end
    def argtable_lib(b); libfile "#{argtable_dir(b)}/.libs"; end
    def argtable_objs_dir(b); "#{argtable_dir(b)}/.objs"; end
    def argtable_header(b); "#{argtable_dir(b)}/argtable3.h"; end

    task :clean do
      FileUtils.rm_rf [argtable_dir(build)]
    end

    file argtable_header(build) do
      sh "mkdir -p #{File.dirname(argtable_dir(build))}"
      sh "git clone https://github.com/argtable/argtable3.git #{argtable_dir(build)}"
    end

    file "#{argtable_objs_dir(build)}/argtable3.o" => argtable_header(build) do
      sh "mkdir -p #{argtable_objs_dir(build)}"
      Dir.chdir argtable_dir(build) do
        puts "Command: #{build.cc.command} -c #{build.cc.flags.join(' ')} *.c -o #{argtable_objs_dir(build)}/argtable3.o"
        sh "#{build.cc.command} -c #{build.cc.flags.join(' ')} *.c -o #{argtable_objs_dir(build)}/argtable3.o"
      end
    end

    libmruby_a = libfile("#{build.build_dir}/lib/libmruby")
    file libmruby_a => "#{argtable_objs_dir(build)}/argtable3.o"

    self.objs << "#{argtable_objs_dir(build)}/argtable3.o"
    self.cc.include_paths << argtable_dir(build)
  end

  spec.bundle_argtable
end
