//  $Id: dummy_pointer.hxx,v 1.5 2003/10/20 19:28:55 grumbel Exp $
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

#ifndef HEADER_PINGUS_INPUT_DUMMY_POINTER_HXX
#define HEADER_PINGUS_INPUT_DUMMY_POINTER_HXX

#include "../pointer.hxx"

namespace Input {
namespace Pointers {

/**
   @brief dummy class to be used if a pointer is required but none defined

   XML definition: none
*/
class DummyPointer : public Pointer {

private:
  const float pos;

public:

  DummyPointer () : pos(0) { }

  virtual const float& get_x_pos () const { return pos; }
  virtual const float& get_y_pos () const { return pos; }

  virtual void  set_pos (float, float) { }
  virtual void  update (float)         { }

private:
  DummyPointer (const DummyPointer&);
  DummyPointer& operator= (const DummyPointer&);
};

} // namespace Pointers
} // namespace Input

#endif

/* EOF */
