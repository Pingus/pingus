//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Jason Green <jave27@gmail.com>,
//                     Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "../display/drawing_context.hpp"
#include "../gui/gui_manager.hpp"
#include "../gui/surface_button.hpp"
#include "../gui/input_box.hpp"
#include "../sound/sound.hpp"
#include "../resource.hpp"
#include "../math/vector3f.hpp"
#include "../fonts.hpp"
#include "../gettext.h"
#include "level_head.hpp"
#include "level_impl.hpp"
#include "../display/display.hpp"
#include "editor_level.hpp"
//#include "editor_panel.hpp"
#include "editor_screen.hpp"

namespace Editor {
  
class LevelHeadCloseButton : public GUI::SurfaceButton
{
private:
  LevelHead* head;
  std::string label;   

public:
  LevelHeadCloseButton(LevelHead* h)
    : GUI::SurfaceButton(Display::get_width() -200,
                         Display::get_height() -100,
                         ResDescriptor("core/menu/exit_button_normal"),
                         ResDescriptor("core/menu/exit_button_pressed"),
                         ResDescriptor("core/menu/exit_button_hover")),
      head(h),
      label(_("Ok"))
  {
  }

  void draw (DrawingContext& gc) {
    SurfaceButton::draw(gc);
    gc.print_right(Fonts::chalk_large, (float)Display::get_width() - 150,
                   (float)Display::get_height() - 100, label);
  }

  void on_click()
  {
    //head->get_head_button()->remove_head();
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
  pos(Vector3f(50, 75))
{
  gui_manager->add((GUI::Component*)this, false); 
   
  // Create GUI items
  name = new GUI::InputBox(400, Vector3f(pos.x + 175, pos.y + 10),
                           impl->levelname, false, "Level Name");
  desc = new GUI::InputBox(500, Vector3f(pos.x + 175, pos.y + 60),
                           impl->description, false, "Level Description");             
  
  // Add GUI Items to the GUIManager 
  gui_manager->add((GUI::Component*)name, true);
  gui_manager->add((GUI::Component*)desc, true); 
  
  // Add close button
  close_button = new LevelHeadCloseButton(this); 
  gui_manager->add((GUI::Component*)close_button, true);
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
  gc.draw_fillrect(pos.x, pos.y, (float)Display::get_width() - 50, 
                   (float)Display::get_height()-50, Color(100,150,50,150));
  // Draw the window border
  gc.draw_rect(pos.x, pos.y, (float)Display::get_width() - 50,
               (float)Display::get_height()-50, Color(255,255,255));
}

bool
LevelHead::is_at(int x, int y)
{
  return (x > pos.x && y > pos.y && x < Display::get_width()-50 && 
          y < Display::get_height()-50);
}

void
LevelHead::update(float delta)
{
  UNUSED_ARG(delta);
  // FIXME:  Should only update this information if it has changed. 
  impl->levelname   = name->get_string();
  impl->description = desc->get_string(); 
}   
  
          
} // namespace Editor

/* EOF */
