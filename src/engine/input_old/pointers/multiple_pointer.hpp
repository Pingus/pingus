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

#ifndef HEADER_PINGUS_ENGINE_INPUT_OLD_POINTERS_MULTIPLE_POINTER_HPP
#define HEADER_PINGUS_ENGINE_INPUT_OLD_POINTERS_MULTIPLE_POINTER_HPP

#include <vector>
#include "../pointer.hpp"

namespace Input {
namespace Pointers {

/**
   @brief wrapper class allowing multiple pointers to be used as one

   XML definition: <multiple-pointer> <pointer 1>...<pointer N> </multiple-pointer>

   This class will check every contained pointer for changes and if any changes all
   pointers are updated to the new coordinates. If more than one pointer changes at
   the same time only the change of the first will be registered.
*/
class MultiplePointer : public Pointer {

private:
  std::vector<Pointer*> pointers;

  float old_x_pos;
  float old_y_pos;

  float x_pos;
  float y_pos;

public:
  MultiplePointer (const std::vector<Pointer*>& pointers_);
  ~MultiplePointer ();

  virtual const float& get_x_pos () const;
  virtual const float& get_y_pos () const;

  virtual void set_pos (float x_pos, float y_pos);

  virtual void update (float delta);

private:
  MultiplePointer (const MultiplePointer&);
  MultiplePointer& operator= (const MultiplePointer&);
};

} // namespace Pointers
} // namespace Input

#endif

/* EOF */
