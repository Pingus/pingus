//  $Id: property_window.hxx,v 1.8 2002/09/27 11:26:45 torangan Exp $
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

#ifndef PINGUS_EDITOR_PROPERTY_WINDOW_HXX
#define PINGUS_EDITOR_PROPERTY_WINDOW_HXX

#include "../pingus.hxx"
#include <ClanLib/GUI/label.h>
#include <ClanLib/GUI/window.h>

class EditorObj;
class CL_Component;

namespace EditorNS {

class Editor;

class PropertyWindow : public CL_Window
{
private:
  Editor* editor;
  CL_Component* current_frame;
  CL_Label label;
      
public:
  PropertyWindow (Editor* parent);

  void update_frame (EditorObj*);
      
private:
  PropertyWindow (const PropertyWindow&);
  PropertyWindow& operator= (const PropertyWindow&);
};

} // namespace EditorNS

#endif

/* EOF */
