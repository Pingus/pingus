//  $Id: laser_exit.hxx,v 1.8 2003/10/19 12:25:47 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLDOBJS_LASER_EXIT_HXX
#define HEADER_PINGUS_WORLDOBJS_LASER_EXIT_HXX

#include "../game_counter.hxx"
#include "../worldobj.hxx"

namespace Pingus {

namespace WorldObjsData {
class LaserExitData;
} // namespace WorldObjsData

class Pingu;

namespace WorldObjs {

class LaserExit : public WorldObj
{
private:
  CL_Sprite   surface;
  Vector      pos;
  GameCounter counter;

  bool killing;

public:
  LaserExit(const FileReader& reader);

  float get_z_pos () const;

  void draw (SceneContext& gc);
  void update ();

protected:
  void catch_pingu (Pingu*);

private:
  LaserExit (const LaserExit&);
  LaserExit& operator= (const LaserExit&);
};

} // namespace WorldObjs
} // namespace Pingus

#endif

/* EOF */
