//  $Id: level_resizer.cxx,v 1.4 2002/12/05 20:38:10 torangan Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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
#include <ClanLib/Display/Font/font.h>
#include "../fonts.hxx"
#include "object_manager.hxx"
#include "level_resizer.hxx"

namespace EditorNS {

LevelResizer::LevelResizer(ObjectManager* m)
  : SpriteEditorObj("cursors/scroll_cur", "core", &pos), obj_manager (m), dragged(false)
{
  sprite.set_align_center();

  pos.x = obj_manager->get_width();
  pos.y = obj_manager->get_height();
}

void
LevelResizer::update(float delta)
{
  pos.x = obj_manager->get_width();
  pos.y = obj_manager->get_height();
  UNUSED_ARG(delta);
}

void
LevelResizer::draw (EditorNS::EditorView* view)
{
  SpriteEditorObj::draw(view);

  if (dragged)
    {
      char str[64];
      snprintf(str, 64, "Level Size: %4dx%4d", obj_manager->get_width(), obj_manager->get_height());
      Fonts::pingus_small->print_left( 300, 200, str);
    }
}

void
LevelResizer::set_position_offset(const Vector& offset)
{
  SpriteEditorObj::set_position_offset(offset);
  obj_manager->set_width(int(pos.x));
  obj_manager->set_height(int(pos.y));
}

void
LevelResizer::drop() 
{
  dragged = false; 

  // Round the position to a multiple of 10
  //pos.x = int(pos.x)/10*10;
  //pos.y = int(pos.y)/10*10;
}
    
} // namespace EditorNS

/* EOF */
