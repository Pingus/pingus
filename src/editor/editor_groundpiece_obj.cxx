//  $Id: editor_groundpiece_obj.cxx,v 1.10 2002/09/04 14:55:12 torangan Exp $
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
#include "../pingus_resource.hxx"
#include "groundpiece_window.hxx"

using namespace Pingus;

EditorGroundpieceObj::EditorGroundpieceObj(const GroundpieceData& data)
  : GroundpieceData (data),
    SpriteEditorObj (desc, pos)
{
}

EditorObj*
EditorGroundpieceObj::duplicate()
{
  return new EditorGroundpieceObj (static_cast<GroundpieceData>(*this));
}

std::string 
EditorGroundpieceObj::status_line()
{
  char str[1024];  
  std::string type_name = GroundpieceData::type_to_string(gptype);
  
  snprintf (str, 1024, "Groundpiece: %s Type: %s",
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

  pos.x += sprite.get_width ()/2;
  pos.y += sprite.get_height ()/2;

  desc.modifier = Pingus::rotate_90(desc.modifier);
  sprite.get_surface () = PingusResource::load_surface (desc);

  pos.x -= sprite.get_width ()/2;
  pos.y -= sprite.get_height ()/2;
}

void
EditorGroundpieceObj::rotate_270 ()
{
  std::cout << "rot 270" << std::endl;

  pos.x += sprite.get_width ()/2;
  pos.y += sprite.get_height ()/2;

  desc.modifier = Pingus::rotate_270(desc.modifier);
  sprite.get_surface () = PingusResource::load_surface (desc);

  pos.x -= sprite.get_width ()/2;
  pos.y -= sprite.get_height ()/2;
}

Pingus::Editor::PropertyFrame*
EditorGroundpieceObj::get_gui_dialog(CL_Component* parent)
{
  return new Pingus::Editor::GroundpieceWindow (parent, this);
}

/* EOF */
