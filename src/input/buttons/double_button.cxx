//  $Id: double_button.cxx,v 1.3 2003/10/20 13:33:44 grumbel Exp $
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

#include "../event_fwd.hxx"
#include "double_button.hxx"

namespace Pingus {
namespace Input {
namespace Buttons {

DoubleButton::DoubleButton (Button* button1_, Button* button2_) : button1(button1_), button2(button2_),
                                                                  first_pressed(false), ignore_second(false)
{
}

DoubleButton::~DoubleButton ()
{
  delete button1;
  delete button2;
}

void
DoubleButton::update (float delta)
{
  button1->update(delta);
  button2->update(delta);

  if (button1->is_pressed())
    {
      if (!first_pressed)
        {
          first_pressed = true;
          ignore_second = button2->is_pressed();
        }
    }
  else
    {
      first_pressed = false;
      ignore_second = true;
    }
}

bool
DoubleButton::is_pressed () const
{
  return ( ! ignore_second && first_pressed && button2->is_pressed());
}

} // namespace Buttons
} // namespace Input
} // namespace Pingus

/* EOF */
