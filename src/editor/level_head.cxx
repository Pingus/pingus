//  $Id: level_head.hxx,v 1.00 2006/02/05 23:41:12 Jave27 Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
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

#include <ClanLib/display.h>
#include "../display/drawing_context.hxx"
#include "../gui/gui_manager.hxx"
#include "../gui/surface_button.hxx"
#include "../gui/input_box.hxx"
#include "../sound/sound.hxx"
#include "../resource.hxx"
#include "../vector.hxx"
#include "../fonts.hxx"
#include "../gettext.h"
#include "level_head.hxx"
#include "level_impl.hxx"
#include "panel_buttons.hxx"
#include "editor_panel.hxx"
#include "editor_screen.hxx"
#include "xml_level.hxx"

namespace Pingus {
namespace Editor {
  
  class LevelHeadCloseButton : public GUI::SurfaceButton
  {
    private:
      LevelHead* head;
      std::string label;   

    public:
      LevelHeadCloseButton(LevelHead* h)
      : GUI::SurfaceButton(CL_Display::get_width() -100,
                           CL_Display::get_height() -100,
                           ResDescriptor("core/menu/exit_button_normal"),
                           ResDescriptor("core/menu/exit_button_pressed"),
                           ResDescriptor("core/menu/exit_button_hover")),
      head(h),
      label(_("Ok"))
      {
      }

      void draw (DrawingContext& gc) {
        SurfaceButton::draw(gc);
        gc.print_right(Fonts::chalk_large, (float)CL_Display::get_width() - 150,
                       (float)CL_Display::get_height() - 100, label);
      }

      void on_click()
      {
        head->get_head_button()->remove_head();
      }

      void on_pointer_enter()
      {
        SurfaceButton::on_pointer_enter();
        Sound::PingusSound::play_sound ("tick");
      }
  };
  	
LevelHead::LevelHead(EditorPanel* p, PanelButtonHead* pbh) :
  head_button(pbh),  
	impl(p->get_screen()->get_level()->get_level_impl()),
  gui_manager(p->get_screen()->get_gui_manager()), 
	pos(Vector(50, 75))
{
  gui_manager->add((GUI::Component*)this); 
   
  // Create GUI items
  name = new GUI::InputBox(400, Vector(pos.x + 175, pos.y + 10),
                           impl->levelname, false, "Level Name");
  desc = new GUI::InputBox(500, Vector(pos.x + 175, pos.y + 60),
                           impl->description, false, "Level Description");             
  
  // Add GUI Items to the GUIManager 
  gui_manager->add((GUI::Component*)name);
  gui_manager->add((GUI::Component*)desc); 
  
  // Add close button
  close_button = new LevelHeadCloseButton(this); 
  gui_manager->add((GUI::Component*)close_button);
}

LevelHead::~LevelHead()
{
  // Remove all GUI Items from the GUIManager 
  gui_manager->remove(name); 
  gui_manager->remove(desc); 
  gui_manager->remove(close_button); 
  gui_manager->remove((GUI::Component*)this);    
}
 
void
LevelHead::draw(DrawingContext& gc)
{
  // Draw the background for window 
	gc.draw_fillrect(pos.x, pos.y, CL_Display::get_width() - 50, 
                   CL_Display::get_height()-50, CL_Color(100,150,50,150));
  // Draw the window border
  gc.draw_rect(pos.x, pos.y, CL_Display::get_width() - 50,
               CL_Display::get_height()-50, CL_Color::white);
}

bool
LevelHead::is_at(int x, int y)
{
  return (x > pos.x && y > pos.y && x < CL_Display::get_width()-50 && 
      y < CL_Display::get_height()-50);
}

void
LevelHead::update(float delta)
{
  UNUSED_ARG(delta);
  // FIXME:  Should only update this information if it has changed. 
  impl->levelname = name->get_string();
  impl->description = desc->get_string(); 
}   
  
          
}	// Editor
}	// Pingus

/* EOF */
