// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/screens/start_screen.hpp"

#include <strut/to_string.hpp>

#include "engine/display/display.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/gui/surface_button.hpp"
#include "engine/screen/screen_manager.hpp"
#include "engine/sound/sound.hpp"
#include "pingus/fonts.hpp"
#include "pingus/game_time.hpp"
#include "pingus/gettext.h"
#include "pingus/globals.hpp"
#include "pingus/screens/game_session.hpp"
#include "pingus/string_format.hpp"

namespace pingus {

class StartScreenComponent : public pingus::gui::Component
{
private:
  PingusLevel plf;
  Sprite background;
  Sprite blackboard;
  std::string time_str;
  std::string description;

public:
  StartScreenComponent(PingusLevel const& plf);
  void draw(DrawingContext& gc) override;
  ~StartScreenComponent() override {}

private:
  std::string const& format_description(int length);
};

class StartScreenOkButton : public pingus::gui::SurfaceButton
{
private:
  StartScreen* parent;
public:
  StartScreenOkButton(StartScreen* p)
    : pingus::gui::SurfaceButton(Display::get_width()/2 + 245,
                         Display::get_height()/2 + 150,
                         "core/start/ok",
                         "core/start/ok_clicked",
                         "core/start/ok_hover"),
      parent(p)
  {
  }

  void draw(DrawingContext& gc) override {
    SurfaceButton::draw(gc);
    gc.print_center(pingus::fonts::chalk_normal, Vector2i(x_pos + 30, y_pos - 20), _("Play"));
  }

  bool is_at(int x, int y) override {
    return x > x_pos && x < x_pos + button_surface.get_width()
      && y > y_pos - 20 && y < y_pos + button_surface.get_height();
  }

  void on_click() override
  {
    pingus::sound::PingusSound::play_sound("yipee");
    parent->start_game();
  }

  void on_pointer_enter() override
  {
    SurfaceButton::on_pointer_enter();
    pingus::sound::PingusSound::play_sound ("tick");
  }

private:
  StartScreenOkButton(StartScreenOkButton const&);
  StartScreenOkButton & operator=(StartScreenOkButton const&);
};

class StartScreenAbortButton
  : public pingus::gui::SurfaceButton
{
private:
  StartScreen* parent;

public:
  StartScreenAbortButton(StartScreen* p)
    : pingus::gui::SurfaceButton(Display::get_width()/2 - 300,
                         Display::get_height()/2 + 200,
                         "core/start/back",
                         "core/start/back_clicked",
                         "core/start/back_hover"),
      parent(p)
  {
  }

  void draw(DrawingContext& gc) override {
    SurfaceButton::draw(gc);
    gc.print_center(pingus::fonts::chalk_normal, Vector2i(x_pos + 55, y_pos), _("Back"));
  }

  void on_click() override {
    parent->cancel_game();
  }

  void on_pointer_enter() override
  {
    SurfaceButton::on_pointer_enter();
    pingus::sound::PingusSound::play_sound ("tick");
  }

private:
  StartScreenAbortButton(StartScreenAbortButton const&);
  StartScreenAbortButton & operator=(StartScreenAbortButton const&);
};

StartScreenComponent::StartScreenComponent(PingusLevel const& p) :
  plf(p),
  background("core/menu/wood"),
  blackboard("core/menu/blackboard"),
  time_str(),
  description()
{
  time_str = GameTime::ticks_to_realtime_string(plf.get_time());
}

void
StartScreenComponent::draw(DrawingContext& gc)
{
  // Paint the background wood panel
  for(int y = 0; y < gc.get_height(); y += background.get_height())
    for(int x = 0; x < gc.get_width(); x += background.get_width())
      gc.draw(background, Vector2i(x, y));

  gc.draw(blackboard, Vector2i(gc.get_width()/2, gc.get_height()/2));

  int left_x  = gc.get_width()/2 - 150;
  int right_x = gc.get_width()/2 + 150;
  int y = gc.get_height()/2 + 40;

  gc.print_center(pingus::fonts::chalk_large,
                  Vector2i(gc.get_width() /2,
                           gc.get_height()/2 - 230),
                  _(plf.get_levelname()));

  gc.print_left(pingus::fonts::chalk_normal,
                Vector2i(gc.get_width() /2 - 300,
                         gc.get_height()/2 - 170),
                format_description(800 - 200));

  y += 32;
  y += 45;

  gc.print_left (pingus::fonts::chalk_normal, Vector2i(left_x,  y), _("Number of Pingus: "));
  gc.print_right(pingus::fonts::chalk_normal, Vector2i(right_x, y), strut::to_string(plf.get_number_of_pingus()));

  gc.print_left (pingus::fonts::chalk_normal, Vector2i(left_x,  (y += 30)), _("Number to Save: "));
  gc.print_right(pingus::fonts::chalk_normal, Vector2i(right_x, y), strut::to_string(plf.get_number_to_save()));

  gc.print_left (pingus::fonts::chalk_normal, Vector2i(left_x,  (y += 30)), _("Time: "));
  gc.print_right(pingus::fonts::chalk_normal, Vector2i(right_x, y), time_str);

  gc.print_center(pingus::fonts::chalk_small,
                  Vector2i(gc.get_width()/2,
                           gc.get_height()/2 + 215),
                  _("Author: ") + plf.get_author());

  if (globals::developer_mode)
  {
    gc.print_center(pingus::fonts::chalk_small, Vector2i(gc.get_width()/2, gc.get_height()-50), plf.get_resname());
  }
}

std::string const&
StartScreenComponent::format_description(int length)
{
  if (!description.empty())
    return description;

  description = _(plf.get_description());

  if (description.empty())
    return description;

  description = StringFormat::break_line(description, length, pingus::fonts::chalk_normal);

  return description;
}

StartScreen::StartScreen(PingusLevel const& arg_plf) :
  plf(arg_plf),
  abort_button(),
  ok_button()
{
  gui_manager->create<StartScreenComponent>(plf);
  ok_button = gui_manager->create<StartScreenOkButton>(this);
  abort_button = gui_manager->create<StartScreenAbortButton>(this);
}

StartScreen::~StartScreen()
{

}

void
StartScreen::on_fast_forward_press()
{
  start_game();
}

void
StartScreen::on_pause_press()
{
  start_game();
}

void
StartScreen::on_escape_press()
{
  cancel_game();
}

void
StartScreen::start_game()
{
  ScreenManager::instance()->replace_screen(std::make_shared<GameSession>(plf, true));
}

void
StartScreen::cancel_game()
{
  ScreenManager::instance()->pop_screen();
}

void
StartScreen::resize(Size const& size_)
{
  GUIScreen::resize(size_);

  abort_button->set_pos(size.width() /2 - 300, size.height()/2 + 200);
  ok_button   ->set_pos(size.width()/2 + 245, size.height()/2 + 150);
}

} // namespace pingus

/* EOF */
