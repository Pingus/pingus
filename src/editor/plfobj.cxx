//  $Id: plfobj.cxx,v 1.14 2002/09/27 11:26:45 torangan Exp $
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
#include "plfobj.hxx"

ExitObj::ExitObj (const ExitData& data)
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
