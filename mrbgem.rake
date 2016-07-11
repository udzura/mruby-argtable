require 'fileutils'

MRuby::Gem::Specification.new('mruby-argtable') do |spec|
  spec.license = 'BSD'
  spec.authors = ["Uchio Kondo", "Stewart Heitmann"]

  spec.linker.libraries << 'argtable'
end
