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

#include "pingus/screens/result_screen.hpp"

#include <strut/to_string.hpp>

#include "engine/display/display.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/gui/surface_button.hpp"
#include "engine/screen/screen_manager.hpp"
#include "engine/sound/sound.hpp"
#include "pingus/fonts.hpp"
#include "pingus/game_time.hpp"
#include "pingus/gettext.h"
#include "pingus/screens/game_session.hpp"

namespace pingus {

class ResultScreenComponent : public pingus::gui::Component
{
public:
  Result result;
  Sprite background;
  Sprite blackboard;
  std::string time_str;

  ResultScreenComponent(Result const& arg_result);
  ~ResultScreenComponent() override {}
  void draw(DrawingContext& gc) override;
};

class ResultScreenOkButton
  : public pingus::gui::SurfaceButton
{
private:
  ResultScreen* parent;
public:
  ResultScreenOkButton(ResultScreen* p, int x, int y)
    : pingus::gui::SurfaceButton(x, y,
                         "core/start/ok",
                         "core/start/ok_clicked",
                         "core/start/ok_hover"),
      parent(p)
  {
  }

  void on_pointer_enter() override
  {
    SurfaceButton::on_pointer_enter();
    pingus::sound::PingusSound::play_sound("tick");
  }

  void on_click() override
  {
    parent->close_screen();
    pingus::sound::PingusSound::play_sound("yipee");
  }

private:
  ResultScreenOkButton(ResultScreenOkButton const&);
  ResultScreenOkButton & operator=(ResultScreenOkButton const&);
};

class ResultScreenAbortButton
  : public pingus::gui::SurfaceButton
{
private:
  ResultScreen* parent;
public:
  ResultScreenAbortButton(ResultScreen* p, int x, int y)
    : pingus::gui::SurfaceButton(x, y,
                         "core/start/back",
                         "core/start/back_clicked",
                         "core/start/back_hover"),
      parent(p)
  {
  }

  void draw(DrawingContext& gc) override
  {
    SurfaceButton::draw(gc);
    gc.print_center(pingus::fonts::chalk_normal, Vector2i(x_pos + 55, y_pos - 4), _("Give up"));
  }

  void on_click() override
  {
    parent->close_screen();
  }

  void on_pointer_enter() override
  {
    SurfaceButton::on_pointer_enter();
    pingus::sound::PingusSound::play_sound ("tick");
  }

private:
  ResultScreenAbortButton(ResultScreenAbortButton const&);
  ResultScreenAbortButton & operator=(ResultScreenAbortButton const&);
};

class ResultScreenRetryButton : public pingus::gui::SurfaceButton
{
private:
  ResultScreen* parent;

public:
  ResultScreenRetryButton(ResultScreen* p, int x, int y)
    : pingus::gui::SurfaceButton(x, y,
                         "core/start/ok",
                         "core/start/ok_clicked",
                         "core/start/ok_hover"),
      parent(p)
  {
  }

  void draw (DrawingContext& gc) override
  {
    SurfaceButton::draw(gc);
    gc.print_center(pingus::fonts::chalk_normal, Vector2i(x_pos + 30, y_pos - 24), _("Retry"));
  }

  bool is_at(int x, int y) override
  {
    return x > x_pos && x < x_pos + button_surface.get_width()
      && y > y_pos - 24 &&
      y < y_pos + button_surface.get_height();
  }

  void on_click() override
  {
    parent->retry_level();
  }

