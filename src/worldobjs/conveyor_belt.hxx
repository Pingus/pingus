//  $Id: conveyor_belt.hxx,v 1.10 2002/09/14 19:06:34 torangan Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_WORLDOBJS_CONVEYOR_BELT_HXX
#define HEADER_PINGUS_WORLDOBJS_CONVEYOR_BELT_HXX

#include "../worldobj.hxx"
#include "../worldobj_data.hxx"
#include "../editor/rect_editorobj.hxx"

namespace WorldObjsData {
class ConveyorBeltData;
}

namespace WorldObjs {

class ConveyorBelt : public  WorldObj
{
private:
  WorldObjsData::ConveyorBeltData* const data;
  
public:
  ConveyorBelt (WorldObjsData::ConveyorBeltData* data_);
  
  void draw (GraphicContext& gc);
  void draw_colmap ();
  void update (float delta);
  float get_z_pos () const;
  
private:
  ConveyorBelt (const ConveyorBelt&);
  ConveyorBelt operator= (const ConveyorBelt&);
};

} // namespace WorldObjs

#endif

/* EOF */
