#!/usr/bin/ruby -w

require "rexml/document"

def parse_sprite(prefix, dir, sprite)
  puts ";; data/images/#{dir[1..-1]}/#{sprite.attribute("name")}.sprite"
  puts "(sprite"
  sprite.elements.each{|el|
    case el.name
    when "image"
      puts "  (images \"#{File.basename(el.attribute("file").value)}\")"
    when "translation"
      puts "  (origin \"#{el.attribute("origin")}\")"
      puts "  (offset #{el.attribute("x") or 0} #{el.attribute("y") or 0})"
    when "animation"
      
    else
      raise "Unhandled tag: #{el.name}"
    end      
  }
  puts " )\n\n"
  puts ";; EOF ;;\n\n"
end

def parse_section(prefix, dir, section)
  section.each{|el|
    if el.is_a?(REXML::Text) then
      # skip text nodes, just indention junk
      # puts "'#{el}'" 
    elsif el.is_a?(REXML::Element) then
      case el.name
      when "section"
        #puts "(section"
        #puts "  (name \"#{el.attribute("name")}\")"
        parse_section("  ", dir + "/#{el.attribute("name")}" , el)
        #puts " )"
      when "sprite"
        parse_sprite(prefix, dir, el)
      when "surface"
        puts "surface: #{prefix}/#{el.attribute("name")}"
      else
        puts "Unknown element: #{el.name}"
      end
    else
      put "<unknown>"
    end
  }
end

ARGV.each{|arg|
  i = 0
  puts ";; File: #{arg}"
  dir = File.dirname(arg)
  doc = REXML::Document.new(File.new(arg))
  doc.elements.each("resources") { |el|
    parse_section("", "", el)
  }
  puts "\n;; EOF ;;"
}

# EOF #
