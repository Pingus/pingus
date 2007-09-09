//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_EDITOR_OBJECT_PROPERTIES_HPP
#define HEADER_EDITOR_OBJECT_PROPERTIES_HPP


#include "gui/group_component.hpp"

namespace Editor {

class Label;
class EditorScreen;

/** */
class ObjectProperties : public GUI::GroupComponent
{
private:
  EditorScreen* editor;
  std::vector<LevelObj*> objects;
  Label* type_label;

  Label* gptype_label;
  RadioButtonGroup* gptype_type;

public:
  ObjectProperties(EditorScreen* editor, const Rect& rect);
  ~ObjectProperties();
  
  void set_object(LevelObj* obj);
  void draw_background(DrawingContext& gc);

  void set_objects(const std::vector<LevelObj*>& objs);
private:
  ObjectProperties (const ObjectProperties&);
  ObjectProperties& operator= (const ObjectProperties&);
};

} // namespace Editor

#endif

/* EOF */
