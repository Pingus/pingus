//  $Id: PingusCounterBar.hh,v 1.1 2001/04/15 20:55:03 grumbel Exp $
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

#ifndef PINGUSCOUNTERBAR_HH
#define PINGUSCOUNTERBAR_HH

#include <ClanLib/core.h>
#include "Server.hh"
#include "GuiObj.hh"
#include "boost/dummy_ptr.hpp"

class PingusCounterBar : public GuiObj
{
public:
    enum Orientation { VERTICAL, HORIZONTAL };
private:
  boost::dummy_ptr<Server> server;
  Orientation orientation;
  CL_Rect rect;
public:
  PingusCounterBar (boost::dummy_ptr<Server> s, Orientation o, const CL_Rect& arg_rect);
  virtual ~PingusCounterBar ();

  void update(float delta);
  void draw ();
};

#endif

/* EOF */
