//  $Id: Entrance.hh,v 1.16 2001/04/16 11:58:34 grumbel Exp $
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

#ifndef ENTRANCE_HH
#define ENTRANCE_HH

#include <string>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "Pingu.hh"
#include "WorldObj.hh"
#include "ActionHolder.hh"
#include "EntranceData.hh"

/** A abstract representation of an entrance, the implementation
    currently sucks and needs to be rewritten */
class Entrance : public EntranceData, 
		 public WorldObj
{
protected:
  ///
  CL_Surface surface;
  ///
  int last_release;
  
public:
  ///
  Entrance();
  ///
  Entrance(const EntranceData& data);
  ///
  virtual ~Entrance() {}
  ///
  int  get_z_pos() const { return int(pos.z); }
  ///
  virtual bool   pingu_ready(void);
  ///
  virtual boost::shared_ptr<Pingu> get_pingu(void);
  ///
  virtual void   update(float delta);
  ///
  virtual void   draw_offset(int x, int y, float s = 1.0);
};

#endif

/* EOF */
