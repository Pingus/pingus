//  $Id: hammer.hxx,v 1.1 2002/09/04 14:55:13 torangan Exp $
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

namespace WorldObjsData {
  class HammerData;
}

class Pingu;


namespace WorldObjs {

  class Hammer : public WorldObj
  {
  private:
    bool particle_thrown;
    WorldObjsData::HammerData* const data;
    
  public:
    Hammer (WorldObjsData::HammerData* data_);
   ~Hammer ();

    float get_z_pos () const;

    void draw_offset (int x, int y, float s);
    void update (float delta);

  protected:
    void catch_pingu (Pingu*);

  private:
    Hammer (const Hammer&);
    Hammer operator= (const Hammer&);
  };

}

#endif

/* EOF */
