//  $Id: MultiplayerClient.hh,v 1.8 2002/06/07 20:35:14 torangan Exp $
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

#ifndef MULTIPLAYERCLIENT_HH
#define MULTIPLAYERCLIENT_HH

#include <vector>
#include "GuiObj.hh"
#include "MultiplayerClientChild.hh"

namespace boost {
  template <class T> class smart_ptr;
}

class MultiplayerClient : public GuiObj
{
private:
  std::vector<boost::shared_ptr<GuiObj> > gui_objs;
  typedef std::vector<boost::shared_ptr<GuiObj> >::iterator GuiObjIter;
  Server * server;
    
public:
  MultiplayerClient (Server * s,
		     boost::shared_ptr<GuiObj> child1,
		     boost::shared_ptr<GuiObj> child2);
  MultiplayerClient (Server * s,
		     boost::shared_ptr<GuiObj> child1,
		     boost::shared_ptr<GuiObj> child2,
		     boost::shared_ptr<GuiObj> child3,
		     boost::shared_ptr<GuiObj> child4);
  ~MultiplayerClient ();

  void update (float delta);
  void draw ();
};

#endif

/* EOF */
