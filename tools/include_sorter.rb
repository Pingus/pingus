#!/usr/bin/ruby -w

# Takes a list of files and sorts #include directives in them
# alphabetically, a extra newline will be inserted between system
# includes and local includes if not already present.

class Content
  def initialize()
    @content = []
    @last_group = nil
  end

  def append(group, text)
    if @content.empty? then
      @content.push([group, [text]])
    else
      (current_group, current_lst) = @content.last
      if current_group == group then
        current_lst.push(text)
      else
        @content.push([group, [text]])
      end
    end
  end

  def sort_and_write(out)
    @content.each{ |item|
      (group, lst) = item

      # sort includes
      if group == :user_include or group == :sys_include then
        lst.sort!
      end
      
      # insert empty lines between include groups
      if (@last_group == :user_include and group == :sys_include or 
          @last_group == :sys_include and group == :user_include) then
        out.puts
      end

      lst.each{|line|
        out.puts line
      }

      @last_group = group
    }
  end
end

ARGV.each { |arg|
  lines = File.new(arg).readlines

  content = Content.new()

  lines.each { |line|
    if line =~ /^#include \"/ then
      content.append(:user_include, line)
    elsif line =~ /^#include </ then
      content.append(:sys_include, line)
    else
      content.append(:content, line)
    end
  }

  content.sort_and_write(File.new(arg, "w"))
}

# EOF #
