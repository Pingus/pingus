//  $Id: starfield_background.hxx,v 1.9 2002/09/10 14:33:07 grumbel Exp $
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

#ifndef HEADER_PINGUS_BACKGROUNDS_STARFIELD_BACKGROUND_HXX
#define HEADER_PINGUS_BACKGROUNDS_STARFIELD_BACKGROUND_HXX

#include <ClanLib/Core/Math/cl_vector.h>
#include "../string_converter.hxx"
#include "../worldobj.hxx"
#include "../worldobj_data.hxx"
#include "../editor/sprite_editorobj.hxx"
#include "../libxmlfwd.hxx"

class StarfieldBackgroundData : public WorldObjData
{
public:
  int small_stars_count;
  int middle_stars_count;
  int large_stars_count;

  StarfieldBackgroundData ();
  StarfieldBackgroundData (xmlDocPtr doc, xmlNodePtr cur);
  StarfieldBackgroundData (const StarfieldBackgroundData& old);
  StarfieldBackgroundData operator= (const StarfieldBackgroundData& old);
  
  virtual ~StarfieldBackgroundData () {}

  void write_xml(std::ostream& xml);

  WorldObj* create_WorldObj();
  EditorObjLst create_EditorObj();
  
};

class StarfieldBackgroundStars : public WorldObj
{
private:
  static CL_Surface small_star;
  static CL_Surface middle_star;
  static CL_Surface large_star;
  static bool is_init;

  CL_Surface sur;

  float x_pos;
  float y_pos;

  float x_add;
  float y_add;

public:
  enum Type { SMALL_STAR, MIDDLE_STAR, LARGE_STAR };

  StarfieldBackgroundStars () {}
  StarfieldBackgroundStars (Type type);
  
  StarfieldBackgroundStars (const StarfieldBackgroundStars& old);
  StarfieldBackgroundStars operator= (const StarfieldBackgroundStars& old);
  
  // Never used
  float get_z_pos() const { return -100; }

  void init ();
  void update(float delta);
  void draw_offset(int x_of, int y_of, float s = 1.0);  
};

class StarfieldBackground : public WorldObj,
			    private StarfieldBackgroundData
{
private:
  std::vector<StarfieldBackgroundStars> stars;

public:
  StarfieldBackground (const StarfieldBackgroundData&);
  ///

  virtual ~StarfieldBackground ();

  // FIXME: Make z_pos handling editable via xml
  float get_z_pos() const { return -10; }

  void update(float delta);
  void draw_offset(int x_of, int y_of, float s = 1.0);
  
private:
  StarfieldBackground (const StarfieldBackground&);
  StarfieldBackground operator= (const StarfieldBackground&);
};

class EditorStarfieldBackground : public StarfieldBackgroundData,
				  public SpriteEditorObj
{
private:
  CL_Vector pos;
  
public:
  EditorStarfieldBackground (const StarfieldBackgroundData& data)
    : StarfieldBackgroundData (data),
      SpriteEditorObj ("Stars/starfield_icon", "game"),
      pos (-64.0f, 0.0f)
  {
    pos_ref = &pos;
  }

  void write_xml(std::ostream& xml) { this->StarfieldBackgroundData::write_xml (xml); }

  EditorObj* duplicate() {
    return new EditorStarfieldBackground (static_cast<StarfieldBackgroundData>(*this));
  }

  std::string status_line () { return "StarfieldBackground: " 
				 + to_string (pos.x) + ", "
				 + to_string (pos.y) + ", "
				 + to_string (pos.z); }

private:
  EditorStarfieldBackground (const EditorStarfieldBackground&);
  EditorStarfieldBackground operator= (const EditorStarfieldBackground&);
};

#endif

/* EOF */

