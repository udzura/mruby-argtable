# mruby-argtable   [![Build Status](https://travis-ci.org/udzura/mruby-argtable.svg?branch=master)](https://travis-ci.org/udzura/mruby-argtable)

Argtable wrapper

## install by mrbgems

- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|
  conf.gem :github => 'udzura/mruby-argtable'
end
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

## License

under the BSD License(same as argtable itself):
- See LICENSE file
- This gem will be bundled with argtable object. See LICENSE_argtable3 for the object.
