//  $Id: selection.cxx,v 1.13 2002/09/11 12:45:58 grumbel Exp $
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
#include <ClanLib/Core/Math/cl_vector.h>
#include <ClanLib/Display/Input/keyboard.h>
#include <ClanLib/Display/Input/key.h>
#include "selection.hxx"
#include "../math.hxx"

using namespace std;

namespace EditorNS {

void
EditorSelection::move(float x, float y) 
{
  for (vector<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->set_position_offset(CL_Vector(x, y));
}

void
EditorSelection::move(const CL_Vector& pos) 
{
  for (vector<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->set_position_offset(pos);
}

void
EditorSelection::drag() 
{
  for (vector<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->drag();
}

void EditorSelection::drop() 
{
  for (vector<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->drop();
}

void EditorSelection::add(EditorObj* obj)
{
  obj_list.push_back(obj);
}

void
EditorSelection::add(vector<EditorObj*> objs)
{
  obj_list.insert(obj_list.end(), objs.begin(), objs.end());
}

void
EditorSelection::remove(EditorObj* obj)
{
  obj_list.erase(std::find(obj_list.begin(), obj_list.end(), obj));
}

void
EditorSelection::raise() 
{
  for (vector<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    object_manager->raise_obj(*it);
}

void
EditorSelection::lower() 
{
  for (vector<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    object_manager->lower_obj(*it);
}

void
EditorSelection::select_rect(float x1_, float y1_, float x2_, float y2_) 
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

  vector<EditorObj*> erg = object_manager->rect_get_objs(x1, y1, x2, y2);
  obj_list.insert(obj_list.end(), erg.begin(), erg.end());
}

bool
EditorSelection::object_selected(EditorObj* obj) 
{
  return std::find(obj_list.begin(), obj_list.end(), obj) != obj_list.end();
}

EditorObj*
EditorSelection::get_current_obj() 
{
  if (obj_list.size() == 1)
    return *obj_list.begin();
  else
    return 0;
}

void
EditorSelection::horizontal_flip() 
{
  for (vector<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->horizontal_flip();
}

void
EditorSelection::vertical_flip() 
{
  for (vector<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->vertical_flip();
}

void
EditorSelection::rotate_90() 
{
  for (vector<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->rotate_90();
}

void
EditorSelection::rotate_270()
{
  for (vector<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->rotate_270();
}

void
EditorSelection::draw(EditorView * view)
{
  
  for (vector<EditorObj*>::iterator i = obj_list.begin(); i != obj_list.end(); ++i) 
    {
      (*i)->draw_mark(view);
    }
}

} // namespace EditorNS

/* EOF */
