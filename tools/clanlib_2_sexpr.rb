#!/usr/bin/ruby -w

require "rexml/document"

def parse_surface(prefix, dir, sprite)
  # convert surfaces to sprites, since we don't really use surfaces anyway
  print "\n#{prefix}(sprite"
  print "\n#{prefix}  (name \"#{sprite.attribute("name").value}\")"
  print "\n#{prefix}  (images \"#{sprite.attribute("file").value}\")"
  puts ")\n"
end

def parse_sprite(prefix, dir, sprite)
  # puts ";; data/images/#{dir[1..-1]}/#{sprite.attribute("name")}.sprite"
  print "\n#{prefix}(sprite"
  print "\n#{prefix}  (name \"#{sprite.attribute("name").value}\")"
  sprite.elements.each{|el|
    case el.name
    when "image"
      if not el.has_elements? then
        print "\n#{prefix}  (image-file \"#{el.attribute("file").value}\")"
      elsif el.elements["grid"] then
        print "\n#{prefix}  (image-file \"#{el.attribute("file").value}\")"

        if el.elements["grid"].attribute("array") then
          print "\n#{prefix}  (image-array #{el.elements["grid"].attribute("array").value.gsub(",", " ")})" 
        end

        if el.elements["grid"].attribute("size") then
          print "\n#{prefix}  (image-size #{el.elements["grid"].attribute("size").value.gsub(",", " ")})"  
        end

        if el.elements["grid"].attribute("pos") then
          print "\n#{prefix}  (image-pos #{el.elements["grid"].attribute("pos").value.gsub(",", " ")})"
        end
      else
        puts "unknown font element: "
      end
    when "translation"
      print "\n#{prefix}  (origin \"#{el.attribute("origin")}\")"
      print "\n#{prefix}  (offset #{el.attribute("x") or 0} #{el.attribute("y") or 0})"
    when "animation"
      if el.attribute("loop") and el.attribute("loop").value == "no" then
        print "\n#{prefix}  (loop #f)"
      end

      if el.attribute("speed") then
        print "\n#{prefix}  (speed #{el.attribute("speed")})"
      end
    else
      raise "Unhandled tag: #{el.name}"
    end    
  }
  puts ")\n"
end

def parse_alias(prefix, dir, section)
  print "\n#{prefix}(alias (name \"#{section.attribute("name").value}\")"
  print "\n#{prefix}       (link \"#{section.attribute("link").value}\"))"
  puts
end

def parse_section(prefix, dir, section)
  section.each{|el|
    if el.is_a?(REXML::Text) then
      # skip text nodes, just indention junk
      # puts "'#{el}'" 
    elsif el.is_a?(REXML::Element) then
      case el.name
      when "section"
        print "\n#{prefix}(section (name \"#{el.attribute("name")}\")"
        parse_section("#{prefix}  ", dir + "/#{el.attribute("name")}" , el)
        puts "#{prefix} )"
      when "sprite"
        parse_sprite(prefix, dir, el)
      when "surface"
        parse_surface(prefix, dir, el)
      when "alias"
        parse_alias(prefix, dir, el)
      else
        puts "unknown element: #{el.name}"
      end
    elsif el.is_a?(REXML::Comment) then
      print "\n#{prefix};; #{el.to_s}"
    else
      puts "<unknown>: #{el.inspect}"
    end
  }
end

ARGV.each{|arg|
  i = 0
  dir = File.dirname(arg)
  doc = REXML::Document.new(File.new(arg))
  doc.elements.each("resources") { |el|
    print ";; #{arg}\n(pingus-resources"
    parse_section("  ", "", el)
    puts " )"
  }
  puts "\n;; EOF ;;"
}

# EOF #
