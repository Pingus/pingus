#!/usr/bin/ruby -w

require "rexml/document"

def parse_sprite(prefix, sprite)
  puts "(sprite"
  puts "  (name \"#{sprite.attribute("name")}\")"
  sprite.elements.each("image") {|el|
    puts "  (file \"#{el.attribute("file")}\"))"
  }
end

def parse_section(prefix, section)
  section.each{|el|
    if el.is_a?(REXML::Text) then
      # skip text nodes, just indention junk
      # puts "'#{el}'" 
    elsif el.is_a?(REXML::Element) then
      case el.name
      when "section"
        parse_section("#{prefix}/#{el.attribute("name")}", el)
      when "sprite"
        parse_sprite(prefix, el)
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
  puts "### Parsing #{arg}"
  dir = File.dirname(arg)
  doc = REXML::Document.new(File.new(arg))
  doc.elements.each("resources") { |el|
    parse_section("", el)
  }
}

# EOF #
