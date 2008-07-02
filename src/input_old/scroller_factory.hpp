//  $Id: scroller_factory.hpp 3582 2008-06-29 16:56:56Z grumbel $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_INPUT_SCROLLER_FACTORY_HPP
#define HEADER_PINGUS_INPUT_SCROLLER_FACTORY_HPP

#include "../file_reader.hpp"

namespace Input {

class Scroller;

class ScrollerFactory
{
private:
  static Scroller* axis_scroller(FileReader reader);
  static Scroller* inverted_scroller(FileReader reader);
  static Scroller* joystick_scroller(FileReader reader);
  static Scroller* mouse_scroller(FileReader reader);
  static Scroller* multiple_scroller(FileReader reader);
  static Scroller* pointer_scroller(FileReader reader);

public:
  static Scroller* create(FileReader reader);

private:
  ScrollerFactory();
  ScrollerFactory(const ScrollerFactory&);
  ScrollerFactory& operator= (const ScrollerFactory&);
};

} // namespace Input

#endif

/* EOF */
