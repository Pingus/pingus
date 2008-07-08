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

#include <iostream>
#include <boost/bind.hpp>
#include "../display/display.hpp"
#include "../system.hpp"
#include "../sound/sound.hpp"
#include "../gui/gui_manager.hpp"
#include "../screen/screen_manager.hpp"
#include "../display/drawing_context.hpp"
#include "../res_descriptor.hpp"
#include "../resource.hpp"
#include "../fonts.hpp"
#include "gettext.h"
#include "file_dialog.hpp"
#include "../path_manager.hpp"
#include "../pathname.hpp"
#include "game_session.hpp"
#include "editor_level.hpp"
#include "panel.hpp"
#include "panel.hpp"
#include "minimap.hpp"
#include "editor_screen.hpp"
#include "viewport.hpp"
#include "level_objs.hpp"
#include "object_selector.hpp"
#include "object_properties.hpp"
#include "action_properties.hpp"
#include "level_properties.hpp"

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
  viewport = new Viewport(this, Rect(0, 38,
                                           Display::get_width() - 244, 
                                           Display::get_height()));
  gui_manager->add(viewport);
	
  // Create the panel for the buttons
  panel = new Panel(this);

  //rect(Vector2i(Display::get_width() - 244 + 2,  38 + 3 + 62),
  //      Size(240, Display::get_height() - (600 - 495))),

  minimap = new Minimap(this, Rect());
  gui_manager->add(minimap);

  object_properties = new ObjectProperties(this, Rect(Vector2i(), Size(200, 150)));
  gui_manager->add(object_properties);

  action_properties = new ActionProperties(this, Rect());
  action_properties->hide();
  gui_manager->add(action_properties);

  level_properties = new LevelProperties(this, Rect());
  level_properties->hide();
  level_properties->set_level(plf);
  action_properties->set_level(plf);
  gui_manager->add(level_properties);

  object_selector = new ObjectSelector(this, Rect());
  gui_manager->add(object_selector);

  file_load_dialog = new FileDialog(this, Rect(Vector2i(50, 50), 
                                               Size(Display::get_width() - 100, 
                                                    Display::get_height() - 100)), 
                                    FileDialog::LOAD);
  file_load_dialog->set_directory(".");
  file_load_dialog->hide();
  gui_manager->add(file_load_dialog);

  file_save_dialog = new FileDialog(this, Rect(Vector2i(50, 50), 
                                               Size(Display::get_width() - 100, 
                                                    Display::get_height() - 100)), 
                                    FileDialog::SAVE);
  file_save_dialog->set_directory(".");
  file_save_dialog->hide();
  gui_manager->add(file_save_dialog);

  viewport->selection_changed.connect(boost::bind(&ObjectProperties::set_objects, object_properties, _1));
  viewport->refresh();

  update_layout();
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
EditorScreen::save(const Pathname& file)
{
  level_pathname = file;
  std::cout << "Save to: " << file.str() << std::endl;
  if (!plf->save_level(level_pathname.get_sys_path()))
    {
      // FIXME: save failed, prompt user
    }
}

// Load a new level
void 
EditorScreen::load(const Pathname& file)
{
  level_pathname = file;
  viewport->clear_selection();
  plf->load_level(level_pathname);
  level_properties->set_level(plf);
  action_properties->set_level(plf);
  viewport->refresh();
}

// Draw the background and components
bool
EditorScreen::draw(DrawingContext& gc)
{
  // Black out screen
  //gc.fill_screen(Color(255,0,255)); // FIXME: Could be removed for added speed
  gui_manager->draw(gc);
  
  if (show_help)
    {
      Size size(600, 400);
      gc.draw_fillrect(int(gc.get_width()/2)  - size.width/2 - 2,
                       int(gc.get_height()/2) - size.height/2 - 2,
                       int(gc.get_width()/2)  + size.width/2 + 2,
                       int(gc.get_height()/2) + size.height/2 + 2,
                       Color(0,0,0));
      gc.draw_fillrect(int(gc.get_width()/2)  - size.width/2, 
                       int(gc.get_height()/2) - size.height/2,
                       int(gc.get_width()/2)  + size.width/2, 
                       int(gc.get_height()/2) + size.height/2,
                       Color(255,255,255));
      
      gc.print_center(Fonts::verdana11,
                      int(gc.get_width()/2),
                      int(gc.get_height()/2) - size.height/2 + 12,
                      "Editor Help");

      int x = int(gc.get_width()/2 - size.width/2 + 12);
      int y = int(gc.get_height()/2) - size.height/2 + 36;
      gc.print_center(Fonts::verdana11, x + 50, y,
                      "A\n"
                      "Shift+A\n"
                      "], w\n"
                      "[, s\n"
                      "Shift+]\n"
                      "Shift+[\n"
                      "R\n"
                      "Shift+R\n"
                      );

      gc.print_left(Fonts::verdana11, x+100, y,
                    _("Select all\n"
                      "Clear Selection\n"
                      "Raise objects\n"
                      "Lower objects\n"
                      "Lower objects to bottom\n"
                      "Raise objects to top\n"
                      "Rotate 90 degree\n"
                      "Rotate 270 degree\n"));

      x = int(gc.get_width()/2 + 12);
      y = int(gc.get_height()/2) - size.height/2 + 36;
      gc.print_center(Fonts::verdana11, x + 50, y,
                      "F\n"
                      "Shift+F\n"
                      "Delete\n"
                      "i\n"
                      "k\n"
                      "j\n"
                      "l\n"
                      );

      gc.print_left(Fonts::verdana11, x + 100, y,
                    _("Flip object horizontaly\n"
                      "Flip object vertically\n"
                      "Delete all marked objects\n"
                      "Move objects up\n"
                      "Move objects down\n"
                      "Move objects left\n"
                      "Move objects right\n")
                    );

      gc.print_left(Fonts::verdana11,
                    int(gc.get_width()/2 - size.width/2 + 12),
                    int(gc.get_height()/2) - 10,
                    _("You should name your level files systematically, i.e. by their theme, "
                      "their number and your nickname:\n\n"
                      "     <levelname><number>-<creator>.pingus\n\n"
                      "So if you create a your second level with a stone theme, call it: "
                      "stone2-yourname.pingus\n\n"
                      "When you have created a level and want to have it included "
                      "in the game mail it to:\n\n"
                      "     pingus-devel@nongnu.org\n\n"
                      "Only levels published under the GPL are allowed into the game. The editor "
                      "automatically inserts a reference \n"
                      "to the GPL, if you want to have your level under a different license, you "
                      "have to change that reference.\n"
                      ));
    }
  
  return true;
}

