##
## Argtable Test
##

assert("Argtable#hello") do
  t = Argtable.new "hello"
  assert_equal("hello", t.hello)
end

assert("Argtable#bye") do
  t = Argtable.new "hello"
  assert_equal("hello bye", t.bye)
end

assert("Argtable.hi") do
  assert_equal("hi!!", Argtable.hi)
end
