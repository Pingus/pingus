//  $Id: entrance_window.cxx,v 1.5 2002/09/27 11:26:45 torangan Exp $
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

#include "../editorobjs/entrance_obj.hxx"
#include "editor.hxx"
#include "object_manager.hxx"
#include "../string_converter.hxx"
#include "entrance_window.hxx"

/**********************************
 Entrance Properties
 ----------------------------------
 Direction:       [left|right|none]
 Release Rate:                   30
 OwnerId:     [player1|player2|...]
 ----------------------------------
 [ok] [cancel]
***********************************/

namespace EditorNS {

EntranceWindow::EntranceWindow (CL_Component* parent, EditorObjs::EntranceObj* obj)
  : PropertyFrame(200, 100, parent),
    entrance(obj),
    direction_label(CL_Rect(10, 10, 90, 30), "Direction", this),
    direction_left(CL_Point(110, 10), "left", this),
    direction_misc(CL_Point(110, 30), "misc", this),
    direction_right(CL_Point(110, 50), "right", this),
    release_rate_label(CL_Rect(10, 70, 90,90), "Release Rate", this),
    release_rate_input(CL_Rect(110, 70, 190, 90), "30", this)
{
  direction_group.add(&direction_left);
  direction_group.add(&direction_misc);
  direction_group.add(&direction_right);

  switch (entrance->direction)
    {
    case WorldObjsData::EntranceData::LEFT:
      direction_left.set_checked(true);
      break;
    case WorldObjsData::EntranceData::MISC:
      direction_misc.set_checked(true);
      break;
    case WorldObjsData::EntranceData::RIGHT:
      direction_right.set_checked(true);
      break;
    }

  release_rate_input.set_text(to_string(entrance->release_rate));
}

EntranceWindow::~EntranceWindow ()
{
  if (EditorObj::get_editor()->get_object_manager()->has_object(entrance))
    {
      if (direction_left.is_checked())
	entrance->direction = WorldObjsData::EntranceData::LEFT;
      else if (direction_misc.is_checked())
	entrance->direction = WorldObjsData::EntranceData::MISC;
      else if (direction_right.is_checked())
	entrance->direction = WorldObjsData::EntranceData::RIGHT;

      from_string(release_rate_input.get_text(), entrance->release_rate);
    }
  else
    {
      std::cout << "EntranceWindow::~EntranceWindow (): object disapeared" << std::endl;
    }
}

} // namespace EditorNS

/* EOF */
