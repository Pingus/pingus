//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_SCREEN_PTR_HPP
#define HEADER_PINGUS_SCREEN_PTR_HPP

#include "screen.hpp"


/** The ScreenPtr is a simple smart pointer to point at a Screen. It
    either deletes the pointer or not, depending on the value of
    delete_it */
class ScreenPtr
{
private:
  Screen* screen;
  bool    delete_it;
  long*   ref_count;

public:
  ScreenPtr()
    : screen(0), delete_it(false), ref_count(0)
  {
  }

  ScreenPtr(Screen* arg_screen, bool arg_delete_it)
    : screen(arg_screen), delete_it(arg_delete_it)
  {
    if (delete_it)
      {
	ref_count = new long;
	*ref_count = 1;
      }
    else
      {
	ref_count = 0;
      }
  }

  ScreenPtr (const ScreenPtr& ptr)
    : screen(ptr.screen), delete_it(ptr.delete_it), ref_count(ptr.ref_count)
  {
    increase_refcount();
  }

  bool operator==(const ScreenPtr& ptr)
  {
    return screen == ptr.screen;
  }

  ScreenPtr& operator= (const ScreenPtr& ptr)
  {
    if (this != &ptr)
      {
	decrease_refcount();

	screen    = ptr.screen;
	delete_it = ptr.delete_it;
	ref_count = ptr.ref_count;

	increase_refcount();
      }
    return *this;
  }

  ~ScreenPtr()
  {
    decrease_refcount();
  }

  Screen* operator->()
  {
    return screen;
  }

  const Screen& operator*()
  {
    return *screen;
  }

  Screen* get()
  {
    return screen;
  }

private:
  void decrease_refcount();
  void increase_refcount();
};


#endif

/* EOF */
