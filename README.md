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
t.literal("a", "aaa", "AAA aaa")
t.literal("b", "bbb", "BBB bbb")

t.parse(["prog", "-a"])
t["a"].count
#=> 1

t.parse ["prog", "XXX"]
#=> prog: unexpected argument "XXX"
```

## License

under the BSD License(same as argtable itself):
- See LICENSE file
- This gem will be bundled with argtable object. See LICENSE_argtable3 for the object.
