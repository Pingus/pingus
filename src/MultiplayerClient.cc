//  $Id: MultiplayerClient.cc,v 1.2 2001/04/15 00:53:11 grumbel Exp $
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

#include "MultiplayerClient.hh"

MultiplayerClient::MultiplayerClient (boost::dummy_ptr<Server> s,
				      boost::shared_ptr<MultiplayerClientChild> child1,
				      boost::shared_ptr<MultiplayerClientChild> child2,
				      boost::shared_ptr<MultiplayerClientChild> child3,
				      boost::shared_ptr<MultiplayerClientChild> child4)
  : server (s)
{
  gui_objs.push_back (child1);
  gui_objs.push_back (child2);
  gui_objs.push_back (child3);
  gui_objs.push_back (child4);
}

MultiplayerClient::MultiplayerClient (boost::dummy_ptr<Server> s,
				      boost::shared_ptr<MultiplayerClientChild> child1,
				      boost::shared_ptr<MultiplayerClientChild> child2)
  : server (s)
{
  gui_objs.push_back (child1);
  gui_objs.push_back (child2);
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
