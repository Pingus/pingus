//  $Id: property_window.cxx,v 1.11 2002/11/28 20:09:54 grumbel Exp $
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

#include <iostream>
#include <ClanLib/GUI/gui_manager.h>
#include "editor.hxx"
#include "editorobj.hxx"
#include "property_frame.hxx"
#include "property_window.hxx"

namespace EditorNS {

PropertyWindow::PropertyWindow (Editor* parent)
  : CL_Window (CL_Rect (0, 0, 200, 200), "Object Properties", parent->get_gui_manager ()),
    editor (parent),
    current_frame (0), label (CL_Point (50, 0), "no properties available", get_client_area ())
{
  label.show (true);
  set_client_size (200, 20);
  show (false);
}

void
PropertyWindow::update_frame (EditorObj* obj)
{
  if (current_frame)
    { // Clean up the old frame
      std::cout << "PropertyWindow::update_frame: cleanup" << std::endl;
      get_client_area ()->remove_child (current_frame);
      delete current_frame;
      current_frame = 0;
    }
  
  if (obj)
    {
      // We are responsible to delete comp
      PropertyFrame* comp = obj->get_gui_dialog (editor);
   
      if (comp)
	{
	  set_title(comp->get_title ());
	  label.show (false);

	  std::cout << "Got GUI" << std::endl;
	  current_frame = comp;
	  set_client_size (comp->get_width (), comp->get_height ());
	}
      else
	{
	  label.show (true);
	  std::cout << "No GUI" << std::endl;
	  current_frame = 0;
	  set_client_size (200, 20);
	}
    }
  else
    {
      set_client_size (200, 20);
      label.show (true);
    }
}

} // namespace EditorNS

/* EOF */
