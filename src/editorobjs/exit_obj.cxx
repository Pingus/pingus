//  $Id: exit_obj.cxx,v 1.2 2003/04/19 10:23:18 torangan Exp $
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
#include "../worldobjsdata/exit_data.hxx"
#include "exit_obj.hxx"

namespace EditorObjs {

ExitObj::ExitObj (const WorldObjsData::ExitData& data_)
  : SpriteEditorObj(data_.desc.res_name, data_.desc.datafile),
    data(new WorldObjsData::ExitData(data_))
{
  pos_ref = &data->pos;
  sprite.set_align_center_bottom();

  if (data->use_old_pos_handling)
    {
      data->pos.x += sprite.get_width ()/2;
      data->pos.y += sprite.get_height();
      data->use_old_pos_handling = false;
    }
}

ExitObj::~ExitObj ()
{
  delete data;
}

EditorObj*
ExitObj::duplicate ()
{
  return new ExitObj(*data);
}

void
ExitObj::write_xml (std::ostream& xml)
{
  data->write_xml(xml);
}

std::string
ExitObj::status_line ()
{
  char str[256];

  snprintf(str, 256, "Exit - %s - X:%4.2f Y:%4.2f Z:%4.2f OwnerId: %d",
           data->desc.res_name.c_str(), data->pos.x, data->pos.y, data->pos.z, data->owner_id);

  return str;
}

} // namespace EditorObjs

/* EOF */