  void on_pointer_enter() override
  {
    SurfaceButton::on_pointer_enter();
    pingus::sound::PingusSound::play_sound ("tick");
  }

private:
  ResultScreenRetryButton(ResultScreenRetryButton const&);
  ResultScreenRetryButton & operator=(ResultScreenRetryButton const&);
};

ResultScreenComponent::ResultScreenComponent(Result const& arg_result) :
  result(arg_result),
  background("core/menu/wood"),
  blackboard("core/menu/blackboard"),
  time_str()
{
  if (result.max_time == -1)
    time_str = "-";
  else
    time_str = GameTime::ticks_to_realtime_string(std::max(0, result.max_time - result.used_time));
}

void
ResultScreenComponent::draw(DrawingContext& gc)
{
  // Paint the background wood panel
  for(int y = 0; y < gc.get_height(); y += background.get_height())
    for(int x = 0; x < gc.get_width(); x += background.get_width())
      gc.draw(background, Vector2i(x, y));

  gc.draw(blackboard, Vector2i(gc.get_width()/2, gc.get_height()/2));

  gc.print_center(pingus::fonts::chalk_large,
                  Vector2i(gc.get_width()/2,
                           Display::get_height()/2 - 200),
                  _(result.plf.get_levelname()));

  if (result.success())
  {
    gc.print_center(pingus::fonts::chalk_large,
                    Vector2i(gc.get_width()/2,
                             Display::get_height()/2 - 140),
                    _("Success!"));
    /*gc.print_center(pingus::fonts::pingus_small, gc.get_width()/2, gc.get_height()-30,
      "..:: Press Space to continue ::..");*/
  }
  else
  {
    gc.print_center(pingus::fonts::chalk_large,
                    Vector2i(gc.get_width()/2, Display::get_height()/2 - 140),
                    _("Failure!"));
    /*gc.print_center(pingus::fonts::pingus_normal, gc.get_width()/2, gc.get_height()-30,
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
  gc.print_center(pingus::fonts::chalk_normal, Vector2i(gc.get_width()/2, gc.get_height()/2 - 70), message);

  int left_x  = gc.get_width()/2 - 100;
  int right_x = gc.get_width()/2 + 100;
  int y = Display::get_height()/2 + 10;

  gc.print_left(pingus::fonts::chalk_normal,  Vector2i(left_x,  y), _("Saved: "));
  gc.print_right(pingus::fonts::chalk_normal, Vector2i(right_x, y), strut::to_string(result.saved)
                 + "/" + strut::to_string(result.needed));

  gc.print_left(pingus::fonts::chalk_normal,  Vector2i(left_x,  (y+=30)), _("Died: "));
  gc.print_right(pingus::fonts::chalk_normal, Vector2i(right_x, y), strut::to_string(result.killed));

  gc.print_left(pingus::fonts::chalk_normal,  Vector2i(left_x, (y+=30)), _("Time left: "));
  gc.print_right(pingus::fonts::chalk_normal, Vector2i(right_x, y), time_str);
}

ResultScreen::ResultScreen(Result const& arg_result) :
  result(arg_result),
  ok_button(),
  abort_button(),
  retry_button()
{
  gui_manager->create<ResultScreenComponent>(result);

  ok_button = abort_button = retry_button = nullptr;

  if (result.success())
  {
    ok_button = gui_manager->create<ResultScreenOkButton>(this,
                                                          Display::get_width()/2 + 245,
                                                          Display::get_height()/2 + 150);
  }
  else
  {
    abort_button = gui_manager->create<ResultScreenAbortButton>(this,
                                                                Display::get_width()/2 - 300,
                                                                Display::get_height()/2 + 200);
    retry_button = gui_manager->create<ResultScreenRetryButton>(this,
                                                                Display::get_width()/2 + 245,
                                                                Display::get_height()/2 + 150);
  }

  //gui_manager->add(new pingus::gui::SurfaceButton(500, 500, cancel_desc, cancel_desc, cancel_desc), true);
}

void
ResultScreen::on_startup()
{
  if (result.success())
  {
    pingus::sound::PingusSound::play_music("success_1.it", 1.f, false);
  }
  else
  {
    pingus::sound::PingusSound::play_music("pingus-2.it", 1.f, false);
  }
}

void
ResultScreen::retry_level()
{
  ScreenManager::instance()->replace_screen(std::make_shared<GameSession>(result.plf, true));
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

void
ResultScreen::resize(Size const& size_)
{
  GUIScreen::resize(size_);

  if (ok_button)
    ok_button->set_pos(size.width()/2 + 245, size.height()/2 + 150);

  if (abort_button)
    abort_button->set_pos(size.width()/2 - 300, size.height()/2 + 200);

  if (retry_button)
    retry_button->set_pos(size.width()/2 + 245, size.height()/2 + 150);
}

} // namespace pingus

/* EOF */
