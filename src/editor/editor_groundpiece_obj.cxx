//  $Id: editor_groundpiece_obj.cxx,v 1.4 2002/06/25 21:31:40 grumbel Exp $
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

#include <stdio.h>
#include "../boost/smart_ptr.hpp"
#include "../pingus_resource.hxx"
#include "editor_groundpiece_obj.hxx"

using namespace Pingus;

EditorGroundpieceObj::EditorGroundpieceObj(const GroundpieceData& data)
  : GroundpieceData (data),
    SpriteEditorObj (desc, pos)
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

void
EditorGroundpieceObj::vertical_flip ()
{
  std::cout << "Vertical flip" << std::endl;
  desc.modifier = Pingus::vertical_flip(desc.modifier);
  sprite.get_surface () = PingusResource::load_surface (desc);
}

void
EditorGroundpieceObj::horizontal_flip ()
{
  std::cout << "Horz flip" << std::endl;
  desc.modifier = Pingus::horizontal_flip(desc.modifier);
  sprite.get_surface () = PingusResource::load_surface (desc);
}

void
EditorGroundpieceObj::rotate_90 ()
{
  std::cout << "rot90" << std::endl;
  desc.modifier = Pingus::rotate_90(desc.modifier);
  sprite.get_surface () = PingusResource::load_surface (desc);
}

void
EditorGroundpieceObj::rotate_270 ()
{
  std::cout << "rot 270" << std::endl;
  desc.modifier = Pingus::rotate_270(desc.modifier);
  sprite.get_surface () = PingusResource::load_surface (desc);
}

/* EOF */
