#!/usr/bin/ruby -w

require "rexml/document"

# Script to convert Pingus 0.6 level files to new sexpr format

$typespec = 
  [
   [/^\/pingus-worldmap$/, :section],
   [/^\/pingus-worldmap\/head$/, :section],
   [/^\/pingus-worldmap\/head\/author$/, :string],
   [/^\/pingus-worldmap\/head\/default-node$/, :string],
   [/^\/pingus-worldmap\/head\/description$/, :string],
   [/^\/pingus-worldmap\/head\/email$/, :string],
   [/^\/pingus-worldmap\/head\/final-node$/, :string],
   [/^\/pingus-worldmap\/head\/height$/, :integer],
   [/^\/pingus-worldmap\/head\/music$/, :string],
   [/^\/pingus-worldmap\/head\/name$/, :string],
   [/^\/pingus-worldmap\/head\/short-name$/, :string],
   [/^\/pingus-worldmap\/head\/width$/, :integer],
   [/^\/pingus-worldmap\/objects\/surface\/position$/, :vector],
   [/^\/pingus-worldmap\/objects\/surface\/name$/, :string],
   [/^\/pingus-worldmap\/(intro_story|end_story)$/, :section],
   [/^\/pingus-worldmap\/(intro_story|end_story)\/title$/, :string],
   [/^\/pingus-worldmap\/(intro_story|end_story)\/music$/, :string],
   [/^\/pingus-worldmap\/(intro_story|end_story)\/pages$/, :section],
   [/^\/pingus-worldmap\/(intro_story|end_story)\/pages\/page[0-9][0-9]$/, :section],
   [/^\/pingus-worldmap\/(intro_story|end_story)\/pages\/page[0-9][0-9]\/surface$/, :section],
   [/^\/pingus-worldmap\/(intro_story|end_story)\/pages\/page[0-9][0-9]\/surface\/image$/, :string],
   [/^\/pingus-worldmap\/(intro_story|end_story)\/pages\/page[0-9][0-9]\/surface\/modifer$/, :string],
   [/^\/pingus-worldmap\/(intro_story|end_story)\/pages\/page[0-9][0-9]\/text$/, :string],
   [/^\/pingus-worldmap\/graph$/, :section],
   [/^\/pingus-worldmap\/graph\/nodes$/, :section],
   [/^\/pingus-worldmap\/graph\/nodes\/leveldot$/, :section],
   [/^\/pingus-worldmap\/graph\/nodes\/leveldot\/dot$/, :section],
   [/^\/pingus-worldmap\/graph\/nodes\/leveldot\/dot\/name$/, :string],
   [/^\/pingus-worldmap\/graph\/nodes\/leveldot\/dot\/position$/, :vector],
   [/^\/pingus-worldmap\/graph\/nodes\/leveldot\/levelname$/, :string],
   [/^\/pingus-worldmap\/graph\/edges$/, :section],
   [/^\/pingus-worldmap\/graph\/edges\/edge$/, :section],
   [/^\/pingus-worldmap\/graph\/edges\/edge\/name$/, :string],
   [/^\/pingus-worldmap\/graph\/edges\/edge\/source$/, :string],
   [/^\/pingus-worldmap\/graph\/edges\/edge\/destination$/, :string],
   [/^\/pingus-worldmap\/graph\/edges\/edge\/positions$/, :section],
   [/^\/pingus-worldmap\/graph\/edges\/edge\/positions\/position$/, :vector],
   [/^\/pingus-worldmap\/objects$/, :section],

   [/surface$/, :section],
   [/surface\/image$/, :string],
   [/surface\/modifer$/, :string],
   [/surface\/modifier$/, :string],
   [/surface\/auto-uncover$/, :integer],
  ]

def get_type(section)
  $typespec.each{|pair|
    if section.match(pair[0]) then
      return pair[1]
    end
  }
  return :unknown
end

def xml2array(section, indent, el)
  if el.is_a?(REXML::Text) then
    # puts section #, " -> ", $typespec[section], "\n"
    case get_type(section)
    when :section 
      # indention white space, ignore it
    when :string
      print "\"#{el.value}\""
    when :integer
      print el.value.to_i
    else
      puts "unknown: section: #{section} #{el.value}"
    end
    
  elsif el.is_a?(REXML::Element)
    case get_type("#{section}/#{el.name}")
    when :section 
      puts "#{indent}(#{el.name} "
      el.children.each{|child|
        xml2array("#{section}/#{el.name}", "#{indent}  ", child)
      }
      puts "#{indent} )"
    when :string, :integer
      print "#{indent}(#{el.name} "
      el.children.each{|child|
        xml2array("#{section}/#{el.name}", "#{indent}  ", child)
      }
      puts ")"
    when :vector
      print "#{indent}(#{el.name} "
      print el.elements["x"][0].value
      print " "
      print el.elements["y"][0].value
      print " "
      print el.elements["z"][0].value
      puts ")"
    else
      puts "#{indent}(#{el.name} "
      el.children.each{|child|
        xml2array("#{section}/#{el.name}", "#{indent}  ", child)
      }
      puts "#{indent} )"
    end
    
  elsif el.is_a?(REXML::Comment)
  elsif el.is_a?(REXML::XMLDecl)
  else
    $stderr.puts "Error, unknown element #{el.class}"
  end
end

ARGV.each{|arg|
  i = 0
  dir = File.dirname(arg)
  doc = REXML::Document.new(File.new(arg))
  doc.children.each{ |el|
    xml2array("", "", el)
  }
  puts "\n;; EOF ;;"
}

# EOF #
