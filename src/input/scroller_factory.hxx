//  $Id: scroller_factory.hxx,v 1.9 2003/10/20 19:28:55 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_INPUT_SCROLLER_FACTORY_HXX
#define HEADER_PINGUS_INPUT_SCROLLER_FACTORY_HXX

#include "../file_reader.hxx"

namespace Input {

class Scroller;

class ScrollerFactory
{
private:
  static Scroller* axis_scroller     (FileReader reader);
  static Scroller* inverted_scroller (FileReader reader);
  static Scroller* joystick_scroller (FileReader reader);
  static Scroller* mouse_scroller    (FileReader reader);
  static Scroller* multiple_scroller (FileReader reader);
  static Scroller* pointer_scroller  (FileReader reader);

public:
  static Scroller* create (FileReader reader);

private:
  ScrollerFactory ();
  ScrollerFactory (const ScrollerFactory&);
  ScrollerFactory& operator= (const ScrollerFactory&);
};

} // namespace Input

#endif

/* EOF */
