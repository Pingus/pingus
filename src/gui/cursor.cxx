//  $Id: cursor.cxx,v 1.3 2003/04/19 10:23:18 torangan Exp $
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

#include "../input/controller.hxx"
#include "../input/pointer.hxx"
#include "cursor.hxx"

Cursor::Cursor (std::string ident, std::string datafile)
  : sprite (ident, datafile, 15.0f)
{
  sprite.set_align_center();
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
  const Input::Pointer* pointer = Input::Controller::get_current()->get_pointer();
  sprite.put_screen (static_cast<int>(pointer->get_x_pos ()),
                     static_cast<int>(pointer->get_y_pos ()));
}

/* EOF */
