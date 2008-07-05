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
#include "gettext.h"
#include "display/display.hpp"
#include "gui/surface_button.hpp"
#include "gui/gui_manager.hpp"
#include "screen/screen_manager.hpp"
#include "blitter.hpp"
#include "res_descriptor.hpp"
#include "resource.hpp"
#include "fonts.hpp"
#include "game_session.hpp"
#include "system.hpp"
#include "string_util.hpp"
#include "sound/sound.hpp"
#include "game_time.hpp"
#include "result_screen.hpp"


class ResultScreenComponent : public GUI::Component
{
public:
  Result result;
  Sprite background;
  std::string time_str;

  std::vector<Sprite> chalk_pingus;

  ResultScreenComponent(Result arg_result);
  virtual ~ResultScreenComponent() {}
  void draw(DrawingContext& gc) ;
};

class ResultScreenOkButton
  : public GUI::SurfaceButton
{
private:
  ResultScreen* parent;
public:
  ResultScreenOkButton(ResultScreen* p)
    : GUI::SurfaceButton(Display::get_width()/2 + 225,
                         Display::get_height()/2 + 125,
                         ResDescriptor("core/start/ok"),
                         ResDescriptor("core/start/ok_clicked"),
                         ResDescriptor("core/start/ok_hover")),
      parent(p)
  {
  }

  void on_pointer_enter ()
  {
    SurfaceButton::on_pointer_enter();
    Sound::PingusSound::play_sound("tick");
  }

  void on_click() {
    parent->close_screen();
    Sound::PingusSound::play_sound("yipee");
  }
};

class ResultScreenAbortButton
  : public GUI::SurfaceButton
{
private:
  ResultScreen* parent;
public:
  ResultScreenAbortButton(ResultScreen* p)
    : GUI::SurfaceButton(Display::get_width()/2 - 278,
                         Display::get_height()/2 + 144,
                         ResDescriptor("core/start/back"),
                         ResDescriptor("core/start/back_clicked"),
                         ResDescriptor("core/start/back_hover")),
      parent(p)
  {
  }

  void draw(DrawingContext& gc) {
    SurfaceButton::draw(gc);
    gc.print_center(Fonts::chalk_normal, x_pos + 55, y_pos - 4, _("Abort"));
  }

  void on_click() {
    parent->close_screen();
  }


  void on_pointer_enter()
  {
    SurfaceButton::on_pointer_enter();
    Sound::PingusSound::play_sound ("tick");
  }
};

class ResultScreenRetryButton
  : public GUI::SurfaceButton
{
private:
  ResultScreen* parent;
public:
  ResultScreenRetryButton(ResultScreen* p)
    : GUI::SurfaceButton(Display::get_width()/2 + 225,
                         Display::get_height()/2 + 125,
                         ResDescriptor("core/start/ok"),
                         ResDescriptor("core/start/ok_clicked"),
                         ResDescriptor("core/start/ok_hover")),
      parent(p)
  {
  }

  void draw (DrawingContext& gc) {
    SurfaceButton::draw(gc);
    gc.print_center(Fonts::chalk_normal, x_pos + 30, y_pos - 24, _("Retry"));
  }

  bool is_at(int x, int y) {
	  return x > x_pos && x < x_pos + int(button_surface.get_width())
		&& y > y_pos - 24 &&
		y < y_pos + int(button_surface.get_height());
  }

  void on_click()
  {
    parent->retry_level();
  }

  void on_pointer_enter()
  {
    SurfaceButton::on_pointer_enter();
    Sound::PingusSound::play_sound ("tick");
  }
};

ResultScreenComponent::ResultScreenComponent(Result arg_result)
  : result(arg_result)
{
  background = Sprite("core/menu/startscreenbg");
  background.scale(Display::get_width(), Display::get_height());
  
  chalk_pingus.push_back(Sprite("core/misc/chalk_pingu1"));
  chalk_pingus.push_back(Sprite("core/misc/chalk_pingu2"));
  chalk_pingus.push_back(Sprite("core/misc/chalk_pingu3"));
  chalk_pingus.push_back(Sprite("core/misc/chalk_pingu4"));

  if (result.max_time == -1)
    time_str = "-";
  else
    time_str = GameTime::ticks_to_realtime_string(result.max_time - result.used_time);
}

