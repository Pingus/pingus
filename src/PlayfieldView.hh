//  $Id: PlayfieldView.hh,v 1.3 2001/04/14 14:37:04 grumbel Exp $
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

#ifndef PLAYFIELDVIEW_HH
#define PLAYFIELDVIEW_HH

#include "boost/dummy_ptr.hpp"
#include "GuiObj.hh"
#include "World.hh"

class PlayfieldView : public GuiObj
{
private:
  boost::dummy_ptr<World> world;
  int x_offset;
  int y_offset;
public:
  PlayfieldView (boost::dummy_ptr<World> w,
		 const CL_Rect& rect);
  virtual ~PlayfieldView ();

  // Update the playfield
  void update (float delta);
  
  // Draw the playfied
  void draw ();
  
  // Scroll the playfield
  void scroll (CL_Vector delta);
  
  // Return the pingu at the given *screen* coordinates
  boost::shared_ptr<Pingu> get_pingu (const CL_Vector& pos);
};

#endif

/* EOF */
