#!/usr/bin/ruby

require "rexml/document"

$lang = "cs"

def prett_print(str)
  str = str.strip
  str = str.gsub(/[ \t]+/, ' ').gsub('\"', '\\\"')

  if str.count("\n") == 0 then
    puts "\"#{str}\""
  else
    puts "\"\""
    str.each {|line|
      puts "\"#{line.strip}\""
    }
  end  
end

def print_msg(doc, path, handle_lang = true)
  node = doc.elements[path + if handle_lang then "[@lang='en']" else "" end]
  
  if node then
    str = node.text
    print "msgid = "
    prett_print(str)
    
    print "msgstr = "
    
    transnode = doc.elements[path + if handle_lang then "[@lang='#{$lang}']" else "" end]
    if transnode then
      prett_print(transnode.text)
    else
      puts "\"\""
    end
    puts ""
  end
end

puts "# Level Translations"
puts "# Copyright (C) 2004 Ingo Ruhnke"
puts "# This file is distributed under the same license as the Pingus package."
puts "# Ingo Ruhnke <grumbel@gmx.de>, 2004."
puts ""

puts "msgid \"\""
puts "msgstr \"\""
puts "\"Project-Id-Version: PACKAGE VERSION\"\n"
puts "\"Report-Msgid-Bugs-To: \"\n"
puts "\"POT-Creation-Date: 2005-02-26 23:07+0100\"\n"
puts "\"PO-Revision-Date: YEAR-MO-DA HO:MI+ZONE\"\n"
puts "\"Last-Translator: FULL NAME <EMAIL@ADDRESS>\"\n"
puts "\"Language-Team: LANGUAGE <LL@li.org>\"\n"
puts "\"MIME-Version: 1.0\"\n"
puts "\"Content-Type: text/plain; charset=UTF-8\"\n"
puts "\"Content-Transfer-Encoding: 8bit\"\n"
puts ""

ARGV.each {|filename|
  doc = REXML::Document.new(File.new(filename))

  puts "# #{filename}"
  print_msg(doc, "/pingus-level/global/levelname")
  print_msg(doc, "/pingus-level/global/description") 
}

puts "# EOF #"

# EOF #
