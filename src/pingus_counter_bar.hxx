//  $Id: pingus_counter_bar.hxx,v 1.3 2002/08/23 15:49:49 torangan Exp $
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

#ifndef HEADER_PINGUS_PINGUS_COUNTER_BAR_HXX
#define HEADER_PINGUS_PINGUS_COUNTER_BAR_HXX

#include <ClanLib/Core/Math/rect.h>
#include "gui_obj.hxx"

class Server;

class PingusCounterBar : public GuiObj
{
public:
    enum Orientation { VERTICAL, HORIZONTAL };
private:
  Server * server;
  Orientation orientation;
  CL_Rect rect;
public:
  PingusCounterBar (Server * s, Orientation o, const CL_Rect& arg_rect);
  virtual ~PingusCounterBar ();

  void update(float delta);
  void draw ();
  
private:
  PingusCounterBar (const PingusCounterBar&);
  PingusCounterBar operator= (const PingusCounterBar&);
};

#endif

/* EOF */



