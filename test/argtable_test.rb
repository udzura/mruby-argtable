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
  t.double("s", "seconds", "<s>", "My seconds")

  t.parse(["prog", "-s", "10.5"])
  assert_equal t["s"].value, 10.5
end

assert("Argtable#string") do
  t = Argtable.new
  t.string("n", "name", "<n>", "My name")

  t.parse(["prog", "-n", "udzura"])
  assert_equal t["n"].value, "udzura"
end

assert("Argtable#string without optname") do
  t = Argtable.new
  t.string(nil, nil, "<yourname>", "My name")
  t.string("l", "last", "<lastname>", "My last name")

  t.parse(["prog", "uchio", "-l", "kondo"])
  assert_equal t.barestring.value, "uchio"
  assert_equal t['l'].value, "kondo"

  errors = t.parse(["prog", "-l", "matsumoto", "ryosuke"])
  assert_equal errors, 0
  assert_equal t.barestring.value, "ryosuke"
  assert_equal t['l'].value, "matsumoto"
end

assert("Argtable#catchall") do
  t = Argtable.new
  t.string("n", "name", "<n>", "My name")
  t.enable_catchall("<rest>, [<rest>...]", "Tha name of rest", 100)

  t.parse(["prog", "-n", "udzura", "uchio", "akubi"])
  assert_equal t['n'].value, "udzura"
  assert_equal t.catchall.value(0), "uchio"
  assert_equal t.catchall.value(1), "akubi"

  assert_equal t.catchall.values, ["uchio", "akubi"]
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
