//  $Id: selection.cxx,v 1.18 2003/03/25 23:15:23 grumbel Exp $
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

#include <algorithm>
#include "../vector.hxx"
#include <ClanLib/Display/Input/keyboard.h>
#include <ClanLib/Display/Input/key.h>
#include "selection.hxx"
#include "../math.hxx"

namespace EditorNS {

void
Selection::move(float x, float y) 
{
  for (std::vector<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->set_position_offset(Vector(x, y));
}

void
Selection::move(const Vector& pos) 
{
  for (std::vector<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->set_position_offset(pos);
}

void
Selection::drag() 
{
  for (std::vector<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->drag();
}

void
Selection::drop() 
{
  for (std::vector<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->drop();
}

void
Selection::add(EditorObj* obj)
{
  if (!has_object(obj))
    obj_list.push_back(obj);
}

void
Selection::add(std::vector<EditorObj*> objs)
{
  for (std::vector<EditorObj*>::iterator it = objs.begin(); it != objs.end(); ++it)
    add(*it);
}

bool
Selection::has_object(EditorObj* obj)
{
  return std::find(obj_list.begin(), obj_list.end(), obj)  !=  obj_list.end();
}

void
Selection::remove(EditorObj* obj)
{
  obj_list.erase(std::find(obj_list.begin(), obj_list.end(), obj));
}

void
Selection::raise() 
{
  for (std::vector<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    object_manager->raise_obj(*it);
}

void
Selection::lower() 
{
  for (std::vector<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    object_manager->lower_obj(*it);
}

void
Selection::select_rect(float x1_, float y1_, float x2_, float y2_) 
{
  int x1, x2, y1, y2;
  
  if (!CL_Keyboard::get_keycode(CL_KEY_LSHIFT)
      && !CL_Keyboard::get_keycode(CL_KEY_RSHIFT))
    clear();
    
  // Rotate the rectangle, so that x1,y1 is top/left und x2,y2 is
  // bottom right
  x1 = static_cast<int> (Math::min(x1_, x2_));
  x2 = static_cast<int> (Math::max(x1_, x2_));
  y1 = static_cast<int> (Math::min(y1_, y2_));
  y2 = static_cast<int> (Math::max(y1_, y2_));

  add(object_manager->rect_get_objs(x1, y1, x2, y2));
}

EditorObj*
Selection::get_current_obj() 
{
  if (obj_list.size() == 1)
    return *obj_list.begin();
  else
    return 0;
}

void
Selection::horizontal_flip() 
{
  for (std::vector<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->horizontal_flip();
}

void
Selection::vertical_flip() 
{
  for (std::vector<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->vertical_flip();
}

void
Selection::rotate_90() 
{
  for (std::vector<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->rotate_90();
}

void
Selection::rotate_270()
{
  for (std::vector<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->rotate_270();
}

void
Selection::draw(EditorView * view)
{
  
  for (std::vector<EditorObj*>::iterator i = obj_list.begin(); i != obj_list.end(); ++i) 
    {
      (*i)->draw_mark(view);
    }
}

} // namespace EditorNS

/* EOF */
