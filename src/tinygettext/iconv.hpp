//  $Id: iconv.hpp 2990 2007-08-17 16:42:30Z jsalmon3 $
// 
//  TinyGetText - A small flexible gettext() replacement
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_ICONV_HPP
#define HEADER_ICONV_HPP

#include <string>
#include "SDL_stdinc.h"

namespace TinyGetText {

class IConv
{
public:
  std::string to_charset;
  std::string from_charset;
  SDL_iconv_t m_conv;

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
