//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_PINGUS_WORLDOBJS_THUNDERSTORM_BACKGROUND_HPP
#define HEADER_PINGUS_PINGUS_WORLDOBJS_THUNDERSTORM_BACKGROUND_HPP

#include "math/vector3f.hpp"
#include "pingus/worldobj.hpp"

namespace WorldObjsData {
class ThunderstormBackgroundData;
}

namespace WorldObjs {

class ThunderstormBackground : public WorldObj
{
private:
  Vector3f    pos;
  Sprite clouds_sur;
  int x_pos;

public:
  ThunderstormBackground(const FileReader& reader);

  float get_z_pos () const;

  void update ();
  void draw (SceneContext& gc);

private:
  ThunderstormBackground (const ThunderstormBackground&);
  ThunderstormBackground& operator= (const ThunderstormBackground&);
};

} // namespace WorldObjs

#endif

/* EOF */
