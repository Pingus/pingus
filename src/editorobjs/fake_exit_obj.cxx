//  $Id: fake_exit_obj.cxx,v 1.13 2003/10/19 12:25:47 grumbel Exp $
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
#include "fake_exit_obj.hxx"
#include "../editor/editor_view.hxx"
#include "../worldobjsdata/fake_exit_data.hxx"

namespace Pingus {
namespace EditorObjs {

FakeExitObj::FakeExitObj (const WorldObjsData::FakeExitData& data_)
  : data(new WorldObjsData::FakeExitData(data_)),
    frame(0)
{
  data->pos.z = -100;
  sprite = Sprite("Traps/fake_exit", "traps");
  sprite.set_align_center_bottom ();

  pos_ref = &data->pos;
}

FakeExitObj::~FakeExitObj ()
{
  delete data;
}

EditorNS::EditorObjLst
FakeExitObj::create (const Vector& pos)
{
  WorldObjsData::FakeExitData newdata;
  newdata.pos = pos;
  return EditorNS::EditorObjLst(1, new FakeExitObj (newdata));
}

EditorNS::EditorObj*
FakeExitObj::duplicate ()
{
  return new FakeExitObj(*data);
}

void
FakeExitObj::write_xml (std::ostream& xml)
{
  data->write_xml(xml);
}

void
FakeExitObj::draw (EditorNS::EditorView* view)
{
  view->draw(sprite, data->pos, frame);
}

std::string
FakeExitObj::status_line ()
{
  char str[64];
  snprintf (str, 64, "FakeExitObj: %4.2fx%4.2fx%4.2f", data->pos.x, data->pos.y, data->pos.z);
  return str;
}

} // namespace EditorObjs
} // namespace Pingus

/* EOF */
