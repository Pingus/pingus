//  $Id: entrance.hxx,v 1.3 2002/10/02 19:20:19 grumbel Exp $
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

#ifndef HEADER_PINGUS_ENTRANCE_HXX
#define HEADER_PINGUS_ENTRANCE_HXX

#include <ClanLib/Display/Display/surface.h>
#include "../worldobj.hxx"

namespace WorldObjsData {
class EntranceData;
}

class Pingu;

namespace WorldObjs {

/** A abstract representation of an entrance, the implementation
    currently sucks and needs to be rewritten */
class Entrance : public WorldObj
{
protected:
  WorldObjsData::EntranceData* const data;
  
  CL_Surface surface;
  int        last_release;
  
public:
  Entrance (const WorldObjsData::EntranceData& data_);
 ~Entrance ();

  float get_z_pos () const;
  
  virtual bool   pingu_ready ();
  virtual void   create_pingu ();
  
  virtual void   update ();
  
  virtual void   draw (GraphicContext& gc);
  
private:
  Entrance (const Entrance&);
  Entrance& operator= (const Entrance&);
};

} // namespace WorldObjs

#endif

/* EOF */
