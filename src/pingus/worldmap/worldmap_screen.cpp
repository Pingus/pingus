// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/worldmap/worldmap_screen.hpp"

#include "engine/display/display.hpp"
#include "engine/gui/surface_button.hpp"
#include "engine/screen/screen_manager.hpp"
#include "engine/sound/sound.hpp"
#include "pingus/fonts.hpp"
#include "pingus/gettext.h"
#include "pingus/screens/story_screen.hpp"
#include "pingus/worldmap/pingus.hpp"
#include "pingus/worldmap/worldmap.hpp"
#include "pingus/worldmap/worldmap_component.hpp"

namespace WorldmapNS {

class WorldmapScreenCloseButton
  : public GUI::SurfaceButton
{
private:
  WorldmapScreen* worldmap_screen;
public:
  WorldmapScreenCloseButton(WorldmapScreen* worldmap_screen);
  void on_click();
  void draw (DrawingContext& gc);
  void on_pointer_enter();

private:
  WorldmapScreenCloseButton(const WorldmapScreenCloseButton&);
  WorldmapScreenCloseButton & operator=(const WorldmapScreenCloseButton&);
};

class WorldmapScreenCreditsButton
  : public GUI::SurfaceButton
{
private:
  WorldmapScreen* worldmap_screen;
public:
  WorldmapScreenCreditsButton(WorldmapScreen* worldmap_screen);
  void on_click();
  void draw (DrawingContext& gc);
  void on_pointer_enter();

private:
  WorldmapScreenCreditsButton(const WorldmapScreenCreditsButton&);
  WorldmapScreenCreditsButton & operator=(const WorldmapScreenCreditsButton&);
};

WorldmapScreenCreditsButton::WorldmapScreenCreditsButton(WorldmapScreen* worldmap_screen_) :
  GUI::SurfaceButton(Display::get_width() - 150, 0,
                     "core/worldmap/credits_button_normal",
                     "core/worldmap/credits_button_pressed",
                     "core/worldmap/credits_button_hover"),
  worldmap_screen(worldmap_screen_)
{
}

void
WorldmapScreenCreditsButton::on_pointer_enter()
{
  SurfaceButton::on_pointer_enter();
  Sound::PingusSound::play_sound ("tick");
}

void
WorldmapScreenCreditsButton::draw (DrawingContext& gc)
{
  SurfaceButton::draw(gc);
  gc.print_center(Fonts::chalk_small, Vector2i(Display::get_width() - 59 - 24, 2), _("Show Ending?"));
}

void
WorldmapScreenCreditsButton::on_click()
{
  worldmap_screen->show_end_story();
}

WorldmapScreenCloseButton::WorldmapScreenCloseButton(WorldmapScreen* worldmap_screen_) :
  GUI::SurfaceButton(0, Display::get_height() - 37,
                     "core/worldmap/leave_button_normal",
                     "core/worldmap/leave_button_pressed",
                     "core/worldmap/leave_button_hover"),
  worldmap_screen(worldmap_screen_)
{
}

void
WorldmapScreenCloseButton::on_pointer_enter()
{
  SurfaceButton::on_pointer_enter();
  Sound::PingusSound::play_sound ("tick");
}

void
WorldmapScreenCloseButton::draw (DrawingContext& gc)
{
  SurfaceButton::draw(gc);
  gc.print_center(Fonts::chalk_small, Vector2i(44, Display::get_height() - 25), _("Leave?"));
}

void
WorldmapScreenCloseButton::on_click()
{
  ScreenManager::instance ()->pop_screen ();
}

WorldmapScreen::WorldmapScreen() :
  is_init(false),
  exit_worldmap(false),
  worldmap(),
  new_worldmap(),
  close_button(),
  credits_button(),
  m_worldmap_component()
{
  // FIXME: a bit ugly because of the proteced member, but should work
  // FIXME: well enough. GUIScreen could also use multi-inheritage,
  // FIXME: but that could lead to member function name conflicts
  m_worldmap_component = gui_manager->create<WorldmapComponent>(this);
  close_button = gui_manager->create<WorldmapScreenCloseButton>(this);
}

WorldmapScreen::~WorldmapScreen ()
{
}

void
WorldmapScreen::load(const Pathname& filename)
{
  worldmap = std::unique_ptr<Worldmap>(new Worldmap(filename));

  bool credits_unlocked = false;
  //StatManager::instance()->get_bool(worldmap->get_short_name() + "-endstory-seen", credits_unlocked);
  if (credits_unlocked)
  {
    gui_manager->create<WorldmapScreenCreditsButton>(this);
  }
}

void
WorldmapScreen::on_startup ()
{
  exit_worldmap = false;
  Sound::PingusSound::stop_music();

  if (worldmap.get())
    worldmap->on_startup ();
}

void
WorldmapScreen::on_escape_press ()
{
  //log_info("WorldmapScreen::on_escape_press ()...");
  exit_worldmap = true;
}

void
WorldmapScreen::update (float delta)
{
  GUIScreen::update(delta);

  // Exit the word
  if (exit_worldmap)
    ScreenManager::instance ()->pop_screen();

  // Check if new worldmap is set and if so, change it
  if (new_worldmap.get())
  {
    worldmap = std::move(new_worldmap);
  }
}

void
WorldmapScreen::draw_foreground(DrawingContext& gc)
{
}

Rect
WorldmapScreen::get_trans_rect() const
{
  return Rect(Vector2i(Math::max((Display::get_width()  - worldmap->get_width())/2,  0),
                       Math::max((Display::get_height() - worldmap->get_height())/2, 0)),
              Size(Math::min(Display::get_width(),  worldmap->get_width()),
                   Math::min(Display::get_height(), worldmap->get_height())));
}

void
WorldmapScreen::show_intro_story()
{
  ScreenManager::instance()->push_screen(std::make_shared<StoryScreen>(worldmap->get_worldmap().get_intro_story().get_mapping()));
}

void
WorldmapScreen::show_end_story()
{
  ScreenManager::instance()->push_screen(std::make_shared<StoryScreen>(worldmap->get_worldmap().get_end_story().get_mapping()));
}

void
WorldmapScreen::resize(const Size& size_)
{
  GUIScreen::resize(size_);
  close_button->set_pos(0, size.height - 37);
}

void
WorldmapScreen::on_fast_forward_press()
{
  m_worldmap_component->on_fast_forward_press();
}

void
WorldmapScreen::on_fast_forward_release()
{
  m_worldmap_component->on_fast_forward_release();
}

} // namespace WorldmapNS

/* EOF */
