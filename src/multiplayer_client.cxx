//  $Id: multiplayer_client.cxx,v 1.2 2002/06/13 10:49:06 torangan Exp $
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

#include <ClanLib/Display/Display/display.h>
#include "smallmap_image.hxx"
#include "multiplayer_client.hxx"

MultiplayerClient::MultiplayerClient (Server * s,
				      GuiObj* child1,
				      GuiObj* child2,
				      GuiObj* child3,
				      GuiObj* child4)
  : gui_objs(5), server (s)
{
  gui_objs[0] = child1;
  gui_objs[1] = child2;
  gui_objs[2] = child3;
  gui_objs[3] = child4;
  gui_objs[4] = new SmallMapImage (s, CL_Vector (CL_Display::get_width ()/2 - 100,
 				   CL_Display::get_height ()/2 - 75), 200, 150);
}

MultiplayerClient::MultiplayerClient (Server * s,
				      GuiObj* child1,
				      GuiObj* child2)
  : gui_objs(2), server (s)
{
  gui_objs[0] = child1;
  gui_objs[1] = child2;
}

MultiplayerClient::~MultiplayerClient ()
{
}

void
MultiplayerClient::update (float delta)
{
  for (GuiObjIter i = gui_objs.begin (); i != gui_objs.end (); ++i) {
    //std::cout << "updating: " << (*i).get () << std::endl;
    (*i)->update (delta);
  }
}

void 
MultiplayerClient::draw ()
{
  for (GuiObjIter i = gui_objs.begin (); i != gui_objs.end (); ++i) {
    //std::cout << "Drawing: " << (*i).get () << std::endl;
    (*i)->draw ();    
  }
}

/* EOF */
