//  $Id: selection.cxx,v 1.2 2002/07/01 16:31:40 grumbel Exp $
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

#include <ClanLib/Core/Math/cl_vector.h>
#include <ClanLib/Display/Input/keyboard.h>
#include <ClanLib/Display/Input/key.h>
#include "selection.hxx"
#include "../math.hxx"

using namespace Pingus;

void EditorSelection::move(float x, float y) {

  for (list<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->set_position_offset(CL_Vector(x, y));
}

void EditorSelection::move(const CL_Vector& pos) {

  for (list<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->set_position_offset(pos);
}

void EditorSelection::drag() {

  for (list<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->drag();
}

void EditorSelection::drop() {

  for (list<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->drop();
}

void EditorSelection::add(EditorObj* obj) {

  obj_list.push_back(obj);
}

void EditorSelection::add(list<EditorObj*> objs) {

  obj_list.insert(obj_list.end(), objs.begin(), objs.end());
}

void EditorSelection::remove(EditorObj* obj) {

  obj_list.erase(std::find(obj_list.begin(), obj_list.end(), obj));
}

void EditorSelection::raise() {

  for (list<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    object_manager->raise_obj(boost::shared_ptr<EditorObj>(*it));
}

void EditorSelection::lower() {

  for (list<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    object_manager->lower_obj(boost::shared_ptr<EditorObj>(*it));
}

void
EditorSelection::select_rect(float x1_, float y1_, float x2_, float y2_) 
{
  int x1, x2, y1, y2;
  
  if (!CL_Keyboard::get_keycode(CL_KEY_LSHIFT)
      && !CL_Keyboard::get_keycode(CL_KEY_RSHIFT))
    clear();
    
  x1 = static_cast<int> (Math::min(x1_, x2_));
  x2 = static_cast<int> (Math::max(x1_, x2_));
  y1 = static_cast<int> (Math::min(y1_, y2_));
  y2 = static_cast<int> (Math::max(y1_, y2_));
  
  const list<EditorObj*> & erg = object_manager->rect_get_objs(x1, x2, y1, y2);
  
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
  for (list<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->horizontal_flip();
}

void
EditorSelection::vertical_flip() 
{
  for (list<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->vertical_flip();
}

void
EditorSelection::rotate_90() 
{
  for (list<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->rotate_90();
}

void
EditorSelection::rotate_270()
{
  for (list<EditorObj*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
    (*it)->rotate_270();
}

void
EditorSelection::draw(EditorView * view)
{
  for (list<EditorObj*>::iterator i = obj_list.begin(); i != obj_list.end(); ++i) 
    {
      (*i)->draw_mark(view);
    }
}

/* EOF */
