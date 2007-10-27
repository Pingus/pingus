//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#include <config.h>
#include "gettext.h"
#include "components/menu_button.hpp"
#include "resource.hpp"
#include "debug.hpp"
#include "globals.hpp"
#include "sound/sound.hpp"
#include "level_menu.hpp"
#include "stat_manager.hpp"
#include "start_screen.hpp"
#include "story_screen.hpp"
#include "worldmap/worldmap.hpp"
#include "worldmap/manager.hpp"
#include "screen/screen_manager.hpp"
#include "pingus_menu_manager.hpp"
#include "gui/gui_manager.hpp"
#include "plf_res_mgr.hpp"
#include "path_manager.hpp"
#include "pathname.hpp"
#include "file_dialog.hpp"
#include "editor/editor_screen.hpp"
#include "credits.hpp"


PingusMenu::PingusMenu (PingusMenuManager* m)
  : PingusSubMenu (m)
{
  is_init = false;
    
  start_button = new MenuButton(this, Vector2i(Display::get_width()  * 250 / 800,
                                               Display::get_height() * 330 / 600),
                                _("Story"),
                                _("..:: Start the game ::.."));


  editor_button = new MenuButton(this, Vector2i(Display::get_width()  * 550 / 800,
                                                Display::get_height() * 330 / 600),
                                 _("Editor"),
                                 _("..:: Create your own levels ::.."));

  quit_button = new MenuButton(this, Vector2i(Display::get_width()  * 550 / 800,
                                              Display::get_height() * 410 / 600),
                               _("Exit"),
                               _("..:: Bye, bye ::.."));

  contrib_button = new MenuButton(this, Vector2i(Display::get_width()  * 250 / 800,
                                                 Display::get_height() * 410 / 600),
                                  _("Levelsets"),
                                  _("..:: Play User Built levels ::.."));

#ifdef NEW_MENU
  story_button  = new MenuButton(this, Vector2i(Display::get_width() * 400 / 800,
                                                Display::get_height() * 340 / 600),
                                 _("Story"),
                                 _("..:: Start the story ::.."));
  
  multiplayer_button = new MenuButton(this, Vector2i(Display::get_width() * 150 / 800,
                                                     Display::get_height() * 340 / 600),
                                      Resource::load_sprite("core/menu/multi_on"),
                                      _("Multiplayer"),
                                      _("..:: Multiplayer Match ::.."));
#endif

  help = _("..:: Ctrl-g: mouse grab   ::   F10: fps counter   ::   F11: fullscreen   ::   F12: screenshot ::..");
}

void
PingusMenu::setup_main_menu()
{
#ifdef NEW_MENU
  gui_manager->remove(contrib_button);
  gui_manager->remove(story_button);
  gui_manager->remove(multiplayer_button);
  gui_manager->remove(editor_button);
#endif

  gui_manager->add(quit_button, false);
  gui_manager->add(contrib_button, false);
  gui_manager->add(start_button, false);
  gui_manager->add(editor_button, false);
}

#ifdef NEW_MENU
void
PingusMenu::setup_game_menu()
{
  gui_manager->remove(start_button);

  gui_manager->add(contrib_button, false);
  gui_manager->add(story_button, false);
  gui_manager->add(editor_button, false);
  // FIXME: Re-enable this next line once multiplayer functionality
  // is actually available.

  //gui_manager->add(multiplayer_button, false);
}
#endif

void
PingusMenu::show_credits()
{
      ScreenManager::instance()->push_screen(Credits::instance(), false);
}

void
PingusMenu::setup_contrib_menu()
{
#if 0
  if (filedialog)
    delete filedialog;
  filedialog = new FileDialog(this, ".pingus", 
                              path_manager.complete("levels/"), true);
  manager->push_menu (filedialog);
#endif
}

void
PingusMenu::setup_worldmap_menu()
{
#if 0
  if (filedialog)
    delete filedialog;
  filedialog = new FileDialog(this, ".worldmap", 
                              path_manager.complete("worldmaps/"), true);
  manager->push_menu (filedialog);
#endif 
}

void
PingusMenu::preload ()
{
  if (!is_init)
    {
      is_init = true;

      background = Resource::load_sprite("core/misc/logo");
      setup_main_menu();
    }
}

