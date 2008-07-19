//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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
#include "gui/gui_manager.hpp"
#include "gui/surface_button.hpp"
#include "gui/component.hpp"
#include "screen/screen_manager.hpp"
#include "blitter.hpp"
#include "gettext.h"
#include "game_session.hpp"
#include "globals.hpp"
#include "system.hpp"
#include "fonts.hpp"
#include "resource.hpp"
#include "start_screen.hpp"
#include "game_time.hpp"
#include "sound/sound.hpp"
#include "pingus_level.hpp"
#include "string_format.hpp"
#include "display/display.hpp"
#include "string_util.hpp"

class StartScreenComponent : public GUI::Component
{
private:
  PingusLevel plf;
  Sprite background;
  Sprite blackboard;
  std::string time_str;
  std::string description;

public:
  StartScreenComponent(const PingusLevel& plf);
  void draw(DrawingContext& gc);
  virtual ~StartScreenComponent() {}

private:
  const std::string& format_description(int length);
};

class StartScreenOkButton : public GUI::SurfaceButton
{
private:
  StartScreen* parent;
public:
  StartScreenOkButton(StartScreen* p)
    : GUI::SurfaceButton(Display::get_width()/2 + 225,
                         Display::get_height()/2 + 125,
                         "core/start/ok",
                         "core/start/ok_clicked",
                         "core/start/ok_hover"),
      parent(p)
  {
  }

  void draw(DrawingContext& gc) {
    SurfaceButton::draw(gc);
    gc.print_center(Fonts::chalk_normal, Vector2i(x_pos + 30, y_pos - 20), _("Ok"));
  }

  bool is_at(int x, int y) {
	  return x > x_pos && x < x_pos + int(button_surface.get_width())
		&& y > y_pos - 20 && y < y_pos + int(button_surface.get_height());
  }

  void on_click()
  {
    Sound::PingusSound::play_sound("yipee");
    parent->start_game();
  }


  void on_pointer_enter()
  {
    SurfaceButton::on_pointer_enter();
    Sound::PingusSound::play_sound ("tick");
  }
};

class StartScreenAbortButton
  : public GUI::SurfaceButton
{
private:
  StartScreen* parent;

public:
  StartScreenAbortButton(StartScreen* p)
    : GUI::SurfaceButton(Display::get_width()/2 - 300,
                         Display::get_height()/2 + 144,
                         "core/start/back",
                         "core/start/back_clicked",
                         "core/start/back_hover"),
      parent(p)
  {
  }

  void draw(DrawingContext& gc) {
    SurfaceButton::draw(gc);
    gc.print_center(Fonts::chalk_normal, Vector2i(x_pos + 55, y_pos), _("Abort"));
  }

  void on_click() {
    parent->cancel_game();
  }

  void on_pointer_enter()
  {
    SurfaceButton::on_pointer_enter();
    Sound::PingusSound::play_sound ("tick");
  }
};

StartScreenComponent::StartScreenComponent(const PingusLevel& p)
  : plf(p),
    background("core/menu/wood"),
    blackboard("core/menu/blackboard")
{
  time_str = GameTime::ticks_to_realtime_string(plf.get_time());
}

void
StartScreenComponent::draw(DrawingContext& gc)
{
  // Paint the background wood panel
  for(int y = 0; y < gc.get_height(); y += background.get_height())
    for(int x = 0; x < gc.get_width(); x += background.get_width())
      gc.draw(background, x, y);

  gc.draw(blackboard, gc.get_width()/2, gc.get_height()/2);

  int left_x  = gc.get_width()/2 - 150;
  int right_x = gc.get_width()/2 + 150;
  int y = gc.get_height()/2 + 40;

  gc.print_center(Fonts::chalk_large,
                  Vector2i(gc.get_width() /2,
                           gc.get_height()/2 - 230),
                  _(plf.get_levelname()));

  gc.print_left(Fonts::chalk_normal,
                Vector2i(gc.get_width() /2 - 300,
                         gc.get_height()/2 - 170),
                format_description(800 - 200));

  y += 32;
  y += 30;

  gc.print_left (Fonts::chalk_normal, Vector2i(left_x,  y), _("Number of Pingus: "));
  gc.print_right(Fonts::chalk_normal, Vector2i(right_x, y), StringUtil::to_string(plf.get_number_of_pingus()));
  
  gc.print_left (Fonts::chalk_normal, Vector2i(left_x,  (y += 30)), _("Number to Save: "));
  gc.print_right(Fonts::chalk_normal, Vector2i(right_x, y), StringUtil::to_string(plf.get_number_to_save()));
  
  gc.print_left (Fonts::chalk_normal, Vector2i(left_x,  (y += 30)), _("Time: "));
  gc.print_right(Fonts::chalk_normal, Vector2i(right_x, y), time_str);

  //gc.print_left (Fonts::chalk_normal, left_x,  (y += 30), _("Difficulty:"));
  //gc.print_right(Fonts::chalk_normal, right_x, y, StringUtil::to_string(plf.get_difficulty()) + "/100");

  gc.print_center(Fonts::chalk_small, 
                  Vector2i(gc.get_width()/2,
                           gc.get_height()/2 + 215),
                  _("Author: ") + plf.get_author());

  if (maintainer_mode)
    gc.print_left(Fonts::chalk_small, Vector2i(110, 430), _("Filename: ") + plf.get_resname());
}

const std::string&
StartScreenComponent::format_description(int length)
{
  if (description != "")
    return description;

  description = _(plf.get_description());

  if (description == "")
    return description;

  description = StringFormat::break_line(description, length, Fonts::chalk_normal);

  return description;
}

StartScreen::StartScreen(const PingusLevel& arg_plf)
  : plf(arg_plf)
{
  StartScreenComponent* comp = new StartScreenComponent(plf);
  gui_manager->add(comp);
  gui_manager->add(ok_button = new StartScreenOkButton(this));
  gui_manager->add(abort_button = new StartScreenAbortButton(this));
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
StartScreen::on_pause_press ()
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
  GameSession* game_session = new GameSession(plf, true);
  ScreenManager::instance()->replace_screen(game_session);
}

void
StartScreen::cancel_game()
{
  ScreenManager::instance()->pop_screen();
}

void
StartScreen::resize(const Size& size)
{
  GUIScreen::resize(size);

  abort_button->set_pos(size.width/2 - 300, size.height/2 + 144);
  ok_button   ->set_pos(size.width/2 + 225, size.height/2 + 125);
}

/* EOF */
