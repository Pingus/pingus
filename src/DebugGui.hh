//  $Id: DebugGui.hh,v 1.6 2002/06/07 13:25:02 torangan Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef DEBUGGUI_HH
#define DEBUGGUI_HH

#include "GuiObj.hh"

class World;

///
class DebugGui : public GuiObj
{
private:
  ///
  CL_Font* font;
  ///
  bool debug_active;
  ///
  World* world;
public:
  ///
  DebugGui(void);
  ///
  ~DebugGui();

  ///
  void update(float delta);
  ///
  void draw(void);
  ///
  void toggle_debug(void);
  ///
  void set_world(World* w);
};

#endif

/* EOF */
