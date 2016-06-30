MRuby::Build.new do |conf|
  toolchain :clang
  conf.gembox 'default'
  conf.gem '../mruby-argtable'
  conf.enable_test
end
