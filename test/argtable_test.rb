assert("Argtable#parse/#literal") do
  t = Argtable.new
  t.literal("a", "aaa", "AAA aaa")
  t.literal("b", "bbb", "BBB bbb")

  t.parse(["prog", "-a"])
  assert_equal t["a"].count, 1
  assert_equal t["b"].count, 0

  t.parse(["prog", "-b"])
  assert_equal t["a"].count, 0
  assert_equal t["b"].count, 1

  assert_equal t["aaa"].count, 0
  assert_equal t["bbb"].count, 1
end

assert("Argtable#integer") do
  t = Argtable.new
  t.integer("c", "count", "<c>", "My count")

  t.parse(["prog", "-c", "9999"])
  assert_equal t["c"].value, 9999
end
