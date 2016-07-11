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

assert("Argtable::OptionsCommon#exist?") do
  t = Argtable.new
  t.integer("c", "count", "<c>", "My count")
  t.string("n", "name", "<n>", "My name")

  t.parse(["prog", "-c", "9999"])
  assert_equal t["c"].count, 1
  assert_equal t["c"].value, 9999
  assert_equal t["n"].count, 0
  assert_false t["n"].exist?
end
