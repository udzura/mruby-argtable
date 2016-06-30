# mruby-argtable   [![Build Status](https://travis-ci.org/udzura/mruby-argtable.svg?branch=master)](https://travis-ci.org/udzura/mruby-argtable)
Argtable class
## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'udzura/mruby-argtable'
end
```
## example
```ruby
p Argtable.hi
#=> "hi!!"
t = Argtable.new "hello"
p t.hello
#=> "hello"
p t.bye
#=> "hello bye"
```

## License
under the BSD License:
- see LICENSE file
