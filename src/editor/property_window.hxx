//  $Id: property_window.hxx,v 1.9 2002/11/28 20:09:54 grumbel Exp $
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
  /** Pointer to the editor (FIXME: its actually a singleton, not really needed) */
  Editor* editor;
  
  /** Pointer to the frame of the current object */
  CL_Component* current_frame;

  /** The empty frame that is shown when no object is selected */
  CL_Label label;
      
public:
  PropertyWindow (Editor* parent);

  /** Called once from the editor when the selection changed or if a
      update() is called. */
  void update_frame (EditorObj*);
      
private:
  PropertyWindow (const PropertyWindow&);
  PropertyWindow& operator= (const PropertyWindow&);
};

} // namespace EditorNS

#endif

/* EOF */
