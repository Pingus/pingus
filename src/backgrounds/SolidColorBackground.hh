//  $Id: SolidColorBackground.hh,v 1.11 2002/01/29 20:43:18 grumbel Exp $
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

#ifndef SOLIDCOLORBACKGROUND_HH
#define SOLIDCOLORBACKGROUND_HH

#include "../StringConverter.hh"
#include "../editor/EditorView.hh"
#include "../boost/smart_ptr.hpp"
#include "../Color.hh"
#include "../WorldObj.hh"
#include "../WorldObjData.hh"
#include "../editor/SpriteEditorObj.hh"
#include "../editor/EditorObj.hh"

class SolidColorBackgroundData : public WorldObjData
{
public:
  Color color;

  // FIXME: Add z_pos handling here

  SolidColorBackgroundData() {}
  virtual ~SolidColorBackgroundData() {}

  /** Writte the content of this object formated as xml to the given
      stream */
  virtual void write_xml(std::ofstream* xml);
  static boost::shared_ptr<WorldObjData> create (xmlDocPtr doc, xmlNodePtr cur);

  boost::shared_ptr<WorldObj> create_WorldObj();
  EditorObjLst create_EditorObj();
};

class SolidColorBackground : public WorldObj,
			     private SolidColorBackgroundData
{
public:
  SolidColorBackground (const SolidColorBackgroundData&);
  virtual ~SolidColorBackground () {}

  // FIXME: Make z_position editable
  float get_z_pos() const { return -10; }  
  void update (float delta) {}
  void draw_offset (int x_of, int y_of, float s = 1.0);
};


class EditorSolidColorBackground : public SolidColorBackgroundData,
				   public RectEditorObj
{
private:
  CL_Vector pos;
public:
  EditorSolidColorBackground (const SolidColorBackgroundData& data)
    : SolidColorBackgroundData (data),
      pos (0.0f, 0.0f)
  {}
  void write_xml(std::ofstream* xml) { this->SolidColorBackgroundData::write_xml (xml); }

  /// Return the object width
  int get_width() { return 256; }
  /// Return the object height
  int get_height() { return 256; }

  boost::shared_ptr<EditorObj> duplicate() {
    return boost::shared_ptr<EditorObj>
      (new EditorSolidColorBackground (static_cast<SolidColorBackgroundData>(*this)));
  }

  float get_z_pos () { return pos.z; }

  CL_Vector get_upper_left_corner () { 
    return pos; 
  }

  void draw(boost::dummy_ptr<EditorView> view)
  {
    view->draw_fillrect ((int)pos.x, (int)pos.y, (int)pos.x + 256, (int)pos.y + 256, 
			 color.red, color.green, color.blue, color.alpha);
  }

  void set_position_offset(const CL_Vector& offset)
  {
    pos += offset;
  }  

  std::string status_line () { return "SolidColorBackground: " + to_string (pos); }
};

#endif

/* EOF */
