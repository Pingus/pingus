//  $Id: pointer_factory.hxx,v 1.8 2003/10/20 19:28:55 grumbel Exp $
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

#ifndef HEADER_PINGUS_INPUT_POINTER_FACTORY_HXX
#define HEADER_PINGUS_INPUT_POINTER_FACTORY_HXX

#include "../file_reader.hpp"

namespace Input {

class Pointer;

class PointerFactory
{
private:
  static Pointer* axis_pointer(FileReader reader);
  static Pointer* mouse_pointer();
  static Pointer* multiple_pointer(FileReader reader);

public:
  static Pointer* create(FileReader reader);

private:
  PointerFactory();
  PointerFactory(const PointerFactory&);
  PointerFactory& operator= (const PointerFactory&);
};

} // namespace Input

#endif

/* EOF */