PingusMenu::~PingusMenu()
{
  delete start_button;
  delete quit_button;
  delete editor_button;
  delete contrib_button;
#ifdef NEW_MENU
  delete story_button;
  delete multiplayer_button;
#endif
}

void
PingusMenu::do_quit()
{
  get_manager()->show_exit_menu ();
}

void
PingusMenu::do_start(const std::string &filename)
{ // Start the story or worldmap mode
  Sound::PingusSound::play_sound ("letsgo");
  WorldMapNS::WorldMapManager::instance()->load(filename);
  
  bool story_seen = false;
  StatManager::instance()->get_bool(WorldMapNS::WorldMapManager::instance()->
                                    get_worldmap()->get_shortname() + "-startstory-seen", story_seen);
	
  if (!story_seen)
    ScreenManager::instance()->push_screen
      (new StoryScreen(WorldMapNS::WorldMapManager::instance()->get_worldmap()->get_intro_story()), true);
  else
    ScreenManager::instance()->push_screen(WorldMapNS::WorldMapManager::instance());
}

void PingusMenu::do_contrib(const std::string &levelfile)
{ // Launch the specified level - don't bother checking for it, it has to exist
  Sound::PingusSound::play_sound ("letsgo");
  ScreenManager::instance()->push_screen
    (new StartScreen(PLFResMgr::load_plf_from_filename(Pathname(levelfile, Pathname::SYSTEM_PATH))), true);
}

void PingusMenu::do_edit()
{	// Launch the level editor
  Sound::PingusSound::stop_music();
  ScreenManager::instance()->push_screen (new Editor::EditorScreen(), true);
}

void
PingusMenu::on_resize(int w, int h)
{
  pout << "Width: " << w << " Height: " << h << std::endl;
}

void
PingusMenu::on_escape_press ()
{
  get_manager ()->show_exit_menu ();
}

void
PingusMenu::draw_foreground(DrawingContext& gc)
{
  if (gc.get_height() == 480)
    {
      gc.draw(background,
              Vector2i((gc.get_width()/2) - (background.get_width()/2),
                       20));
    }
  else
    {
      gc.draw(background, 
              Vector2i((gc.get_width()/2) - (background.get_width()/2),
                       Display::get_height()/10));
    }

  gc.print_left(Fonts::pingus_small, 25, 
                Display::get_height()-130,
                "Pingus "VERSION", Copyright (C) 1998-2007 Ingo Ruhnke <grumbel@gmx.de>\n"
                "See the file AUTHORS for a complete list of contributors.\n");
  gc.print_left(Fonts::pingus_small, 25, 
                Display::get_height()-80,
                "Pingus comes with ABSOLUTELY NO WARRANTY. This is free software, and you are\n"
                "welcome to redistribute it under certain conditions; see the file COPYING for details.\n");

  gc.print_center(Fonts::pingus_small, Display::get_width() / 2,
                  Display::get_height() - Fonts::pingus_small.get_height(),
                  help);

  if (0)
    {
      gc.print_center(Fonts::pingus_small, Display::get_width() / 2,
                      Display::get_height() - Fonts::pingus_small.get_height(),
                      hint);
    }
}

void
PingusMenu::load(const std::string &file, const std::string &filemask)
{
  // Level
  if (filemask == ".pingus")
    do_contrib(file);
  // Worldmap
  else if (filemask == ".worldmap")
    do_start(file);
  manager->pop_menu();
}
		
void
PingusMenu::cancel()
{
  manager->pop_menu();
}

void
PingusMenu::on_click(MenuButton* button)
{
  if (button == start_button)
    {
#ifdef NEW_MENU
      setup_game_menu();
#else
      do_start("worldmaps/tutorial.worldmap");
#endif
    }
  else if (button == quit_button)
    {
      do_quit();
    }
  else if (button == editor_button)
    {
      do_edit();
    }
  else if (button == contrib_button)
    {
      ScreenManager::instance()->push_screen(new LevelMenu(), true);
    }
#ifdef NEW_MENU
  else if (button == story_button)
    {
      setup_worldmap_menu();
    }
  else if (button == multiplayer_button)
    {
      setup_main_menu();
    }
#endif
}

void
PingusMenu::set_hint(const std::string& str)
{
  hint = str;
}

void
PingusMenu::update(float delta)
{
  //text_scroll_offset += 100.0f * delta;
}

/* EOF */
