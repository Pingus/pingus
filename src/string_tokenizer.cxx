//  $Id: string_tokenizer.cxx,v 1.1 2003/04/14 17:56:36 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include "string_tokenizer.hxx"

StringTokenizer::StringTokenizer(const std::string& str, char split_char)
{
  std::string::size_type start = 0;
  std::string::size_type end   = 0;

  while (start < str.size())
    {
      if ((end = str.find(split_char, start)) == std::string::npos)
        {
          tokens.push_back(str.substr(start));
          break;
        }

      const std::string& ret = str.substr(start, end - start);

      if (!ret.empty())
        tokens.push_back(ret);

      start = end + 1;
    }
}

#ifdef TEST_ME
#include <iostream>

int main()
{
  StringTokenizer tokenizer("Hello World:aoeu:aoeu:aeu:aeuaoeu::Aoeuaoe123::1", ':');
  for(StringTokenizer::iterator i = tokenizer.begin(); i != tokenizer.end(); ++i)
    std::cout << "Str: '" << *i << "'" << std::endl;
}
#endif

/* EOF */
