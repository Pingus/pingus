//  $Id: EntranceData.hh,v 1.3 2001/04/10 19:42:57 grumbel Exp $
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

#ifndef ENTRANCE_DATA_HH
#define ENTRANCE_DATA_HH

#include <string>

#include "Position.hh"
#include "ResDescriptor.hh"

///
class EntranceData 
{
public:
  enum EntranceDirection { LEFT, RIGHT, MISC };
  EntranceDirection direction;
  ResDescriptor desc;
  CL_Vector pos;
  int  release_rate;

  /// The type of the entrance type (woodthing, generic, etc.)
  std::string type;

  EntranceData() {
    clean();
  }
  
  void clean(void) {
    type = "generic";
    direction = MISC;
    release_rate = 50;
  }
};

#endif

/* EOF */
