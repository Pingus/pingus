//  $Id: selection.hxx,v 1.14 2003/03/25 23:15:23 grumbel Exp $
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

#ifndef HEADER_PINGUS_EDITOR_SELECTION_HXX
#define HEADER_PINGUS_EDITOR_SELECTION_HXX

#include "object_manager.hxx"

class Vector;

namespace EditorNS {

class EditorView;

class Selection 
{
private:
  
  std::vector<EditorObj*> obj_list;
  ObjectManager*        object_manager;
    
public:
  
  Selection(ObjectManager* mng) : object_manager(mng) { }
  ~Selection() { }
    
  void clear() { obj_list.clear(); }
    
  void move(float x, float y);
  void move(const Vector& pos);
  void drag();
  void drop();

  /** @return true if the obj is already in the selection, false otherwise */
  bool has_object(EditorObj* obj);
    
  /** Add the given object to the selection */
  void add(EditorObj* obj);
  void add(std::vector<EditorObj*> objs);
    
  /** Remove the given object from the selection */
  void remove(EditorObj* obj);
    
  void raise();
  void lower();
    
  /** Coordinates must be given in world-co not screen-co */
  void select_rect(float x1_, float y1_, float x2_, float y2_);
    
  EditorObj* get_current_obj();
  const std::vector<EditorObj*> & get_objects() { return obj_list; }

  int size () { return obj_list.size (); }
    
  void horizontal_flip();
  void vertical_flip();
  void rotate_90();
  void rotate_270();

  void draw(EditorView* view);  
  
private:
  Selection (const Selection&);
  Selection& operator= (const Selection&);
};

} // namespace EditorNS

#endif

/* EOF */
