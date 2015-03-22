//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/screens/pingus_menu.hpp"

#include "editor/editor_screen.hpp"
#include "engine/screen/screen_manager.hpp"
#include "engine/sound/sound.hpp"
#include "pingus/components/menu_button.hpp"
#include "pingus/gettext.h"
#include "pingus/layer_manager.hpp"
#include "pingus/plf_res_mgr.hpp"
#include "pingus/resource.hpp"
#include "pingus/screens/credits.hpp"
#include "pingus/screens/level_menu.hpp"
#include "pingus/screens/option_menu.hpp"
#include "pingus/screens/start_screen.hpp"
#include "pingus/screens/story_screen.hpp"
#include "pingus/worldmap/worldmap_screen.hpp"
#include "pingus/stat_manager.hpp"

PingusMenu::PingusMenu() :
  is_init(),
  hint(),
  help(),
  text_scroll_offset(),
  background(),
  logo(),
  start_button(),
  quit_button(),
  editor_button(),
  contrib_button(),
  options_button()
{
  is_init = false;

  Size size_(Display::get_width(), Display::get_height());

  start_button = new MenuButton(this, Vector2i(size_.width/2 - 125,
                                               size_.height/2 - 20),
                                _("Story"),
                                _("..:: Start the game ::.."));

  editor_button = new MenuButton(this, Vector2i(size_.width/2 + 125,
                                                size_.height/2 - 20),
                                 _("Editor"),
                                 _("..:: Create your own levels ::.."));

  options_button = new MenuButton(this, Vector2i(size_.width/2 + 125,
                                                 size_.height/2 + 50),
                                  _("Options"),
                                  _("..:: Configure the game ::.."));

  contrib_button = new MenuButton(this, Vector2i(size_.width/2 - 125,
                                                 size_.height/2 + 50),
                                  _("Levelsets"),
                                  _("..:: Play User Built levels ::.."));

  quit_button = new MenuButton(this, Vector2i(size_.width/2,
                                              size_.height/2 + 120),
                               _("Exit"),
                               _("..:: Bye, bye ::.."));

  gui_manager->add(quit_button);
  gui_manager->add(options_button);
  gui_manager->add(contrib_button);
  gui_manager->add(start_button);
  gui_manager->add(editor_button);

  logo = Sprite("core/misc/logo");

  create_background(Size(Display::get_width(), Display::get_height()));

  help = _("..:: Ctrl-g: mouse grab   ::   F10: fps counter   ::   F11: fullscreen   ::   F12: screenshot ::..");

  Sound::PingusSound::play_music("pingus-1.it");
}

PingusMenu::~PingusMenu()
{
}

void
PingusMenu::show_credits()
{
  ScreenManager::instance()
    ->push_screen(std::make_shared<Credits>(Pathname("credits/pingus.credits", Pathname::DATA_PATH)));
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

  std::shared_ptr<WorldmapNS::WorldmapScreen> worldmap_screen = std::make_shared<WorldmapNS::WorldmapScreen>();
  worldmap_screen->load(Pathname(filename, Pathname::DATA_PATH));
  ScreenManager::instance()->push_screen(worldmap_screen);

  bool story_seen = false;
  StatManager::instance()->get_bool("tutorial-startstory-seen", story_seen); // FIXME: Hardcoding tutorial is evil
  if (!story_seen)
  {
    FileReader reader = FileReader::parse(Pathname("stories/tutorial_intro.story", Pathname::DATA_PATH));
    ScreenManager::instance()->push_screen(std::make_shared<StoryScreen>(reader));
    StatManager::instance()->set_bool("tutorial-startstory-seen", true);
  }
}

void PingusMenu::do_contrib(const std::string &levelfile)
{ // Launch the specified level - don't bother checking for it, it has to exist
  Sound::PingusSound::play_sound ("letsgo");
  ScreenManager::instance()->push_screen
    (std::make_shared<StartScreen>(PLFResMgr::load_plf_from_filename(Pathname(levelfile, Pathname::SYSTEM_PATH))));
}

void PingusMenu::do_edit()
{       // Launch the level editor
  Sound::PingusSound::stop_music();
  ScreenManager::instance()->push_screen(std::make_shared<Editor::EditorScreen>());
}

void
PingusMenu::on_escape_press ()
{
  //FIXME: get_manager()->show_exit_menu ();
}

