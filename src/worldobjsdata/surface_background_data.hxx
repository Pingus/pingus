//  $Id: surface_background_data.hxx,v 1.3 2002/09/28 11:52:27 torangan Exp $
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

#ifndef HEADER_PINGUS_WORLDOBJSDATA_SURFACE_BACKGROUND_DATA_HXX
#define HEADER_PINGUS_WORLDOBJSDATA_SURFACE_BACKGROUND_DATA_HXX

#include "../vector.hxx"
#include "../worldobj_data.hxx"
#include "../color.hxx"
#include "../res_descriptor.hxx"
#include "../libxmlfwd.hxx"

namespace WorldObjsData {

class SurfaceBackgroundData : public WorldObjData
{
public:
  ResDescriptor desc;
  float para_x;
  float para_y;

  /** Position of the background, only the z_pos is really used. */
  Vector pos;

  /** The amount of pixel the background is scrolled each frame in x
      direction. */
  float scroll_x;

  /** The amount of pixel the background is scrolled each frame in x
      direction. */
  float scroll_y;

  /** fill_rect() components
      An fill_rect() can be drawn over the background, the following
      for components are passed to the fill_rect() call. */
  Color color;

  /// Stretch the background to the full screen size in x direction
  bool stretch_x;

  /// Stretch the background to the full screen size in x direction
  bool stretch_y;

public:
  /// Init all fields with some usefull defaults values.
  SurfaceBackgroundData ();
  
  /** Parse the xml snip and return a newly allocated
      SurfaceBackgroundData*, the user is responsible to delete the
      object */
  SurfaceBackgroundData (xmlDocPtr doc, xmlNodePtr cur);

  SurfaceBackgroundData (const SurfaceBackgroundData& old);
  
  /** Write the content of this object formated as xml to the given
      stream */
  void write_xml (std::ostream& xml);
  
  WorldObj* create_WorldObj ();
  EditorObjLst create_EditorObj ();

private:
  SurfaceBackgroundData& operator= (const SurfaceBackgroundData&);
};

} // namespace WorldObjsData

#endif

/* EOF */
