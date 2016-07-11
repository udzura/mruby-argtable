class Argtable
  module OptionsCommon
    def exist?
      self.count > 0
    end
  end
  [Literal, Integer, Double, String].each do |klass|
    klass.include OptionsCommon
  end

  def literal(short, long, glossary)
    lit = Literal.new(short, long, glossary)
    @table[short] = lit if short
    @table[long] = lit  if long
    @options << lit
    self.__push__ lit
    self
  end

  def integer(short, long, datatype, glossary)
    int = Integer.new(short, long, datatype, glossary)
    @table[short] = int if short
    @table[long] = int  if long
    @options << int
    self.__push__ int
    self
  end

  def double(short, long, datatype, glossary)
    dbl = Double.new(short, long, datatype, glossary)
    @table[short] = dbl if short
    @table[long] = dbl  if long
    @options << dbl
    self.__push__ dbl
    self
  end

  def string(short, long, datatype, glossary)
    str = String.new(short, long, datatype, glossary)
    @table[short] = str if short
    @table[long] = str  if long
    if !short and !long
      @barestring ||= str
    end
    @options << str
    self.__push__ str
    self
  end

  def enable_catchall(datatype, glossary, count)
    str = String.new(nil, nil, datatype, glossary, count)
    @catchall ||= str
    self.__push__ str
    self
  end

  attr_reader :table, :barestring, :catchall

  def options(idx=nil)
    return @options unless idx
    @options[idx]
  end

  def [](k)
    table[k]
  end
end
