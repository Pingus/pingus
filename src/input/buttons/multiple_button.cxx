//  $Id: multiple_button.cxx,v 1.2 2003/04/19 10:23:19 torangan Exp $
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

#include "multiple_button.hxx"

namespace Input {

  namespace Buttons {

    MultipleButton::MultipleButton (const std::vector<Button*>& buttons_) : buttons(buttons_)
    {
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

  }
}

/* EOF */
