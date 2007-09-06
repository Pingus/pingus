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

namespace Editor {

// Default constructor
EditorScreen::EditorScreen()
  : plf(new EditorLevel()), 
    panel(0),
    viewport(0),
    object_selector(0),
    filedialog(0),
    close_dialog(false),
    show_help(false)
{
  // Create the viewport for the images and data
  viewport = new EditorViewport(this);
  gui_manager->add(viewport, true);	
	
  // Create the panel for the buttons
  panel = new Panel(this);

  object_selector = new ObjectSelector(this);
}

// Destructor
EditorScreen::~EditorScreen()
{
  delete plf;
  if (filedialog)
    delete filedialog;
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

// Show dialog box
void
EditorScreen::show_file_dialog(bool for_loading)
{
  if (filedialog)
    delete filedialog;
  close_dialog = false;
  filedialog = new FileDialog(this, ".scm", 
                              path_manager.complete("levels/"), for_loading);
  filedialog->preload();	
}

// Close dialog box
void
EditorScreen::cancel()
{
  close_dialog = true;
}

// Save the current level
void 
EditorScreen::save(const std::string &file)
{
  close_dialog = true;
  plf->save_level(file);
}

// Load a new level
void 
EditorScreen::load(const Pathname& file)
{
  close_dialog = true;
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
  
  if (filedialog)
    filedialog->draw(gc);
	
  return true;
}

void
EditorScreen::update(const GameDelta &delta)
{
  if (filedialog)
    {
      if (close_dialog)
        {
          delete filedialog;
          filedialog = 0;
        }
      else
        filedialog->update(delta);
    }
  else
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
  std::cout << "Function at '" << __FILE__ << ":" << __LINE__ << "' is unimplemented" << std::endl; 
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
  std::cout << "Function at '" << __FILE__ << ":" << __LINE__ << "' is unimplemented" << std::endl; 
}

void 
EditorScreen::objects_raise()
{
  std::cout << "Function at '" << __FILE__ << ":" << __LINE__ << "' is unimplemented" << std::endl; 
}

void 
EditorScreen::objects_lower()
{
  std::cout << "Function at '" << __FILE__ << ":" << __LINE__ << "' is unimplemented" << std::endl; 
}

void 
EditorScreen::objects_lower_to_bottom()
{
  std::cout << "Function at '" << __FILE__ << ":" << __LINE__ << "' is unimplemented" << std::endl; 
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


