//  $Id: exit.hxx,v 1.5 2003/10/18 23:17:28 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLDOBJS_EXIT_HXX
#define HEADER_PINGUS_WORLDOBJS_EXIT_HXX

#include "../res_descriptor.hxx"
#include "../worldobj.hxx"

namespace WorldObjsData {
class ExitData;
}

namespace WorldObjs {

class Exit : public WorldObj
{
private:
  Vector3f pos;
  int owner_id;

  Sprite sprite;
  Sprite flag;
  Sprite smallmap_symbol;

public:
  Exit(const FileReader& reader);
  ~Exit();

  void  on_startup ();

  void  draw (SceneContext& gc);
  void  draw_smallmap(SmallMap* smallmap);

  void  update ();

  float get_z_pos () const;

private:
  Exit (const Exit&);
  Exit& operator= (const Exit&);
};

} // namespace WorldObjs

#endif

/* EOF */
