#!/usr/bin/ruby -w

require "sexpr.rb"

if ARGV.empty?() then
  puts SExpr::SExpr.parse("(pi8ngulevel -.51 a (b +1.5) -5)")
else
  ARGV.each{|filename|
    sexpr = SExpr::SExpr.parse(File.new(filename).read())
    sexpr.each{|el|
      el.write($stdout)
    }
  }
end

# EOF #

