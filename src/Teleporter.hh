//  $Id: Teleporter.hh,v 1.1 2000/09/23 20:28:18 grumbel Exp $
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

#include "Position.hh"
#include "WorldObj.hh"
#include "WorldObjData.hh"

class TeleporterData : public WorldObjData
{
private:
  Position pos;
  Position target_pos;
  
public:
  TeleporterData () {}
  virtual ~TeleporterData () {}

  /** Writte the content of this object formated as xml to the given
      stream */
  virtual void write_xml(ofstream* xml);
  ///
  static WorldObjData* create(xmlDocPtr doc, xmlNodePtr cur);
};

class Teleporter : public TeleporterData,
		   public WorldObj
{
private:
  CL_Surface* sur;
  
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
};

#endif

/* EOF */
