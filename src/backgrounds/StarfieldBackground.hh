//  $Id: StarfieldBackground.hh,v 1.16 2002/06/08 23:11:08 torangan Exp $
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

#ifndef STARFIELDBACKGROUND_HH
#define STARFIELDBACKGROUND_HH

#include "../StringConverter.hh"
#include "../WorldObj.hh"
#include "../WorldObjData.hh"

class _xmlDoc;  typedef _xmlDoc*  xmlDocPtr;
class _xmlNode; typedef _xmlNode* xmlNodePtr;

class StarfieldBackgroundData : public WorldObjData
{
public:
  int small_stars_count;
  int middle_stars_count;
  int large_stars_count;

  StarfieldBackgroundData ();
  virtual ~StarfieldBackgroundData () {}

  void write_xml(std::ofstream* xml);
  static boost::shared_ptr<WorldObjData> create(xmlDocPtr doc, xmlNodePtr cur);

  boost::shared_ptr<WorldObj> create_WorldObj();
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
  ///static boost::shared_ptr<StarfieldBackground> create (boost::shared_ptr<BackgroundData> arg_data);

  virtual ~StarfieldBackground ();

  // FIXME: Make z_pos handling editable via xml
  float get_z_pos() const { return -10; }

  void update(float delta);
  void draw_offset(int x_of, int y_of, float s = 1.0);
};

class EditorStarfieldBackground : public StarfieldBackgroundData,
				  public SpriteEditorObj
{
private:
  CL_Vector pos;
public:
  EditorStarfieldBackground (const StarfieldBackgroundData& data)
    : StarfieldBackgroundData (data),
      SpriteEditorObj ("Stars/starfield_icon", "game", pos),
      pos (-64.0f, 0.0f)
  {}

  void write_xml(std::ofstream* xml) { this->StarfieldBackgroundData::write_xml (xml); }

  boost::shared_ptr<EditorObj> duplicate() {
    return boost::shared_ptr<EditorObj>
      (new EditorStarfieldBackground (static_cast<StarfieldBackgroundData>(*this)));
  }

  std::string status_line () { return "StarfieldBackground: " 
				 + to_string (pos.x) + ", "
				 + to_string (pos.y) + ", "
				 + to_string (pos.z); }
};

#endif

/* EOF */
