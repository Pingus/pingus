// Pingus - A free Lemmings clone
// Copyright (C) 1998-2009 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>

#include "util/line_iterator.hpp"

void test(const std::string& str)
{
  std::cout << "Testing: " << std::endl;
  std::cout << "in:  \"" << str << "\"" << std::endl;

  std::cout << "out: " << std::flush;

  LineIterator it(str);
  while(it.next())
    std::cout << "\"" << it.get() << "\" " << std::flush;

  std::cout << std::endl;
  std::cout << std::endl;
}

int main()
{
  test("Test One");
  test("Test\nTwo");
  test("\nHello\nThree");
  test("Hello Four\n");
  test("Hello\nWorld\nFooBar\n");
  test("Hello\nWorld\nFooBar\n\n");
  test("1");
  return 0;
}

/* EOF */
