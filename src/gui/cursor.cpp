//  $Id$
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

#include "../input/controller.hpp"
#include "../input/pointer.hpp"
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
  int x,y;
  SDL_GetMouseState(&x, &y);
  sprite.draw((float)x, (float)y, Display::get_screen());
}


/* EOF */
