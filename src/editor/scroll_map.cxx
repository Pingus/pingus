//  $Id: scroll_map.cxx,v 1.4 2002/09/11 12:45:58 grumbel Exp $
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

#include <ClanLib/Display/Display/display.h>
#include "scroll_map.hxx"
#include "editor.hxx"
#include "object_manager.hxx"

namespace EditorNS {

EditorEvent* ScrollMap::editor_event;

ScrollMap::ScrollMap()
  : width (200), height (100),
    x_pos (CL_Display::get_width() - width - 1),
    y_pos (CL_Display::get_height() - height - 1),
    view (new EditorView(x_pos, y_pos, x_pos + width, y_pos + height))
{
  view->set_zoom (0.1f);
  view->move (CL_Vector (-640, -480));
}

ScrollMap::~ScrollMap()
{
}

///
void
ScrollMap::on_button_release(CL_InputDevice * /*device*/, const CL_Key& /*key*/)
{

}

///
void
ScrollMap::on_button_press(CL_InputDevice * /*device*/, const CL_Key& /*key*/)
{
  //int click_x_pos = (int) key.x - x_pos;
  //int click_y_pos = (int) key.y - y_pos;

  //int viewpoint_x_pos = click_x_pos * editor_event->object_manager->get_width ()  / width;
  //int viewpoint_y_pos = click_y_pos * editor_event->object_manager->get_height () / height;

  //editor_event->object_manager->set_viewpoint(viewpoint_x_pos, viewpoint_y_pos);
}

///
bool 
ScrollMap::mouse_over(int x, int y)
{
  if (x > x_pos && x < x_pos + width
      && y > y_pos && y < y_pos + height)
    return true;
  else
    return false;
}

void 
ScrollMap::draw()
{
  CL_Display::fill_rect (x_pos, y_pos, x_pos + width, y_pos + height,
			 0.4f, 0.4f, 0.0f);
  Editor::instance ()->get_object_manager ()->draw (view.get ());
  /*
  int viewarea_width = (CL_Display::get_width() * width
			/ editor_event->object_manager->get_width());
  int viewarea_height = (CL_Display::get_height() * height
			 / editor_event->object_manager->get_height());
  int viewarea_x_pos = int((x_pos - (view->get_offset().x * width
				 / editor_event->object_manager->get_width())));
  int viewarea_y_pos = int((y_pos - (view->get_offset().y * height
				 / editor_event->object_manager->get_height()))); 

  CL_Display::fill_rect(x_pos, y_pos,
			x_pos + width,
			y_pos + height,
			0.0, 0.0, 0.0, 1.0);

  // The rectangle, which represents the current viewpoint
  if (mouse_over(CL_Mouse::get_x(), CL_Mouse::get_y()))
    {
      Display::draw_rect(viewarea_x_pos,
			 viewarea_y_pos,
			 viewarea_x_pos + viewarea_width,
			 viewarea_y_pos + viewarea_height,
			 1.0, 1.0, 1.0, 1.0);
    }

  editor_event->object_manager->draw_scroll_map(x_pos, y_pos, width, height);
  

  // The rectangle, which represents the complet world
  Display::draw_rect(x_pos, y_pos,
		     x_pos + width,
		     y_pos + height,
		     1.0, 1.0, 1.0, 1.0);*/
}

} // namespace EditorNS

/* EOF */

