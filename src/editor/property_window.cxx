//  $Id: property_window.cxx,v 1.1 2002/06/30 22:03:13 grumbel Exp $
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

#include "../boost/smart_ptr.hpp"
#include "editorobj.hxx"
#include "property_window.hxx"

using namespace Pingus::Editor;

PropertyWindow::PropertyWindow (CL_Component* parent)
  : CL_Window (CL_Rect (0, 0, 200, 200), "Object Properties", parent),
    current_frame (0)
{
  
}

void
PropertyWindow::update_frame (boost::shared_ptr<EditorObj> obj)
{
  if (current_frame)
    remove_child (current_frame);
  
  if (obj.get ())
    {
      CL_Component* comp = obj->get_gui_dialog (this);
   
      if (comp)
	{
	  // FIXME: This looks like a workaround for a missing feature in
	  // FIXME: CL_Window
	  comp->set_position (2, 22);

	  std::cout << "Got GUI" << std::endl;
	  current_frame = comp;
	  set_client_size (comp->get_width (), comp->get_height ());
	}
      else
	{
	  std::cout << "No GUI" << std::endl;
	  current_frame = 0;
	  set_client_size (200, 30);
	}
    }
  else
    {
      set_client_size (200, 30);
    }
}

/* EOF */
