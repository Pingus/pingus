//  $Id: spike_data.hxx,v 1.11 2003/10/20 13:11:09 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLDOBJSDATA_SPIKE_DATA_HXX
#define HEADER_PINGUS_WORLDOBJSDATA_SPIKE_DATA_HXX

#include "../vector.hxx"
#include <ClanLib/Display/sprite.h>
#include "../game_counter.hxx"
#include "../libxmlfwd.hxx"
#include "../worldobj_data.hxx"

namespace Pingus {
namespace WorldObjsData {

class SpikeData : public WorldObjData
{
public:
  CL_Sprite  surface;
  Vector   pos;
  GameCounter counter;

public:
  SpikeData ();
  SpikeData (xmlDocPtr doc, xmlNodePtr node);
  SpikeData (const SpikeData& old);

  void insert_WorldObjs (World*);

private:
  SpikeData& operator= (const SpikeData&);
};

} // namespace WorldObjsData
} // namespace Pingus {

#endif

/* EOF */
