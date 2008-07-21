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

class UTF8Iterator
{
private:
  const std::string&     text;
  std::string::size_type pos;
  uint32_t chr;

  /**
   * returns true if this byte matches a bitmask of 10xx.xxxx, i.e. it is the 2nd, 3rd or 4th byte of a multibyte utf8 string
   */
  bool has_multibyte_mark(unsigned char c);

  /**
   * gets unicode character at byte position @a p of UTF-8 encoded @a
   * text, then advances @a p to the next character.
   *
   * @throws std::runtime_error if decoding fails.
   * See unicode standard section 3.10 table 3-5 and 3-6 for details.
   */
  uint32_t decode_utf8(const std::string& text, size_t& p);

public:
  UTF8Iterator(const std::string& text_);
  bool done() const;
  UTF8Iterator& operator++();
  uint32_t operator*() const;
};

#endif

/* EOF */
