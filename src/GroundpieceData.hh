//  $Id: GroundpieceData.hh,v 1.15 2002/06/10 11:00:25 torangan Exp $
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

#ifndef GROUNDPIECEDATA_HH
#define GROUNDPIECEDATA_HH

#include <ClanLib/Core/Math/cl_vector.h>
#include <ClanLib/Display/Display/surface.h>

#include "ResDescriptor.hh"

class EditorObj;

namespace boost {
  template <class T> class shared_ptr;
}

///
class GroundpieceData 
{
public:
  CL_Surface surface;
  CL_Resource* resource;
  ResDescriptor desc;
  CL_Vector pos;

  enum GPType { GP_SOLID, 
	      GP_TRANSPARENT,
	      GP_GROUND,
	      GP_BRIDGE, 
	      GP_WATER,
	      GP_LAVA,
	      GP_REMOVE, 
	      GP_NOTHING } gptype; 

  /********************/
  /* Static Functions */
  /********************/

  std::list<boost::shared_ptr<EditorObj> > create_EditorObj();
  void write_xml(std::ofstream* xml);

  static GPType string_to_type(const std::string& arg_type);
  static std::string type_to_string(GPType arg_type);
};

/* EOF */

#endif

/* EOF */




