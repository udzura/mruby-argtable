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

  def integer(short, long, datatype, glossary)
    int = Integer.new(short, long, datatype, glossary)
    @table[short] = int
    if long
      @table[long] = int
    end
    self.__push__ int
    self
  end

  def double(short, long, datatype, glossary)
    dbl = Double.new(short, long, datatype, glossary)
    @table[short] = dbl
    if long
      @table[long] = dbl
    end
    self.__push__ dbl
    self
  end

  def string(short, long, datatype, glossary)
    str = String.new(short, long, datatype, glossary)
    @table[short] = str
    if long
      @table[long] = str
    end
    self.__push__ str
    self
  end

  attr_reader :table

  def [](k)
    table[k]
  end
end
