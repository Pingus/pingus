//  $Id: surface_background_data.hxx,v 1.7 2002/07/02 13:36:06 torangan Exp $
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

#ifndef HEADER_PINGUS_BACKGROUNDS_SURFACE_BACKGROUND_DATA_HXX
#define HEADER_PINGUS_BACKGROUNDS_SURFACE_BACKGROUND_DATA_HXX

#include "../string_converter.hxx"
#include "../editor/sprite_editorobj.hxx"
#include "../worldobj_data.hxx"
#include "../color.hxx"
#include "../res_descriptor.hxx"

#include "../libxmlfwd.hxx"

class SurfaceBackground;

class SurfaceBackgroundData : public WorldObjData
{
public:
  ResDescriptor desc;
  float para_x;
  float para_y;

  /** Position of the background, only the z_pos is really used. */
  CL_Vector pos;

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

  /// Init all fields with some usefull defaults values.
  SurfaceBackgroundData();
  
  /** Parse the xml snip and return a newly allocated
      SurfaceBackgroundData*, the user is responsible to delete the
      object */
  SurfaceBackgroundData(xmlDocPtr doc, xmlNodePtr cur);

  /// Empty destructor
  virtual ~SurfaceBackgroundData();

  /** Write the content of this object formated as xml to the given
      stream */
  void write_xml(std::ostream& xml);
  
  WorldObj* create_WorldObj();
  EditorObjLst create_EditorObj();
};


// FIXME: this should get into its own file
class EditorSurfaceBackground : public SurfaceBackgroundData,
				public SpriteEditorObj
{
private:
public:
  EditorSurfaceBackground (const SurfaceBackgroundData& data)
    : SurfaceBackgroundData (data),
      SpriteEditorObj (desc.res_name, desc.datafile, pos)
  {
    
  }

  void write_xml(std::ostream& xml) { this->SurfaceBackgroundData::write_xml (xml); }

  EditorObj* duplicate() {
    return new EditorSurfaceBackground (static_cast<SurfaceBackgroundData>(*this));
  }

  std::string status_line () { return "SurfaceBackground: " 
				 + to_string (pos.x) + ", "
				 + to_string (pos.y) + ", "
				 + to_string (pos.z); }
};

#endif

/* EOF */

