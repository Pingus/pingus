//  $Id: cursor.hxx,v 1.2 2003/04/19 10:23:18 torangan Exp $
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

#ifndef HEADER_PINGUS_CURSOR_HXX
#define HEADER_PINGUS_CURSOR_HXX

#include <string>
#include "display.hxx"
#include "../sprite.hxx"

/** A simple mouse cursor class and stupid. We can't use the X11/win32
    nativ mouse cursor because we want also to support non-mouse input
    devices and because we want to support multiple cursors */
class Cursor : public DisplayHook
{
private:
  Sprite sprite;

public:
  Cursor (std::string ident, std::string datafile);
  virtual ~Cursor ();

  virtual void update (float delta);
  virtual void on_event();

private:
  Cursor (const Cursor&);
  Cursor& operator= (const Cursor&);
};

#endif

/* EOF */
