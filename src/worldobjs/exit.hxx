//  $Id: exit.hxx,v 1.1 2002/09/27 16:01:55 torangan Exp $
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

#include "../worldobj.hxx"
#include "../sprite.hxx"

namespace WorldObjsData {
class ExitData;
}

namespace WorldObjs {

class Exit : public WorldObj
{
private:
  WorldObjsData::ExitData* const data;
  Sprite sprite;
  Sprite flag;

public:
  Exit (const WorldObjsData::ExitData& data_);
 ~Exit ();
  
  void  on_startup ();
  void  draw (GraphicContext& gc);
  
  void  update (float delta);
  
  float get_z_pos () const;
  
private:
  Exit (const Exit&);
  Exit& operator= (const Exit&);
};

} // namespace WorldObjs

#endif

/* EOF */
