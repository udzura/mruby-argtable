# mruby-argtable   [![Build Status](https://travis-ci.org/udzura/mruby-argtable.svg?branch=master)](https://travis-ci.org/udzura/mruby-argtable)

Argtable wrapper

## install by mrbgems

- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|
  conf.gem mgem: 'mruby-argtable'
end
```

- or add mgem dependency like:

```ruby
MRuby::Gem::Specification.new('your-cli') do |spec|
  spec.bins = ["your-cli"]

  spec.add_dependency 'mruby-argtable', mgem: 'mruby-argtable'
  #...
end
# This pattern is useful for mruby-cli bundle
```

- **NOTE**: `argtable2` library(as application) or dev headers(when you build) are required.
  - Ubuntu/Debian: `apt-get install libargtable2-dev`
  - RH-ish: `yum install argtable-devel`

### Want to use statically linked version?

- Specify the branch(and it checks-out and uses [argtable3](https://github.com/argtable/argtable3)).

```ruby
conf.gem github: 'udzura/mruby-argtable', branch: 'static-link-argtable3'
```

## example

```ruby
t = Argtable.new
t.literal("v", "verbose", "You are verbose")
t.integer("c", "count", "<c>", "My count")
t.double("s", "seconds", "<t>", "My seconds")
t.string("n", "name", "<n>", "My name")

t.parse(["prog", "-c", "9999", "--verbose", "--name=udzura", "-s", "12.25"])

t["v"].count #=> 1, no value
t["c"].value #=> 9999
t["n"].value #=> "udzura"
t["s"].value #=> 12.25

t.glossary
# -v, --verbose             You are verbose
# -c, --count=<c>           My count
# -s, --seconds=<t>         My seconds
# -n, --name=<n>            My name
```

- [See more examples as test case.](https://github.com/udzura/mruby-argtable/blob/master/test/argtable_test.rb)

## License

under the BSD License(same as argtable itself):
- See LICENSE file
- This gem will be bundled with argtable object. See LICENSE_argtable3 for the object.
