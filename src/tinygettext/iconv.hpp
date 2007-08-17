//  $Id$
// 
//  TinyGetText - A small flexible gettext() replacement
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_ICONV_HPP
#define HEADER_ICONV_HPP

#include <string>

namespace TinyGetText {

class IConv
{
public:
  std::string to_charset;
  std::string from_charset;
  iconv_t m_conv;

  IConv();
  IConv(const std::string& fromcode, const std::string& tocode);
  ~IConv();

  void close();
 
  /// Convert a string from encoding to another.
  std::string convert(std::string text);

  static std::string convert(const std::string& text,
                             const std::string& from_charset,
                             const std::string& to_charset);
};

} // namespace TinyGetText

#endif

/* EOF */
