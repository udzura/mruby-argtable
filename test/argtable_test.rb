assert("Argtable#literal") do
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

assert("Argtable#integer will raise") do
  t = Argtable.new
  t.integer("c", "count", "<c>", "My count")

  t.parse(["prog", "-c", "9999"])
  assert_raise(ArgumentError) do
    t["c"].value(2)
  end
end

assert("Argtable#double") do
  t = Argtable.new
  t.double("s", "seconds", "<t>", "My seconds")

  t.parse(["prog", "-s", "10.5"])
  assert_equal t["s"].value, 10.5
end

assert("Argtable#string") do
  t = Argtable.new
  t.string("n", "name", "<n>", "My name")

  t.parse(["prog", "-n", "udzura"])
  assert_equal t["n"].value, "udzura"
end

assert("Argtable#parse when mixed") do
  t = Argtable.new
  t.literal("v", "verbose", "You are verbose")
  t.integer("c", "count", "<c>", "My count")
  t.double("s", "seconds", "<t>", "My seconds")
  t.string("n", "name", "<n>", "My name")

  t.parse(["prog", "-c", "9999", "--verbose", "--name=udzura", "-s", "12.25"])
  assert_equal t["v"].count, 1
  assert_equal t["c"].value, 9999
  assert_equal t["n"].value, "udzura"
  assert_equal t["s"].value, 12.25
end
