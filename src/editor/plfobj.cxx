//  $Id: plfobj.cxx,v 1.13 2002/09/25 17:21:38 torangan Exp $
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
#include "../pingus_error.hxx"
#include "editor_view.hxx"
#include "plfobj.hxx"
#include "entrance_window.hxx"

using namespace std;

EntranceObj::EntranceObj(const EntranceData& data)
  : EntranceData (data)
{
  pos_ref = &pos;

  std::cout << "EntranceObj::EntranceObj(const EntranceData& data): " 
	    << type << std::endl;

  if (type == "generic")
    {
      sprite = Sprite("Entrances/generic", "entrances");
      sprite.set_align_center_bottom ();
    } 
  else if (type == "woodthing") 
    {
      std::cout << "WOODTHING" << std::endl;
      sprite = Sprite("Entrances/woodthing_mov", "entrances");
      sprite.set_align(0  - sprite.get_width()/2,
		       32 - sprite.get_height());
    }
  else if (type == "cloud")
    {
      sprite = Sprite("Entrances/cloud", "entrances");
      sprite.set_align(-115, -75);
    } 
  else 
    {
      std::cout << "Entrance obj error!" << endl;
      PingusError::raise("EntranceObj: Unknown entrance type: " + type);
    }
}

EditorObj*
EntranceObj::duplicate()
{
  std::cout << "EntranceObj::duplicate()" << std::endl;
  return new EntranceObj(static_cast<EntranceData>(*this));
}

std::string
EntranceObj::status_line()
{
  std::string dir_str = "not set - this is a bug";
  char str[256];

  switch(direction)
    {
    case EntranceData::LEFT:
      dir_str = "left";
      break;
    case EntranceData::RIGHT:
      dir_str = "right";
      break;
    case EntranceData::MISC:
      dir_str = "misc";
      break;
    }

  snprintf(str, 256, "Entrance: %s Rate: %d Direction: %s Owner: %d",
	  type.c_str(), release_rate, dir_str.c_str(), owner_id);

  return std::string(str);
}

EditorNS::PropertyFrame*
EntranceObj::get_gui_dialog (CL_Component* parent)
{
  // FIXME: due to the shared_ptr<> this is not guaranted to stay here
  // FIXME: long enough
  return new EditorNS::EntranceWindow (parent, this);
}

ExitObj::ExitObj(const ExitData& data)
  : SpriteEditorObj (data.desc.res_name, data.desc.datafile, &pos),
    ExitData (data)
{
  sprite.set_align_center_bottom ();
         
  if (use_old_pos_handling)
    {
      pos.x += sprite.get_width ()/2;
      pos.y += sprite.get_height ();
      use_old_pos_handling = false;
    }
}

EditorObj*
ExitObj::duplicate()
{
  // FIXME: The static_cast<> looks ugly.. 
  return new ExitObj(static_cast<ExitData>(*this));
}

std::string 
ExitObj::status_line()
{
  char str[256];
  
  snprintf(str, 256, "Exit - %s - X:%4.2f Y:%4.2f Z:%4.2f OwnerId: %d", desc.res_name.c_str(), pos.x, pos.y, pos.z, owner_id);

  return str;
}

/* EOF */
