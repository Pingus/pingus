//  $Id: ExitData.hh,v 1.7 2001/12/04 12:18:49 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef EXIT_DATA_HH
#define EXIT_DATA_HH

#include <ClanLib/core.h>
#include "ResDescriptor.hh"
#include "WorldObjData.hh"

///
class ExitData : public WorldObjData
{
public:
  CL_Vector pos;
  ResDescriptor desc;
  int owner_id;
  
  /** Causes the entrance handling to fall back to the old position
      treatment, that means the position is treaten as the upper/left
      corner of the exit surface. The new handling treats the position
      of the bottom/center position of the surface, which is much more
      convenience. */
  bool use_old_pos_handling;

  ///
  ExitData() { clean (); }
  /// Reset the values to default
  void clean(void) 
  { 
    owner_id = 0; 
    use_old_pos_handling = true;
  }

  void write_xml(std::ofstream *);
  static boost::shared_ptr<WorldObjData> create(xmlDocPtr doc, xmlNodePtr cur);
  
  boost::shared_ptr<WorldObj> create_WorldObj();
  EditorObjLst create_EditorObj();
};

#endif

/* EOF */
