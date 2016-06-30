class Argtable
  def literal(short, long, glossary)
    lit = Literal.new(short, long, glossary)
    @table[short] = lit
    if long
      @table[long] = lit
    end
    self.__push__ lit
    self
  end

  attr_reader :table

  def [](k)
    table[k]
  end
end
