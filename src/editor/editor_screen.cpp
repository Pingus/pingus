//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Jason Green <jave27@gmail.com>,
//                     Ingo Ruhnke <grumbel@gmx.de>
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
#include <boost/bind.hpp>
#include "../gui/display.hpp"
#include "../system.hpp"
#include "../sound/sound.hpp"
#include "../gui/gui_manager.hpp"
#include "../gui/screen_manager.hpp"
#include "../display/drawing_context.hpp"
#include "../res_descriptor.hpp"
#include "../resource.hpp"
#include "../fonts.hpp"
#include "../file_dialog.hpp"
#include "file_load_dialog.hpp"
#include "../path_manager.hpp"
#include "../pathname.hpp"
#include "game_session.hpp"
#include "editor_level.hpp"
#include "panel.hpp"
#include "panel.hpp"
#include "editor_screen.hpp"
#include "editor_viewport.hpp"
#include "level_objs.hpp"
#include "object_selector.hpp"
#include "object_properties.hpp"

namespace Editor {

// Default constructor
EditorScreen::EditorScreen()
  : plf(new EditorLevel()), 
    panel(0),
    viewport(0),
    object_selector(0),
    show_help(false)
{
  // Create the viewport for the images and data
  viewport = new EditorViewport(this);
  gui_manager->add(viewport, true);	
	
  // Create the panel for the buttons
  panel = new Panel(this);

  object_selector = new ObjectSelector(this);
  object_properties = new ObjectProperties(this, Rect(0,500,200,600));
  gui_manager->add(object_properties, true);
  file_load_dialog = new FileLoadDialog(this, Rect(Vector2i(50, 50), 
                                                   Size(Display::get_width() - 100, 
                                                        Display::get_height() - 100)));
  file_load_dialog->set_directory(".");
  file_load_dialog->hide();
  gui_manager->add(file_load_dialog, true);

  viewport->selection_changed.connect(boost::bind(&ObjectProperties::set_objects, object_properties, _1));
}

// Destructor
EditorScreen::~EditorScreen()
{
  delete plf;
}

// Close the current screen
void
EditorScreen::close_screen()
{
  ScreenManager::instance()->pop_screen();
}

// Escape was pressed
void
EditorScreen::on_escape_press()
{
  close_screen();
}

// Save the current level
void 
EditorScreen::save(const std::string &file)
{
  plf->save_level(file);
}

// Load a new level
void 
EditorScreen::load(const Pathname& file)
{
  plf->load_level(file);
  viewport->refresh();
}

// Draw the background and components
bool
EditorScreen::draw(DrawingContext& gc)
{
  // Black out screen
  gc.fill_screen(Color(255,0,255));
  gui_manager->draw(gc);
  
  if (show_help)
    {
      Size size(600, 400);
      gc.draw_fillrect(int(gc.get_width()/2)  - size.width/2, 
                       int(gc.get_height()/2) - size.height/2,
                       int(gc.get_width()/2)  + size.width/2, 
                       int(gc.get_height()/2) + size.height/2,
                       Color(0,0,0));
      
      gc.print_center(Fonts::courier_small,
                      int(gc.get_width()/2),
                      int(gc.get_height()/2) - size.height/2 + 12,
                      "Editor Help");

      gc.print_left(Fonts::courier_small,
                    int(gc.get_width()/2 - size.width/2 + 12),
                    int(gc.get_height()/2) - size.height/2 + 36,
                    "F1 - display this help\n"
                    "F2 - toggle background color\n"
                    "F4 - play the level\n"
                    "F5 - load a level\n"
                    "F6 - save this level\n"
                    "F8 - backup save\n"
                    "F9 - change level width/height\n"
                    );

      gc.print_left(Fonts::courier_small,
                    int(gc.get_width()/2 + 12),
                    int(gc.get_height()/2) - size.height/2 + 36,
                    "F1 - display this help\n"
                    "F2 - toggle background color\n"
                    "F4 - play the level\n"
                    "F5 - load a level\n"
                    "F6 - save this level\n"
                    "F8 - backup save\n"
                    "F9 - change level width/height\n"
                    );

      gc.print_left(Fonts::courier_small,
                    int(gc.get_width()/2 - size.width/2 + 12),
                    int(gc.get_height()/2) - 10,
                    "Naming Convention: <levelname><number>-<creator>.pingus\n\n"
                    "When you have created a level and want to have it included\n"
                    "in the game mail it to pingus-devel@nongnu.org\n"
                    "\n");
    }
  
  return true;
}

void
EditorScreen::update(const GameDelta &delta)
{
  GUIScreen::update(delta);

  SDL_Delay(33); // FIXME: Brute force delay to get CPU usage down
}

void
EditorScreen::add_object(LevelObj* obj)
{
  plf->add_object(obj);
  viewport->add_object(obj);
}

void
EditorScreen::add_objects(std::vector<LevelObj*> objs)
{
  for (std::vector<LevelObj*>::const_iterator it = objs.begin(); it != objs.end(); it++)
    add_object(*it);
}

void 
EditorScreen::level_new()
{
  std::cout << "Function at '" << __FILE__ << ":" << __LINE__ << "' is unimplemented" << std::endl; 
}

void 
EditorScreen::level_load()
{
  if (file_load_dialog->is_visible())
    file_load_dialog->hide();
  else 
    {
      //file_load_dialog->set_rect(Rect(Vector2i(rand() % 200, rand() % 200),
      //                                Size(rand()%600+200, rand()%600+300)));
      file_load_dialog->show();
    }
}

void 
EditorScreen::level_save()
{
  std::cout << "Function at '" << __FILE__ << ":" << __LINE__ << "' is unimplemented" << std::endl; 
}

void 
EditorScreen::level_save_as()
{
  std::cout << "Function at '" << __FILE__ << ":" << __LINE__ << "' is unimplemented" << std::endl; 
}
void
EditorScreen::level_play()
{
  plf->save_level(path_manager.complete("levels/editortmpfile.pingus"));
  PingusLevel level("levels/editortmpfile.pingus",
                    Pathname("levels/editortmpfile.pingus", Pathname::DATA_PATH));
  ScreenManager::instance()->push_screen(new PingusGameSession(level, false), true);
}

void 
EditorScreen::show_level_properties()
{
  std::cout << "Function at '" << __FILE__ << ":" << __LINE__ << "' is unimplemented" << std::endl; 
}

void 
EditorScreen::show_action_properties()
{
  std::cout << "Function at '" << __FILE__ << ":" << __LINE__ << "' is unimplemented" << std::endl; 
}

void 
EditorScreen::show_object_properties()
{
  std::cout << "Function at '" << __FILE__ << ":" << __LINE__ << "' is unimplemented" << std::endl; 
}

void 
EditorScreen::objects_delete()
{
  viewport->delete_selected_objects();
}

void 
EditorScreen::objects_raise_to_top()
{
  viewport->raise_objects_to_top();
}

void 
EditorScreen::objects_raise()
{
  viewport->raise_objects();
}

void 
EditorScreen::objects_lower()
{
  viewport->lower_objects();
}

void 
EditorScreen::objects_lower_to_bottom()
{
  viewport->lower_objects_to_bottom();
}


void 
EditorScreen::objects_flip_vertical()
{
  viewport->vflip_selected_objects();
}

void 
EditorScreen::objects_flip_horizontal()
{
  viewport->hflip_selected_objects();
}

void 
EditorScreen::objects_rotate_left()
{
  viewport->rotate_270_selected_objects();
}

void 
EditorScreen::objects_rotate_right()
{
  viewport->rotate_90_selected_objects();
}

void 
EditorScreen::toggle_grid_snap()
{
  std::cout << "Function at '" << __FILE__ << ":" << __LINE__ << "' is unimplemented" << std::endl; 
}

void 
EditorScreen::toggle_object_selector()
{
  std::cout << "Function at '" << __FILE__ << ":" << __LINE__ << "' is unimplemented" << std::endl; 
}

void 
EditorScreen::toggle_help()
{
  show_help = !show_help;
}

void
EditorScreen::exit()
{
  ScreenManager::instance()->pop_screen();  
}

} // namespace Editor 

/* EOF */


