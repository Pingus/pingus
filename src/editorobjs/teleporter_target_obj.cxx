//  $Id: teleporter_target_obj.cxx,v 1.6 2002/10/12 00:49:10 torangan Exp $
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

#include <stdio.h>
#include "../vector.hxx"
#include "teleporter_obj.hxx"
#include "teleporter_target_obj.hxx"

namespace EditorObjs {

TeleporterTargetObj::TeleporterTargetObj (TeleporterObj* arg_teleporter)
  : SpriteEditorObj ("teleporter2", 
		     "worldobjs",
		     &arg_teleporter->get_target_pos_ref()),
    teleporter(arg_teleporter)
{
  sprite.set_align_center();
}

EditorObj* 
TeleporterTargetObj::duplicate ()
{
  return teleporter->duplicate();
}

std::string
TeleporterTargetObj::status_line()
{
  char str[128];
  snprintf(str, 128, "TeleporterTarget - %f %f %f", pos_ref->x, pos_ref->y, pos_ref->z);
  return str;
}

} // namespace EditorObjs

/* EOF */
