#!/usr/bin/ruby -w

require "rexml/document"
require "rexml/xpath"

puts "# SOME DESCRIPTIVE TITLE."
puts "# Copyright (C) YEAR Free Software Foundation, Inc."
puts "# FIRST AUTHOR <EMAIL@ADDRESS>, YEAR."
puts "#"
puts "#, fuzzy"
puts "msgid \"\""
puts "msgstr \"\""
puts "\"Project-Id-Version: PACKAGE VERSION\\n\""
puts "\"PO-Revision-Date: YEAR-MO-DA HO:MI +ZONE\\n\""
puts "\"Last-Translator: FULL NAME <EMAIL@ADDRESS>\\n\""
puts "\"Language-Team: LANGUAGE <LL@li.org>\\n\""
puts "\"MIME-Version: 1.0\\n\""
puts "\"Content-Type: text/plain; charset=UTF-8\\n\""
puts "\"Content-Transfer-Encoding: 8bit\\n\""
puts ""

lang = ARGV[0]
ARGV[1..-1].each{|arg|
  doc = REXML::Document.new(File.new(arg))

  ["/pingus-level/global/description", "/pingus-level/global/levelname"].each{|tree|
    element = REXML::XPath.match(doc, "#{tree}[@lang='en']")
    
    puts "#: #{arg.gsub(/^\.\.\/\.\.\/pingus_0_6\//, "")}"
    puts "msgid \"#{element[0].text.gsub(/\s+/, " ")}\""

    element = REXML::XPath.match(doc, "#{tree}[@lang='#{lang}']")
    if (element.length >= 1)
      puts "msgstr \"#{element[0].text.gsub(/\s+/, " ")}\""    
    else
      puts "msgstr \"\""
    end 

    puts ""
  }
}

# EOF #
