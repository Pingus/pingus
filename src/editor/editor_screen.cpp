// Pingus - A free Lemmings clone
// Copyright (C) 2007 Jason Green <jave27@gmail.com>,
//                    Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "editor/editor_screen.hpp"

#include <sstream>

#include "editor/action_properties.hpp"
#include "editor/editor_level.hpp"
#include "editor/level_properties.hpp"
#include "editor/minimap.hpp"
#include "editor/object_properties.hpp"
#include "editor/object_selector.hpp"
#include "editor/panel.hpp"
#include "editor/viewport.hpp"
#include "engine/display/display.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/sound/sound.hpp"
#include "engine/screen/screen_manager.hpp"
#include "editor/message_box.hpp"
#include "pingus/fonts.hpp"
#include "pingus/gettext.h"
#include "pingus/screens/game_session.hpp"
#include "util/log.hpp"

namespace Editor {

// Default constructor
EditorScreen::EditorScreen() :
  plf(new EditorLevel()),
  level_pathname(),
  panel(0),
  viewport(0),
  object_selector(0),
  minimap(),
  object_properties(),
  action_properties(),
  level_properties(),
  file_load_dialog(),
  file_save_dialog(),
  m_level_new_msgbox(),
  show_help(false)
{
  // Create the viewport for the images and data
  viewport = gui_manager->create<Viewport>(this, Rect(0, 38,
                                                      size.width - 244,
                                                      size.height));

  // Create the panel for the buttons
  panel = gui_manager->create<Panel>(this, Rect(0, 0, size.width, 38));

  //rect(Vector2i(Display::get_width() - 244 + 2,  38 + 3 + 62),
  //      Size(240, Display::get_height() - (600 - 495))),

  minimap = gui_manager->create<Minimap>(this, Rect());

  object_properties = gui_manager->create<ObjectProperties>(this, Rect(Vector2i(), Size(200, 150)));

  action_properties = gui_manager->create<ActionProperties>(this, Rect());
  action_properties->hide();

  level_properties = gui_manager->create<LevelProperties>(this, Rect());
  level_properties->hide();
  level_properties->set_level(plf.get());
  action_properties->set_level(plf.get());

  object_selector = gui_manager->create<ObjectSelector>(this, Rect());

  file_load_dialog = gui_manager->create<FileDialog>(this, Rect(Vector2i(50, 50),
                                                                Size(size.width  - 100,
                                                                     size.height - 100)),
                                                     FileDialog::LOAD);
  file_load_dialog->hide();

  file_save_dialog = gui_manager->create<FileDialog>(this, Rect(Vector2i(50, 50),
                                                                Size(Display::get_width() - 100,
                                                                     Display::get_height() - 100)),
                                                     FileDialog::SAVE);
  file_save_dialog->hide();

  {
    Size msg_size(600, 160);

    m_level_new_msgbox = gui_manager->create<MessageBox>(Rect(Vector2i((Display::get_width() - msg_size.width)/2,
                                                                       (Display::get_height() - msg_size.height)/2),
                                                              msg_size));
    m_level_new_msgbox->set_title("Create new level");
    m_level_new_msgbox->set_text("Replace current level with an empty new one?");
    m_level_new_msgbox->set_ok_text("Replace");
    m_level_new_msgbox->on_ok.connect(std::bind(&EditorScreen::level_new_without_confirm, this));

    m_level_new_msgbox->hide();
  }

  viewport->selection_changed.connect(std::bind(&ObjectProperties::set_objects, object_properties, std::placeholders::_1));
  viewport->refresh();

  update_layout();
}

// Destructor
EditorScreen::~EditorScreen()
{
}

void
EditorScreen::on_startup ()
{
  Sound::PingusSound::stop_music();
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

void
EditorScreen::on_action_up_press()
{
  object_selector->scroll_up();
}

void
EditorScreen::on_action_down_press()
{
  object_selector->scroll_down();
}

// Save the current level
void
EditorScreen::save(const Pathname& file)
{
  try
  {
    std::string filename = file.get_sys_path();

    if (System::get_file_extension(filename) == "prefab")
    {
      level_pathname = file;
      log_info("Save to: %1%", file.str());
      plf->save_prefab(filename);
    }
    else
    {
      level_pathname = file;
      log_info("Save to: %1%", file.str());
      plf->save_level(filename);
    }
  }
  catch(std::exception const& err)
  {
    Size msg_size(600, 160);
    auto msgbox = gui_manager->create<MessageBox>(Rect(Vector2i((Display::get_width() - msg_size.width)/2,
                                                                (Display::get_height() - msg_size.height)/2),
                                                       msg_size));
    msgbox->set_title("Error: level saving failed");
    msgbox->set_text(err.what());
  }
}

void
EditorScreen::set_level(std::unique_ptr<EditorLevel> level)
{
  viewport->clear_selection();

  plf = std::move(level);

  level_properties->set_level(plf.get());
  action_properties->set_level(plf.get());
  viewport->refresh();
}

// Load a new level
void
EditorScreen::load(const Pathname& file)
{
  try
  {
    std::string filename = file.get_sys_path();

    if (System::get_file_extension(filename) == "prefab")
    {
      level_pathname = file;
      set_level(EditorLevel::from_prefab_file(level_pathname));
    }
    else
    {
      level_pathname = file;
      set_level(EditorLevel::from_level_file(level_pathname));
    }
  }
  catch(const std::exception& err)
  {
    // FIXME: show a MessageBox
    log_error("%1%", err.what());
  }
}

// Draw the background and components
void
EditorScreen::draw(DrawingContext& gc)
{
  // Black out screen
  //gc.fill_screen(Color(255,0,255)); // FIXME: Could be removed for added speed
  gui_manager->draw(gc);

  if (show_help)
  {
    Size size_(640, 400);
    gc.draw_fillrect(Rect(gc.get_width()/2  - size_.width/2 - 2,
                          gc.get_height()/2 - size_.height/2 - 2,
                          gc.get_width()/2  + size_.width/2 + 2,
                          gc.get_height()/2 + size_.height/2 + 2),
                     Color(0,0,0));
    gc.draw_fillrect(Rect(gc.get_width()/2  - size_.width/2,
                          gc.get_height()/2 - size_.height/2,
                          gc.get_width()/2  + size_.width/2,
                          gc.get_height()/2 + size_.height/2),
                     Color(255,255,255));

    gc.print_center(Fonts::verdana11,
                    Vector2i(gc.get_width()/2,
                             gc.get_height()/2 - size_.height/2 + 12),
                    _("== Editor Help =="));

    int x = gc.get_width()/2 - size_.width/2 + 12;
    int y = gc.get_height()/2 - size_.height/2 + 36;
    gc.print_center(Fonts::verdana11, Vector2i(x + 75, y),
                    "A\n"
                    "Shift+A\n"
                    "PageUp, ], w\n"
                    "PageDown, [, s\n"
                    "Shift+PageUp\n"
                    "Shift+PageDown\n"
                    "R\n"
                    "Shift+R\n"
                    "G\n"
                    "Shift+G\n"
      );

    gc.print_left(Fonts::verdana11, Vector2i(x+150, y),
                  _("Select all\n"
                    "Clear Selection\n"
                    "Raise objects\n"
                    "Lower objects\n"
                    "Raise objects to top\n"
                    "Lower objects to bottom\n"
                    "Rotate 90 degree\n"
                    "Rotate 270 degree\n"
                    "Group selected objects\n"
                    "Ungroup selected objects\n"));

    x = int(gc.get_width()/2 + 12);
    y = int(gc.get_height()/2) - size_.height/2 + 36;
    gc.print_center(Fonts::verdana11, Vector2i(x + 50, y),
                    "F\n"
                    "Shift+F\n"
                    "Delete, Backspace\n"
                    "I, K, J, L\n"
                    "Shift+I, K, J, L\n"
                    "B, Shift+B\n"
                    "=, +, -\n");

    gc.print_left(Fonts::verdana11, Vector2i(x + 125, y),
                  _("Flip object horizontally\n"
                    "Flip object vertically\n"
                    "Delete all marked objects\n"
                    "Move objects by one pixel\n"
                    "Move objects by 32 pixel\n"
                    "Toggle background color\n"
                    "Increase/lower repeat\n"));

    gc.print_left(Fonts::verdana11,
                  Vector2i(gc.get_width()/2 - size_.width/2 + 12,
                           gc.get_height()/2 - 10),
                  _("You should name your level files systematically, i.e. by their theme, "
                    "their number and your nickname:\n\n"
                    "     <levelname><number>-<creator>.pingus\n\n"
                    "So if you create your second level with a stone theme, call it: "
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
}

void
EditorScreen::update(const Input::Event& event)
{
  GUIScreen::update(event);

  switch (event.type)
  {
    case Input::SCROLLER_EVENT_TYPE:
      viewport->set_scroll_pos(viewport->get_scroll_pos() -
                               Vector2i(static_cast<int>(event.scroll.x_delta),
                                        static_cast<int>(event.scroll.y_delta)));
      break;

    default:
      // other events are for most part handled by the GUIScreen/GUIManager
      break;
  }
}

void
EditorScreen::level_new_without_confirm()
{
  // FIXME: dialogs don't update
  level_pathname = Pathname();
  set_level(std::unique_ptr<EditorLevel>(new EditorLevel));
}

void
EditorScreen::level_new()
{
  m_level_new_msgbox->show();
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
  plf->save_level(tmp.get_sys_path());
  PingusLevel level(tmp);
  ScreenManager::instance()->push_screen(std::make_shared<GameSession>(level, false));
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
  Size size_(gui_manager->get_rect().get_width(),
             gui_manager->get_rect().get_height());

  panel->set_rect(Rect(0, 0, size_.width, 38));
  minimap->set_rect(Rect(Vector2i(size_.width-244, size_.height-183), Size(244, 183)));

  if (minimap->is_visible())
    object_selector->set_rect(Rect(size_.width-244, 38, size_.width, size_.height - 183));
  else
    object_selector->set_rect(Rect(size_.width-244, 38, size_.width, size_.height));

  if (object_selector->is_visible())
  {
    viewport->set_rect(Rect(0, 38, size_.width - 244, size_.height));
    level_properties->set_rect(Rect(Vector2i(0,38), Size(size_.width-244, 258)));
  }
  else
  {
    viewport->set_rect(Rect(0, 38, size_.width, size_.height));
    level_properties->set_rect(Rect(Vector2i(0,38), Size(size_.width, 258)));
  }

  action_properties->set_rect(Rect(Vector2i(0, 38), Size(150, 240)));

  object_properties->set_rect(Rect(Vector2i(0, size_.height - object_properties->get_rect().get_height()),
                                   Size(object_properties->get_rect().get_width(),
                                        object_properties->get_rect().get_height())));

  file_load_dialog->set_rect(Rect(Vector2i(50, 50), Size(size_.width  - 100,
                                                         size_.height - 100)));
  file_save_dialog->set_rect(Rect(Vector2i(50, 50), Size(size_.width  - 100,
                                                         size_.height - 100)));
}

void
EditorScreen::resize(const Size& size_)
{
  gui_manager->set_rect(Rect(Vector2i(0, 0), size_));

  {
    Size msg_size(600, 160);
    m_level_new_msgbox->set_rect(Rect(Vector2i((Display::get_width()  - msg_size.width)/2,
                                               (Display::get_height() - msg_size.height)/2),
                                      msg_size));
  }

  update_layout();
}

} // namespace Editor

/* EOF */

