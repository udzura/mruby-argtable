# mruby-argtable   [![Build Status](https://travis-ci.org/udzura/mruby-argtable.svg?branch=master)](https://travis-ci.org/udzura/mruby-argtable)

Argtable class

## install by mrbgems

- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|
  conf.gem :github => 'udzura/mruby-argtable'
end
```

## example

```ruby
a = Argtable::Literal.new("a", "aaa", "aaa aaa")
b = Argtable::Literal.new("b", "bbb", "bbb bbb")
t = Argtable.new
t.push a; t.push b
t.compile

t.parse(["prog", "-a"])
a.count
#=> 1

t.parse ["prog", "-b", "XXX"]
#=> prog: unexpected argument "XXX"
```

## License

under the BSD License(same as argtable itself):
- See LICENSE file
- This gem will be bundled with argtable object. See LICENSE_argtable3 for the object.