void
EditorScreen::update(const Input::Event& event)
{
  GUIScreen::update(event);

  switch (event.type)
    {
      case Input::SCROLLER_EVENT_TYPE:
        viewport->set_scroll_pos(viewport->get_scroll_pos() -
                                 Vector2f(event.scroll.x_delta,
                                          event.scroll.y_delta));
        break;

      default:
        // other events are for most part handled by the GUIScreen/GUIManager
        break;
    }
}

void
EditorScreen::add_object(LevelObj* obj)
{
  plf->add_object(obj);
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
  // FIXME: dialogs don't update
  level_pathname = Pathname();
  viewport->clear_selection();
  plf->clear();
  level_properties->set_level(plf);
  action_properties->set_level(plf);
}

void 
EditorScreen::level_load()
{
  if (file_load_dialog->is_visible())
    file_load_dialog->hide();
  else 
    file_load_dialog->show();
}

void 
EditorScreen::level_save()
{
  if (level_pathname.empty())
    {
      level_save_as();
    }
  else
    {
      save(level_pathname); 
    }
}

void 
EditorScreen::level_save_as()
{
  if (file_save_dialog->is_visible())
    file_save_dialog->hide();
  else 
    file_save_dialog->show();
}

void
EditorScreen::level_play()
{
  Pathname tmp(System::get_userdir() + "backup/editortmpfile.pingus", Pathname::SYSTEM_PATH);
  if (!plf->save_level(tmp.get_sys_path()))
    {
      // FIXME: save failed, prompt user
      return;
    }
  PingusLevel level(tmp);
  ScreenManager::instance()->push_screen(new GameSession(level, false));
}

void
EditorScreen::objects_duplicate()
{
  viewport->duplicate_selected_objects();
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
EditorScreen::toggle_minimap()
{
  if (minimap->is_visible())
    minimap->hide();
  else
    minimap->show();

  update_layout();
}

void 
EditorScreen::toggle_object_selector()
{
  if (object_selector->is_visible())
    object_selector->hide();
  else
    object_selector->show();
  
  update_layout();
}

void 
EditorScreen::toggle_help()
{
  show_help = !show_help;
}

void
EditorScreen::toggle_object_properties()
{
  if (object_properties->is_visible())
    object_properties->hide();
  else
    object_properties->show();
}

void
EditorScreen::toggle_action_properties()
{
  if (action_properties->is_visible())
    {
      action_properties->hide();
    }
  else
    {
      action_properties->show();
      level_properties->hide();
    }
}

void
EditorScreen::toggle_level_properties()
{
  if (level_properties->is_visible())
    {
      level_properties->hide();
    }
  else
    {
      level_properties->show();
      action_properties->hide();
    }
}

void
EditorScreen::exit()
{
  ScreenManager::instance()->pop_screen();  
}

void
EditorScreen::update_layout()
{
  Size size(gui_manager->get_rect().get_width(),
            gui_manager->get_rect().get_height());

  minimap->set_rect(Rect(Vector2i(size.width-244, size.height-183), Size(244, 183)));

  if (minimap->is_visible())
    object_selector->set_rect(Rect(size.width-244, 38, size.width, size.height - 183));
  else
    object_selector->set_rect(Rect(size.width-244, 38, size.width, size.height));

  if (object_selector->is_visible())
    {
      viewport->set_rect(Rect(0, 38, size.width - 244, size.height));
      level_properties->set_rect(Rect(Vector2i(0,38), Size(size.width-244, 302))); 
    }
  else
    {
      viewport->set_rect(Rect(0, 38, size.width, size.height));
      level_properties->set_rect(Rect(Vector2i(0,38), Size(size.width, 302))); 
    }

  action_properties->set_rect(Rect(Vector2i(0, 38), Size(150, 240)));

  object_properties->set_rect(Rect(Vector2i(0, size.height - object_properties->get_rect().get_height()), 
                                   Size(object_properties->get_rect().get_width(),
                                        object_properties->get_rect().get_height())));
  
  file_load_dialog->set_rect(Rect(Vector2i(50, 50), Size(size.width  - 100, 
                                                         size.height - 100)));
  file_save_dialog->set_rect(Rect(Vector2i(50, 50), Size(size.width  - 100, 
                                                         size.height - 100)));
}

} // namespace Editor 

/* EOF */


