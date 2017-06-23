MRuby::Build.new do |conf|
  if ENV['OSTYPE'] =~ /darwin/
    toolchain :clang
  else
    toolchain :gcc
  end

  conf.gembox 'default'
  conf.gem '../mruby-argtable'
  conf.cc.flags << "-DEOVERFLOW_=EOVERFLOW"
  conf.enable_test
  conf.enable_debug
end
