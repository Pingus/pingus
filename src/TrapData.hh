// $Id: TrapData.hh,v 1.12 2002/01/21 12:06:09 grumbel Exp $
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

#ifndef TRAP_DATA_HH
#define TRAP_DATA_HH

#include <config.h>
#include <string>
#include <iostream>

#include "boost/smart_ptr.hpp"
#include "WorldObjData.hh"

/// Structure holding all the data needed for a trap.
class TrapData : public WorldObjData
{
public:
  std::string type;
  CL_Vector pos;

  int x_target, y_target;
  
  TrapData(){}
  TrapData(const TrapData& data) 
    : WorldObjData(data),
      type (data.type),
      pos (data.pos) {}
  virtual ~TrapData(){}
  void clean(){}

  void write_xml(std::ofstream* xml);

  static boost::shared_ptr<WorldObjData> create(xmlDocPtr doc, xmlNodePtr cur);

  /** Create an WorldObj from the given data object */
  boost::shared_ptr<WorldObj> create_WorldObj ();

  /** Create an EditorObj from the given data object */
  std::list<boost::shared_ptr<EditorObj> > create_EditorObj ();
};

#endif

/* EOF */
