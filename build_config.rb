MRuby::Build.new do |conf|
  if ENV['OSTYPE'] =~ /darwin/
    toolchain :clang
  else
    toolchain :gcc
  end

  conf.gembox 'default'
  conf.gem '../mruby-argtable'
  conf.enable_test
end
