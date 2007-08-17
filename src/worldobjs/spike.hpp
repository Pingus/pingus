//  $Id$
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

#ifndef HEADER_PINGUS_TRAPS_SPIKE_HXX
#define HEADER_PINGUS_TRAPS_SPIKE_HXX

#include "../worldobj.hpp"


namespace WorldObjsData {
class SpikeData;
} // namespace WorldObjsData

class Pingu;

namespace WorldObjs {

class Spike : public WorldObj
{
private:
  Sprite   surface;
  Vector3f      pos;
  bool        killing;

public:
  Spike(const FileReader& reader);

  float get_z_pos () const;

  void draw (SceneContext& gc);
  void update ();

protected:
  void catch_pingu (Pingu*);

private:
  Spike (const Spike&);
  Spike& operator= (const Spike&);
};

} // namespace WorldObjs

#endif

/* EOF */
