//  $Id: hammer.hxx,v 1.10 2003/10/18 23:17:28 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLDOBJS_HAMMER_HXX
#define HEADER_PINGUS_WORLDOBJS_HAMMER_HXX

#include "../worldobj.hxx"

namespace Pingus {

namespace WorldObjsData {
class HammerData;
} // namespace WorldObjsData

class Pingu;

namespace WorldObjs {

class Hammer : public WorldObj
{
private:
  WorldObjsData::HammerData* const data;
  CL_Sprite sprite;

public:
  Hammer (const WorldObjsData::HammerData& data_);
 ~Hammer ();

  float get_z_pos () const;

  void draw (SceneContext& gc);
  void update ();

private:
  Hammer (const Hammer&);
  Hammer& operator= (const Hammer&);
};

} // namespace WorldObjs
} // namespace Pingus

#endif

/* EOF */
