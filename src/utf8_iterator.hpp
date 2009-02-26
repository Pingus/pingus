//  Pingus - A free Lemmings clone
//  Copyright (C) 2008 Matthias Braun <matze@braunis.de>, 
//                     Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_UTF8_ITERATOR_HPP
#define HEADER_PINGUS_UTF8_ITERATOR_HPP

#include <stdint.h>
#include <string>

class UTF8
{
public:
  static std::string::size_type length(const std::string& str);
  static std::string substr(const std::string& text, std::string::size_type pos, std::string::size_type n);
  static std::string::const_iterator advance(std::string::const_iterator it, std::string::size_type n = 1);

  /** return true if a linebreak is allowed after this character */
  static bool is_linebreak_character(uint32_t unicode);
};

class UTF8Iterator
{
private:
  const std::string&     text;
  std::string::size_type pos;
  uint32_t chr;

public:
  /**
   * returns true if this byte matches a bitmask of 10xx.xxxx, i.e. it is the 2nd, 3rd or 4th byte of a multibyte utf8 string
   */
  static bool has_multibyte_mark(unsigned char c);

  /**
   * gets unicode character at byte position @a p of UTF-8 encoded @a
   * text, then advances @a p to the next character.
   *
   * @throws std::runtime_error if decoding fails.
   * See unicode standard section 3.10 table 3-5 and 3-6 for details.
   */
  static uint32_t decode_utf8(const std::string& text, size_t& p);

  static uint32_t decode_utf8(const std::string& text);
  
public:
  UTF8Iterator(const std::string& text_);
  bool done() const;
  UTF8Iterator& operator++();
  uint32_t operator*() const;
};

#endif

/* EOF */