void
PingusMenu::draw_background(DrawingContext& gc)
{
  background->draw(gc);

  gc.draw(logo, Vector2i((gc.get_width()/2) - (logo.get_width()/2),
                         gc.get_height()/2 - 280));

  gc.print_left(Fonts::pingus_small, Vector2i(gc.get_width()/2 - 400 + 25, gc.get_height()-140),
                "Pingus " VERSION " - Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmail.com>\n"
                "See the file AUTHORS for a complete list of contributors.\n"
                "Pingus comes with ABSOLUTELY NO WARRANTY. This is free software, and you are\n"
                "welcome to redistribute it under certain conditions; see the file COPYING for details.\n");

  gc.draw_fillrect(Rect(0,
                        Display::get_height () - 26,
                        Display::get_width (),
                        Display::get_height ()),
                   Color(0, 0, 0, 255));

  gc.print_center(Fonts::pingus_small,
                  Vector2i(gc.get_width() / 2,
                           gc.get_height() - Fonts::pingus_small.get_height() - 8),
                  help);

  if (0) // display hint
  {
    gc.print_center(Fonts::pingus_small,
                    Vector2i(gc.get_width() / 2,
                             gc.get_height() - Fonts::pingus_small.get_height()),
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
    ScreenManager::instance()->push_screen(std::make_shared<LevelMenu>());
  }
  else if (button == options_button)
  {
    ScreenManager::instance()->push_screen(std::make_shared<OptionMenu>());
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
  background->update(delta);
}

void
PingusMenu::create_background(const Size& size_)
{
  // Recreate the layer manager in the new size
  background = std::unique_ptr<LayerManager>(new LayerManager());

  Surface layer1 = Resource::load_surface("core/menu/layer1");
  Surface layer2 = Resource::load_surface("core/menu/layer2");
  Surface layer3 = Resource::load_surface("core/menu/layer3");
  Surface layer4 = Resource::load_surface("core/menu/layer4");
  Surface layer5 = Resource::load_surface("core/menu/layer5");

  int w = size_.width;
  int h = size_.height;

  // We only need to scale the background main menu images if the screen
  // resolution is not default
  if (w != globals::default_screen_width ||
      h != globals::default_screen_height)
  {
    layer1 = layer1.scale(w, 185 * h / globals::default_screen_height);
    layer2 = layer2.scale(w, 362 * h / globals::default_screen_height);
    layer3 = layer3.scale(w, 306 * h / globals::default_screen_height);
    layer4 = layer4.scale(w, 171 * h / globals::default_screen_height);
    layer5 = layer5.scale(302 * w / globals::default_screen_width, 104 * h / globals::default_screen_height);

    background->add_layer(Sprite(layer1), 0, 0, 12, 0);
    background->add_layer(Sprite(layer2), 0, 150 * static_cast<float>(h) / static_cast<float>(globals::default_screen_height), 25, 0);
    background->add_layer(Sprite(layer3), 0, 200 * static_cast<float>(h) / static_cast<float>(globals::default_screen_height), 50, 0);
    background->add_layer(Sprite(layer4), 0, 429 * static_cast<float>(h) / static_cast<float>(globals::default_screen_height), 100, 0);
    background->add_layer(Sprite(layer5), 0, 500 * static_cast<float>(h) / static_cast<float>(globals::default_screen_height), 200, 0);
  }
  else
  {
    background->add_layer(Sprite(layer1), 0, 0, 12, 0);
    background->add_layer(Sprite(layer2), 0, 150, 25, 0);
    background->add_layer(Sprite(layer3), 0, 200, 50, 0);
    background->add_layer(Sprite(layer4), 0, 429, 100, 0);
    background->add_layer(Sprite(layer5), 0, 500, 200, 0);
  }
}

void
PingusMenu::resize(const Size& size_)
{
  GUIScreen::resize(size_);
  create_background(size);

  start_button->set_pos(size.width/2 - 125,
                        size.height/2 - 20);

  editor_button->set_pos(size.width/2 + 125,
                         size.height/2 - 20);

  contrib_button->set_pos(size.width/2 - 125,
                          size.height/2 + 50);

  options_button->set_pos(size.width/2 + 125,
                          size.height/2 + 50);

  quit_button->set_pos(size.width/2,
                       size.height/2 + 120);
}

/* EOF */
