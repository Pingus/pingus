//  $Id: teleporter.hxx,v 1.9 2002/09/05 11:26:35 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLDOBJS_TELEPORTER_HXX
#define HEADER_PINGUS_WORLDOBJS_TELEPORTER_HXX

#include "../worldobj.hxx"
#include "../worldobj_data.hxx"
#include "../editor/sprite_editorobj.hxx"
#include "../libxmlfwd.hxx"

class EditorTeleporterObj;

class TeleporterData : public WorldObjData
{
public:
  CL_Vector pos;
  CL_Vector target_pos;
  
  TeleporterData () {}
  TeleporterData (xmlDocPtr doc, xmlNodePtr cur);

  TeleporterData (const TeleporterData& data);
  TeleporterData operator= (const TeleporterData& data);
  
  /** Write the content of this object formatted as xml to the given
      stream */
  void write_xml(std::ostream& xml);
  
  /** Create an WorldObj from the given data object */
  WorldObj* create_WorldObj ();

  /** Create an EditorObj from the given data object */
  EditorObjLst create_EditorObj ();
};

class Teleporter : private TeleporterData,
		   public WorldObj
{
private:
  Sprite sprite;
  Sprite target_sprite;
  
public:
  Teleporter (const TeleporterData& data);

  int   get_z_pos() { return 0; }  
  void  draw (GraphicContext& gc);
  void  update(float delta);
  float get_z_pos() const { return (int) pos.z; }
  
private:
  Teleporter (const Teleporter&);
  Teleporter operator= (const Teleporter&);
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

  EditorObj* duplicate();
  static EditorObjLst create (const TeleporterData& data);
  
  void write_xml(std::ostream& xml) { TeleporterData::write_xml(xml); }

  /** Create this object (and child objects) with reasonable defaults
      for the editor */
  static EditorObjLst create (const CL_Vector& pos);

  void draw (EditorView * view);
  void save_xml (std::ostream& xml);
  std::string status_line();
  
private:
  EditorTeleporterObj (const EditorTeleporterObj&);
  EditorTeleporterObj operator= (const EditorTeleporterObj&);
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

  EditorObj* duplicate() { return teleporter->duplicate (); }

  /// The saving will be done in EditorTeleporterObj::save_xml
  void write_xml (std::ostream& xml) { UNUSED_ARG(xml); }
  std::string status_line();
  
private:
  EditorTeleporterTargetObj (const EditorTeleporterTargetObj&);
  EditorTeleporterTargetObj operator= (const EditorTeleporterTargetObj&);
};

#endif

/* EOF */