void
ResultScreenComponent::draw(DrawingContext& gc)
{
  gc.draw(background, Vector2i(gc.get_width()/2, gc.get_height()/2));

  gc.print_center(Fonts::chalk_large, gc.get_width()/2, 
                  Display::get_height()/2 - 200,
                  _(result.plf.get_levelname()));

  if (result.success())
    {
      gc.print_center(Fonts::chalk_large, gc.get_width()/2,
                      Display::get_height()/2 - 140, _("Success!"));
      /*gc.print_center(Fonts::pingus_small, gc.get_width()/2, gc.get_height()-30,
        "..:: Press Space to continue ::..");*/
    }
  else
    {
      gc.print_center(Fonts::chalk_large, gc.get_width()/2, Display::get_height()/2 - 140,
                      _("Failure!"));
      /*gc.print_center(Fonts::pingus_normal, gc.get_width()/2, gc.get_height()-30,
        "..:: Press Space to retry the level ::..");*/
    }

  std::string message;
  if (result.success())
    {
      if (result.killed == 0 && result.saved == result.total)
        message = _("Perfect! You saved everyone possible - great!");
      else if (result.killed == 0)
        message = _("No-one got killed, pretty good work.");
      else if (result.saved == result.needed)
        message = _("You saved exactly what you needed - you made it, but\n"
                    "maybe you can do better?");
      else if (result.killed >= 5)
        message = _("Not everybody was saved, but still good work!");
      else
        message = _("What can I say, you made it - congratulations!");
    }
  else
    {
      if (result.killed == result.total)
        message = _("You killed everybody, not good.");
      else if (result.saved == 0)
        message = _("No-one got saved - I know you can do better.");
      else if (result.saved > 0)
        message = _("You didn't save enough, but you saved a few.  Next\n"
                    "time you might do better.");
      else if (result.saved + 1 >= result.needed)
        message = _("Only one more and you would have made it - try again!");
      else if (result.saved + 5 >= result.needed)
        message = _("Only a handful more and you would have made it - try again!");
      else
        message = _("Better luck next time!");
    }
  gc.print_center(Fonts::chalk_normal, gc.get_width()/2,
                  Display::get_height()/2 - 70, message);


#if 0
  for (int i = 0; i < result.saved; ++i)
    {
      gc.draw(chalk_pingus[rand() % chalk_pingus.size()], 230 + i * 15, 210);
    }
#endif

  int left_x  = Display::get_width()/2 - 100;
  int right_x = Display::get_width()/2 + 100;
  int y = Display::get_height()/2 + 10;

  gc.print_left(Fonts::chalk_normal,  left_x,  y, _("Saved: "));
  gc.print_right(Fonts::chalk_normal, right_x, y, StringUtil::to_string(result.saved)
                 + "/" + StringUtil::to_string(result.needed));;

  gc.print_left(Fonts::chalk_normal,  left_x,  (y+=30), _("Died: "));
  gc.print_right(Fonts::chalk_normal, right_x, y, StringUtil::to_string(result.killed));


  gc.print_left(Fonts::chalk_normal,  left_x, (y+=30), _("Time left: "));
  gc.print_right(Fonts::chalk_normal, right_x, y, time_str);
}

ResultScreen::ResultScreen(Result arg_result)
  : result(arg_result)
{
  ResDescriptor ok_desc("core/result/ok");
  ResDescriptor cancel_desc("core/result/retry");

  ResultScreenComponent* comp = new ResultScreenComponent(result);
  gui_manager->add(comp, true);

  if (result.success())
    {
      gui_manager->add(new ResultScreenOkButton(this), true);
    }
  else
    {
      gui_manager->add(new ResultScreenAbortButton(this), true);
      gui_manager->add(new ResultScreenRetryButton(this), true);
    }

  //gui_manager->add(new GUI::SurfaceButton(500, 500, cancel_desc, cancel_desc, cancel_desc), true);
}

void
ResultScreen::on_startup()
{
  if (result.success())
    {
      Sound::PingusSound::play_music("success_1.it", 1.f, false);
    }
  else
    {
      Sound::PingusSound::play_music("pingus-2.it", 1.f, false);
    }
}

void
ResultScreen::retry_level()
{
  ScreenManager::instance()->replace_screen(new GameSession (result.plf, true));
}

void
ResultScreen::close_screen()
{
  ScreenManager::instance()->pop_screen();
}

void
ResultScreen::on_fast_forward_press()
{
  on_pause_press();
}

void
ResultScreen::on_pause_press()
{
  if (result.success())
    close_screen();
  else
    retry_level();
}

void
ResultScreen::on_escape_press()
{
  close_screen();
}


/* EOF */
