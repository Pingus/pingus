//  $Id: editor_groundpiece_obj.cxx,v 1.2 2002/06/13 14:25:13 torangan Exp $
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

#include "../boost/smart_ptr.hpp"

#include "editor_groundpiece_obj.hxx"

EditorGroundpieceObj::EditorGroundpieceObj(const GroundpieceData& data)
  : GroundpieceData (data),
    SpriteEditorObj (desc.res_name, desc.datafile, pos)
{
}

boost::shared_ptr<EditorObj>   
EditorGroundpieceObj::duplicate()
{
  return boost::shared_ptr<EditorObj>(new EditorGroundpieceObj
				      (static_cast<GroundpieceData>(*this)));
}

std::string 
EditorGroundpieceObj::status_line()
{
  // FIXME: This is a potential buffer overrun, to lazy to fix it right now
  char str[1024];  
  std::string type_name = GroundpieceData::type_to_string(gptype);
  
  sprintf (str, "Groundpiece: %s Type: %s",
	   desc.res_name.c_str(), type_name.c_str());

  return std::string(str);
}

/* EOF */
