//  $Id: Teleporter.hh,v 1.25 2002/06/01 18:05:37 torangan Exp $
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

#ifndef TELEPORTER_HH
#define TELEPORTER_HH

#include <fstream>

#include "../boost/smart_ptr.hpp"
#include "../Sprite.hh"
#include "../WorldObj.hh"
#include "../WorldObjData.hh"
#include "../editor/SpriteEditorObj.hh"

class EditorTeleporterObj;

class TeleporterData : public WorldObjData
{
public:
  CL_Vector pos;
  CL_Vector target_pos;
  
  TeleporterData () {}
  TeleporterData (const TeleporterData& data);
  
  /** Write the content of this object formatted as xml to the given
      stream */
  void write_xml(std::ofstream* xml);
  
  static boost::shared_ptr<WorldObjData> create(xmlDocPtr doc, xmlNodePtr cur);

  /** Create an WorldObj from the given data object */
  boost::shared_ptr<WorldObj> create_WorldObj ();

  /** Create an EditorObj from the given data object */
  std::list<boost::shared_ptr<EditorObj> > create_EditorObj ();
};

class Teleporter : private TeleporterData,
		   public WorldObj
{
private:
  Sprite sprite;
  Sprite target_sprite;
  
public:
  Teleporter (const TeleporterData& data);

  int get_z_pos() { return 0; }  
  void draw_offset (int x, int y, float s = 1.0);
  void update(float delta);
  float get_z_pos() const { return (int) pos.z; }
};

class EditorTeleporterTargetObj;

class EditorTeleporterObj : public SpriteEditorObj, 
			    public TeleporterData
{
private:
  EditorTeleporterTargetObj* target;

public:
  EditorTeleporterObj (const TeleporterData& data);
  
  CL_Vector& get_target_pos_ref () { return target_pos; }

  boost::shared_ptr<EditorObj> duplicate();
  static std::list<boost::shared_ptr<EditorObj> > create (const TeleporterData& data);
  
  void write_xml(std::ofstream* xml) { TeleporterData::write_xml(xml); }

  /** Create this object (and child objects) with reasonable defaults
      for the editor */
  static std::list<boost::shared_ptr<EditorObj> > create (const CL_Vector& pos);

  void draw (boost::dummy_ptr<EditorView> view);
  void save_xml (std::ofstream* xml);
  std::string status_line();
};

/** A pseudo object to represent the teleporter target; all the
    data itself is handled inside the EditorTeleporterObj, but we
    need this helper object to be able to show and move the
    teleporter target inside the editor */
class EditorTeleporterTargetObj : public SpriteEditorObj
{
private:
  EditorTeleporterObj* teleporter;
  
public:
  /// Basic constructor
  EditorTeleporterTargetObj (EditorTeleporterObj* obj);
  virtual ~EditorTeleporterTargetObj () {}

  boost::shared_ptr<EditorObj> duplicate() { return teleporter->duplicate (); }

  /// The saving will be done in EditorTeleporterObj::save_xml
  void write_xml (std::ofstream* /*xml*/) {}
  std::string status_line();
};

#endif

/* EOF */
