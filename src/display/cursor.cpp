//  $Id: cursor.cpp 3265 2007-09-30 16:07:08Z grumbel $
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

#include "../input/controller.hpp"
#include "../input/control.hpp"
#include "../resource.hpp"
#include "cursor.hpp"

Cursor::Cursor(const std::string& ident)
  : sprite(Resource::load_sprite(ident))
{
}

Cursor::~Cursor ()
{
}

void
Cursor::update (float delta)
{
  sprite.update (delta);
}

void
Cursor::on_event()
{
  Vector2f pos = Input::Controller::current()->get_pointer(Input::STANDARD_POINTER)->get_pos();
  sprite.draw(pos.x, pos.y, Display::get_screen());
}


/* EOF */
