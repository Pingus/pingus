#  SExpr - A simple Ruby library for parsing and validating s-expressions
#  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

module SExpr
  class Parser

    def is_digit(c)
      return (?0..?9).member?(c)
    end

    def is_letter(c)
      return((?a..?z).member?(c) or (?A..?Z).member?(c))
    end

    def is_whitespace(c)
      return(c == ?\s or c == ?\n or c == ?\t)
    end

    def is_special_initial(c)
      return(c == ?! or
             c == ?$ or 
             c == ?% or 
             c == ?& or 
             c == ?* or 
             c == ?/ or 
             c == ?: or 
             c == ?< or 
             c == ?= or 
             c == ?> or 
             c == ?? or 
             c == ?^ or 
             c == ?_ or 
             c == ?~)
    end

    def is_special_subsequent(c)
      return(c == ?+ or
             c == ?- or
             c == ?. or
             c == ?@)
    end

    def is_sign(c)
      return(c == ?+ or
             c == ?-)
    end

    def initialize(str)
      @str = str
    end

    def parse()
      @state = :look_for_token
      tokenize()

      elements = []
      sublists = []

      @tokens.each{ |token|
        if token.is_a?(SExpr) then
          if not sublists.empty? then
            sublists.last().append(token)
          else
            elements << token
          end
        elsif token.is_a?(Array) then
          if token[0] == :list_start then
            sublists.push(List.new([], token[1]))
          elsif token[0] == :list_end then
            if sublists.empty? then
              raise "Unexpected List end"
            else
              lst = sublists.pop()
              if not sublists.empty? then
                sublists.last().append(lst)
              else
                elements << lst
              end
            end
          end
        else
          raise "Parser bug: parse: #{token}"
        end
      }

      return elements
    end

    def tokenize()
      @line   = 1
      @column = 1

      @last_c = nil
      @tokens = []

      @pos = 0
      @token_start = @pos
      
      @advance = true
      while(@pos < @str.length) do
        @c = @str[@pos]

        scan(@c)
        
        if @advance then
          # Line/Column counting
          if (@c == ?\n) then
            @line   += 1
            @column  = 1
          else
            @column += 1
          end

          @last_c = @c

          @pos += 1
        else
          @advance = true
        end
      end

      scan(nil) # EOF #
    end

    def scan(c)
      case @state
      when :look_for_token:
          if is_digit(c) or is_sign(c) then
            @state = :parse_integer_or_real
          elsif c == ?\" then
            @state = :parse_string
          elsif c == ?# then
            @state = :parse_boolean
          elsif is_letter(c) or is_special_initial(c) then
            @state = :parse_symbol
          elsif is_whitespace(c) then
            @state = :parse_whitespace
          elsif c == ?; then
            @state = :parse_comment
          elsif c == ?) then
            submit(:list_end, true)
          elsif c == ?( then
            submit(:list_start, true)
          elsif c == nil then
            # EOF is ok
          else
            raise "#{@line}:#{@column}: unexpected character '#{c.chr}'"
          end

      when :parse_integer_or_real:
          if is_digit(c) then
            # ok
          elsif c == ?. then
            @state = :parse_real
          else
            if @token_start == @pos - 1 and not is_digit(@str[@token_start]) then
              raise "#{@line}:#{@column}: '#{@str[@token_start].chr}' must be followed by digit"
            else
              submit(:integer, false)
            end
          end

      when :parse_boolean:
          if c == ?t or c == ?f then
            submit(:boolean, true)
          else
            raise "#{@line}:#{@column}: expected 'f' or 't', got '#{c.chr}"
          end

      when :parse_real:
          if (?0..?9).member?(c) then
            # ok
          else
            submit(:real, false)
          end

      when :parse_symbol:
          if is_letter(c) or is_digit(c) or is_special_subsequent(c) or is_special_initial(c) then
            # ok
          else
            submit(:symbol, false)
          end
        
      when :parse_string:
          if (c == ?" and @last_c != ?\\) then
            submit(:string, true)
          end

      when :parse_whitespace
        if not is_whitespace(c) then
          submit(:whitespace, false)
        end

      when :parse_comment
        if c == ?\n then
          submit(:comment, true)
        end     

      else
        raise "Parser error, unknown state: #{@state}"
      end
    end

    def get_pos()
      return "@line:@column"
    end

    def submit(type, include_current_character)
      @state = :look_for_token

      if include_current_character then
        current_token = @str[@token_start..(@pos)]
        @token_start = @pos+1
      else
        current_token = @str[@token_start..(@pos-1)]
        @token_start = @pos
        @advance = false
      end
      
      case type
      when :boolean
        @tokens << Boolean.new(current_token == "#t", get_pos())
        
      when :integer
        @tokens << Integer.new(current_token.to_i, get_pos())

      when :real
        @tokens << Real.new(current_token.to_f, get_pos())

      when :string
        @tokens << String.new(current_token[1..-2].
                              gsub("\\n", "\n").
                              gsub("\\\"", "\"").
                              gsub("\\t", "\t"),
                              get_pos())

      when :symbol
        @tokens << Symbol.new(current_token, get_pos())
        
      when :list_start
        @tokens << [:list_start, get_pos()]

      when :list_end
        @tokens << [:list_end, get_pos()]

      when :comment
        # ignore

      when :whitespace
        # ignore

      else
        raise "Parser Bug: #{type}" 
      end
      # puts "#{current_token.inspect} => #{type} : #{@line}:#{@column}"
    end
  end

  class SExpr
    def initialize(pos = nil)
    end

    def self.parse(str)
      parser = Parser.new(str)
      return parser.parse()
    end

    def write(out)
      out << to_s()
    end
  end

  # Boolean
  class Boolean < SExpr
    attr_reader :value

    def initialize(value, pos)
      super(pos)
      @value = value
    end

    def to_s()
      if @value then
        return "#t"
      else
        return "#f"
      end
    end
  end

  # 1025
  class Integer < SExpr
    attr_reader :value

    def initialize(value, pos)
      super(pos)
      @value = value
    end

    def to_s()
      return @value.to_s
    end
  end

  # 5.1
  class Real < SExpr
    attr_reader :value

    def initialize(value, pos)
      super(pos)
      @value = value
    end

    def to_s()
      return @value.to_s
    end
  end

  # "foo"
  class String < SExpr
    attr_reader :value

    def initialize(value, pos = nil)
      super(pos)
      @value = value
    end

    def to_s()
      return @value.to_s # inspect
    end
    
    def write(out)
      out << @value.inspect
    end
  end

  # foo
  class Symbol < SExpr
    attr_reader :value

    def initialize(value, pos = nil)
      super(pos)
      @value = value
    end

    def to_s()
      return @value.to_s
    end
  end

  # (foo bar baz)
  class List < SExpr
    attr_reader :children  

    def initialize(value, pos = nil)
      super(pos)
      @value = value
    end

    def append(el)
      @value << el
    end

    def to_s()
      return "(" + @value.map{|i| i.to_s}.join(" ") + ")"
    end

    def write(out)
      out << "("
      @value.each_with_index{|i, idx|
        i.write(out)
        if (idx != @value.length()-1) then
          out << " "
        end
      }
      out << ")"
    end
  end
end

# EOF #
