//  $Id: Teleporter.hh,v 1.17 2001/08/09 12:04:49 grumbel Exp $
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

#include "../boost/smart_ptr.hpp"
#include "../WorldObj.hh"
#include "../editor/EditorWorldObj.hh"
#include "../WorldObjData.hh"

class EditorTeleporterObj;

class TeleporterData : public WorldObjData
{
public:
  CL_Vector pos;
  CL_Vector target_pos;
  
  TeleporterData () {}
  TeleporterData (const TeleporterData& data);
  virtual ~TeleporterData () {}

  /** Write the content of this object formatted as xml to the given
      stream */
  virtual void write_xml(std::ofstream* xml);
  ///
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
  CL_Surface sur;
  
public:
  Teleporter (const TeleporterData& data);
  virtual ~Teleporter () {}
  
  virtual void draw (boost::dummy_ptr<EditorView> view);
  virtual void update(float delta);
  virtual int  get_z_pos() const { return (int) pos.z; }
};

/** A pseudo object to represent the teleporter target; all the
    data itself is handled inside the EditorTeleporterObj, but we
    need this helper object to be able to show and move the
    teleporter target inside the editor */
class EditorTeleporterTargetObj : public EditorObj
{
public:
  /// Basic constructor
  EditorTeleporterTargetObj (EditorTeleporterObj* obj);
  virtual ~EditorTeleporterTargetObj () {}
    
  static std::list<boost::shared_ptr<EditorObj> > create (TeleporterData*);

  CL_Vector get_position () { return *position; }

  /// The saving will be done in EditorTeleporterObj::save_xml
  virtual void save_xml (std::ofstream* xml);
  virtual std::string status_line();
};

class EditorTeleporterObj : public EditorWorldObj, 
			    public TeleporterData
{
private:
  EditorTeleporterTargetObj* target;

public:
  EditorTeleporterObj (const TeleporterData& data);
  virtual ~EditorTeleporterObj ();

  CL_Vector* get_target_pos_p () { return &target_pos; }

  static std::list<boost::shared_ptr<EditorObj> > create (const TeleporterData& data);

  /** Create this object (and child objects) with reasonable defaults
      for the editor */
  static std::list<boost::shared_ptr<EditorObj> > create (const CL_Vector& pos);

  virtual void draw (boost::dummy_ptr<EditorView> view);
  virtual void save_xml (std::ofstream* xml);
  virtual std::string status_line();
};

#endif

/* EOF */
