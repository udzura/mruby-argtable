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
