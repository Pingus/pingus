//  $Id: InfoBox.hh,v 1.2 2001/12/02 21:43:48 grumbel Exp $
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

#ifndef INFOBOX_HH
#define INFOBOX_HH

#include <ClanLib/core.h>
#include "../WorldObj.hh"
#include "../WorldObjData.hh"
#include "../editor/SpriteEditorObj.hh"

class InfoBoxData : public WorldObjData
{
public:
  InfoBoxData ();
  ~InfoBoxData ();

  std::string info_text;
  CL_Vector pos;
  CL_Vector text_pos;

  void write_xml(ofstream* xml);  
  boost::shared_ptr<WorldObj> create_WorldObj();
  EditorObjLst create_EditorObj();
  static boost::shared_ptr<WorldObjData> create(xmlDocPtr doc, xmlNodePtr cur);
};

class InfoBox : public InfoBoxData,
		public WorldObj		
{
private:
  Sprite sprite;
  CL_Font* font;
  bool is_open;

public:
  InfoBox (const InfoBoxData& data);

  void draw_offset (int x, int y, float s = 1.0);
  void update (float delta);
  float get_z_pos() const { return pos.z; }
};

class EditorInfoBox : public InfoBoxData,
			public SpriteEditorObj
{
public:
  EditorInfoBox (const InfoBoxData& data);

  static std::list<boost::shared_ptr<EditorObj> > create (const CL_Vector& pos);

  void write_xml(ofstream* xml) { InfoBoxData::write_xml (xml); }
  boost::shared_ptr<EditorObj> duplicate();
  std::string status_line ();
};

#endif

/* EOF */
