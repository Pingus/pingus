// $Id: TrapData.hh,v 1.3 2000/12/16 23:11:20 grumbel Exp $
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

#include <string>
#include "boost/smart_ptr.hpp"
#include "WorldObjData.hh"
#include "Position.hh"

/// Structure holding all the data needed for a trap.
class TrapData : public WorldObjData
{
public:
  std::string type;
  Position pos;
  
  TrapData(){}
  virtual ~TrapData(){}
  void clean(){}

  void write_xml(ofstream* xml);

  static boost::shared_ptr<WorldObjData> create(xmlDocPtr doc, xmlNodePtr cur);
};

#endif

/* EOF */
