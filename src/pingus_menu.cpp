//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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
#include "worldmap/worldmap_screen.hpp"
#include "screen/screen_manager.hpp"
#include "pingus_menu_manager.hpp"
#include "gui/gui_manager.hpp"
#include "plf_res_mgr.hpp"
#include "path_manager.hpp"
#include "pathname.hpp"
#include "editor/editor_screen.hpp"
#include "credits.hpp"

PingusMenu::PingusMenu()
{
  is_init = false;
    
  start_button = new MenuButton(this, Vector2i(Display::get_width()  * 250 / default_screen_width,
                                               Display::get_height() * 330 / default_screen_height),
                                _("Story"),
                                _("..:: Start the game ::.."));


  editor_button = new MenuButton(this, Vector2i(Display::get_width()  * 550 / default_screen_width,
                                                Display::get_height() * 330 / default_screen_height),
                                 _("Editor"),
                                 _("..:: Create your own levels ::.."));

  quit_button = new MenuButton(this, Vector2i(Display::get_width()  * 550 / default_screen_width,
                                              Display::get_height() * 410 / default_screen_height),
                               _("Exit"),
                               _("..:: Bye, bye ::.."));

  contrib_button = new MenuButton(this, Vector2i(Display::get_width()  * 250 / default_screen_width,
                                                 Display::get_height() * 410 / default_screen_height),
                                  _("Levelsets"),
                                  _("..:: Play User Built levels ::.."));

  gui_manager->add(quit_button);
  gui_manager->add(contrib_button);
  gui_manager->add(start_button);
  gui_manager->add(editor_button);

  logo = Sprite("core/misc/logo");

  int w = Display::get_width();
  int h = Display::get_height();

  Sprite layer1("core/menu/layer1");
  Sprite layer2("core/menu/layer2");
  Sprite layer3("core/menu/layer3");
  Sprite layer4("core/menu/layer4");
  Sprite layer5("core/menu/layer5");

  // We only need to scale the background main menu images if the screen 
  // resolution is not default
  if (w != default_screen_width && h != default_screen_height)
    {
      layer1.scale(w, 185 * h / default_screen_height);
      layer2.scale(w, 362 * h / default_screen_height);
      layer3.scale(w, 306 * h / default_screen_height);
      layer4.scale(w, 171 * h / default_screen_height);
      layer5.scale(302 * w / default_screen_width, 104 * h / default_screen_height);
      
      background.add_layer(layer1, 0, 0, 12, 0);
      background.add_layer(layer2, 0, 150 * (float)h / default_screen_height, 25, 0);
      background.add_layer(layer3, 0, 200 * (float)h / default_screen_height, 50, 0);
      background.add_layer(layer4, 0, 429 * (float)h / default_screen_height, 100, 0);
      background.add_layer(layer5, 0, 500 * (float)h / default_screen_height, 200, 0);
    }
  else
    {
      background.add_layer(layer1, 0, 0, 12, 0);
      background.add_layer(layer2, 0, 150, 25, 0);
      background.add_layer(layer3, 0, 200, 50, 0);
      background.add_layer(layer4, 0, 429, 100, 0);
      background.add_layer(layer5, 0, 500, 200, 0);
    }

  help = _("..:: Ctrl-g: mouse grab   ::   F10: fps counter   ::   F11: fullscreen   ::   F12: screenshot ::..");
}

PingusMenu::~PingusMenu()
{
}

void
PingusMenu::show_credits()
{
  ScreenManager::instance()->push_screen(new Credits());
}

void
PingusMenu::do_quit()
{
  ScreenManager::instance ()->pop_screen ();
}

void
PingusMenu::do_start(const std::string &filename)
{ // Start the story or worldmap mode
  Sound::PingusSound::play_sound ("letsgo");

#if 0  // FIXME: Fri Jul  4 10:33:01 2008
  bool story_seen = false;
  StatManager::instance()->get_bool("tutorial-startstory-seen", story_seen); // FIXME: Hardcoding tutorial is evil
	
  if (!story_seen)
    {
      ScreenManager::instance()->push_screen
        (new StoryScreen(WorldmapNS::WorldmapScreen::instance()->get_worldmap()->get_intro_story()), true);
    }
  else
#endif
    {
      std::auto_ptr<WorldmapNS::WorldmapScreen> worldmap_screen(new WorldmapNS::WorldmapScreen());
      worldmap_screen->load(filename);
      ScreenManager::instance()->push_screen(worldmap_screen.release());
    }
}

void PingusMenu::do_contrib(const std::string &levelfile)
{ // Launch the specified level - don't bother checking for it, it has to exist
  Sound::PingusSound::play_sound ("letsgo");
  ScreenManager::instance()->push_screen
    (new StartScreen(PLFResMgr::load_plf_from_filename(Pathname(levelfile, Pathname::SYSTEM_PATH))));
}

void PingusMenu::do_edit()
{	// Launch the level editor
  Sound::PingusSound::stop_music();
  ScreenManager::instance()->push_screen (new Editor::EditorScreen());
}

void
PingusMenu::on_resize(int w, int h)
{
  pout << "Width: " << w << " Height: " << h << std::endl;
}

void
PingusMenu::on_escape_press ()
{
  //FIXME: get_manager()->show_exit_menu ();
}

void
PingusMenu::draw_background(DrawingContext& gc)
{
  background.draw(gc);

  gc.draw_fillrect(0,
                   Display::get_height () - 26,
                   Display::get_width (),
                   Display::get_height (),
                   Color(0, 0, 0, 255));

  if (gc.get_height() == 480)
    {
      gc.draw(logo,
              Vector2i((gc.get_width()/2) - (logo.get_width()/2),
                       20));
    }
  else
    {
      gc.draw(logo, 
              Vector2i((gc.get_width()/2) - (logo.get_width()/2),
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

  if (0) // display hint
    {
      gc.print_center(Fonts::pingus_small, Display::get_width() / 2,
                      Display::get_height() - Fonts::pingus_small.get_height(),
                      hint);
    }
}

void
PingusMenu::on_click(MenuButton* button)
{
  if (button == start_button)
    {
      do_start("worldmaps/tutorial.worldmap");
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
      ScreenManager::instance()->push_screen(new LevelMenu());
    }
}

void
PingusMenu::set_hint(const std::string& str)
{
  hint = str;
}

void
PingusMenu::update(float delta)
{
  background.update(delta);
}

/* EOF */
