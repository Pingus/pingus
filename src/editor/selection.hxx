//  $Id: selection.hxx,v 1.1 2002/07/01 16:10:29 torangan Exp $
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

#include <list>
#include "editorobj.hxx"
#include "object_manager.hxx"

class CL_Vector;

class EditorSelection 
{
  private:
  
    std::list<EditorObj*> obj_list;
    ObjectManager*        object_manager;
    
  public:
  
    EditorSelection(ObjectManager* mng) : object_manager(mng) { }
    ~EditorSelection() { }
    
    void clear() { obj_list.clear(); }
    
    void move(float x, float y);
    void move(const CL_Vector& pos);
    void drag();
    void drop();
    
    void add(EditorObj* obj);
    void add(list<EditorObj*> objs);
    
    void remove(EditorObj* obj);
    
    void raise();
    void lower();
    
    void select_rect(float x1_, float y1_, float x2_, float y2_);
    
    bool object_selected(EditorObj* obj);
    
    EditorObj* get_current_obj();
    const list<EditorObj*> & get_current_objs() { return obj_list; }
    
    void horizontal_flip();
    void vertical_flip();
    void rotate_90();
    void rotate_270();
    
};

#endif

/* EOF */
