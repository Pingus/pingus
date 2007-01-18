//  $Id: multiple_button.cxx,v 1.3 2003/10/20 13:33:44 grumbel Exp $
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

#include <iostream>
#include "multiple_button.hxx"

namespace Input {
namespace Buttons {

MultipleButton::MultipleButton (const std::vector<Button*>& buttons_) : buttons(buttons_)
{
  down_count = 0;

  for (std::vector<Button*>::iterator it = buttons.begin(); it != buttons.end(); it++)
    {
      slots.push_back((*it)->sig_button_up()  .connect(this, &MultipleButton::on_button_up));
      slots.push_back((*it)->sig_button_down().connect(this, &MultipleButton::on_button_down));
    }
}

MultipleButton::~MultipleButton ()
{
  for (std::vector<Button*>::iterator it = buttons.begin(); it != buttons.end(); it++)
    delete *it;
}

void
MultipleButton::update (float delta)
{
  for (std::vector<Button*>::iterator it = buttons.begin(); it != buttons.end(); it++)
    (*it)->update(delta);
}

bool
MultipleButton::is_pressed () const
{
  for (std::vector<Button*>::const_iterator it = buttons.begin(); it != buttons.end(); it++)
    if ((*it)->is_pressed())
      return true;

  return false;
}

void
MultipleButton::on_button_up()
{
  down_count -= 1;
  if (down_count < 0)
    down_count = 0;

  if (down_count == 0)
    button_up();
}

void
MultipleButton::on_button_down()
{
  if (down_count == 0)
    button_down();

  down_count += 1; 
}

} // namespace Buttons
} // namespace Input

/* EOF */
