//  $Id: Teleporter.hh,v 1.6 2000/12/14 21:35:56 grumbel Exp $
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

#include "../Position.hh"
#include "../WorldObj.hh"
#include "../editor/EditorWorldObj.hh"
#include "../WorldObjData.hh"

class TeleporterData : public WorldObjData
{
public:
  Position pos;
  Position target_pos;
  
  TeleporterData () {}
  TeleporterData (const TeleporterData& data);
  virtual ~TeleporterData () {}

  /** Writte the content of this object formated as xml to the given
      stream */
  virtual void write_xml(ofstream* xml);
  ///
  static WorldObjData* create(xmlDocPtr doc, xmlNodePtr cur);
};

class Teleporter : private TeleporterData,
		   public WorldObj
{
private:
  CL_Surface sur;
  
public:
  ///
  Teleporter () {};
  ///
  Teleporter (WorldObjData* data);
  ///
  virtual ~Teleporter () {}
  ///
  virtual void draw_offset (int x_of, int y_of, float s = 1.0);
  ///
  virtual void let_move(void);
  ///
  virtual int  get_z_pos() const { return pos.z_pos; }
};

/** An pseudo object to represent the teleporter target, all the
    data itself is handled inside the EditorTeleporterObj, but we
    need this helper objects to be able to show and move the
    teleporter target inside the editor */
class EditorTeleporterTargetObj : public EditorObj
{
public:
  /// Basic constructor
  EditorTeleporterTargetObj ();
  EditorTeleporterTargetObj (WorldObjData* obj, Position*);

  virtual ~EditorTeleporterTargetObj () {}
    
  static std::list<EditorObj*> create (TeleporterData*);

  Position get_position () { return *position; }

  /// The saveing will be done in EditorTeleporterObj::save_xml
  virtual void save_xml (std::ofstream* xml);
  virtual std::string status_line();
};

class EditorTeleporterObj : public EditorWorldObj, 
			    public TeleporterData
{
private:
  EditorTeleporterTargetObj* target;

public:
  EditorTeleporterObj (WorldObjData* obj);
  virtual ~EditorTeleporterObj ();

  Position* get_target_pos_p () { return &target_pos; }

  static std::list<EditorObj*> create (WorldObjData* obj);

  /** Create this object (and child objects) with resonable defaults
      for the editor */
  static std::list<EditorObj*> create (const Position& pos);

  virtual void draw_offset(int, int);
  virtual void save_xml (std::ofstream* xml);
  virtual std::string status_line();
};

#endif

/* EOF */
