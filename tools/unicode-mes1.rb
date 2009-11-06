#! /usr/bin/ruby -w 

# http://www.eki.ee/letter/chardata.cgi?comp1=MES-1&comp2
# use with:
# ./mes1.rb | recode UCS-4BE..UTF-8


# removed: 0x0020 - space 
mes1 = 
  (0x0021..0x007E).to_a + (0x00A0..0x00FF).to_a +
  (0x0100..0x0113).to_a + (0x0116..0x012B).to_a + (0x012E..0x014D).to_a + (0x0150..0x017E).to_a +
  [0x02C7] + (0x02D8..0x02DB).to_a + [0x02DD] +
  [0x2015] + (0x2018..0x2019).to_a + (0x201C..0x201D).to_a + [0x20AC] +
  [0x2122] + [0x2126] + (0x215B..0x215E).to_a + (0x2190..0x2193).to_a +
  [0x266A]

mes1.each{|i|
  print [i].pack("N")
  print [0x0020].pack("N") # space
  print [0x0020].pack("N") # space
}

print [0x000a].pack("N") # newline

# EOF #



