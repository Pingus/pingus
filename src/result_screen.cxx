//  $Id: result_screen.cxx,v 1.20 2003/10/21 21:37:06 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include <iostream>
#include "gettext.h"
#include "sprite.hxx"
#include "gui/surface_button.hxx"
#include "gui/gui_manager.hxx"
#include "gui/screen_manager.hxx"
#include "res_descriptor.hxx"
#include "resource.hxx"
#include "fonts.hxx"
#include "plf.hxx"
#include "string_converter.hxx"
#include "game_session.hxx"
#include "system.hxx"
#include "sound/sound.hxx"
#include "game_time.hxx"
#include "result_screen.hxx"

namespace Pingus {

class ResultScreenComponent : public GUI::Component
{
public:
  Result result;
  Sprite background;
  std::string time_str;

  std::vector<CL_Sprite> chalk_pingus;

  ResultScreenComponent(Result arg_result);
  virtual ~ResultScreenComponent() {}
  void draw(GraphicContext& gc) ;
};

class ResultScreenOkButton
  : public GUI::SurfaceButton
{
private:
  ResultScreen* parent;
public:
  ResultScreenOkButton(ResultScreen* p)
    : GUI::SurfaceButton(CL_Display::get_width()/2 + 225,
                         CL_Display::get_height()/2 + 125,
                         ResDescriptor("start/ok", "core"),
                         ResDescriptor("start/ok_clicked", "core"),
                         ResDescriptor("start/ok_hover", "core")),
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
    : GUI::SurfaceButton(CL_Display::get_width()/2 - 278,
                         CL_Display::get_height()/2 + 144,
                         ResDescriptor("start/back", "core"),
                         ResDescriptor("start/back_clicked", "core"),
                         ResDescriptor("start/back_hover", "core")),
      parent(p)
  {
  }

  void draw(GraphicContext& gc) {
    SurfaceButton::draw(gc);
    gc.print_center(Fonts::chalk_normal, x_pos + 55, y_pos, _("Abort"));
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
    : GUI::SurfaceButton(CL_Display::get_width()/2 + 225,
                         CL_Display::get_height()/2 + 125,
                         ResDescriptor("start/ok", "core"),
                         ResDescriptor("start/ok_clicked", "core"),
                         ResDescriptor("start/ok_hover", "core")),
      parent(p)
  {
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
  background = Sprite("menu/startscreenbg", "core");
  background.set_align_center();

  chalk_pingus.push_back(Resource::load_sprite("misc/chalk_pingu1", "core"));
  chalk_pingus.push_back(Resource::load_sprite("misc/chalk_pingu2", "core"));
  chalk_pingus.push_back(Resource::load_sprite("misc/chalk_pingu3", "core"));
  chalk_pingus.push_back(Resource::load_sprite("misc/chalk_pingu4", "core"));

  if (result.max_time == -1)
    time_str = "-";
  else
    time_str = GameTime::ticks_to_realtime_string(result.max_time - result.used_time);
}

void
ResultScreenComponent::draw(GraphicContext& gc)
{
  gc.draw(background, Vector(gc.get_width()/2, gc.get_height()/2));

  if (!result.success())
    gc.print_right(Fonts::chalk_normal,
                   CL_Display::get_width()/2 + 275,
                   CL_Display::get_height()/2 + 110, _("Retry"));

  gc.print_center(Fonts::chalk_large, gc.get_width()/2, CL_Display::get_height()/2 - 200,
                  System::translate(result.plf->get_levelname()));

  if (result.success())
    {
      gc.print_center(Fonts::chalk_large, gc.get_width()/2,
                      CL_Display::get_height()/2 - 140, _("Success!"));
      /*gc.print_center(Fonts::pingus_small, gc.get_width()/2, gc.get_height()-30,
        "..:: Press Space to continue ::..");*/
    }
  else
    {
      gc.print_center(Fonts::chalk_large, gc.get_width()/2, CL_Display::get_height()/2 - 140,
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
                  CL_Display::get_height()/2 - 70, message);


#if 0
  for (int i = 0; i < result.saved; ++i)
    {
      gc.draw(chalk_pingus[rand() % chalk_pingus.size()], 230 + i * 15, 210);
    }
#endif
  int left_x  = CL_Display::get_width()/2 - 100;
  int right_x = CL_Display::get_width()/2 + 100;
  int y = CL_Display::get_height()/2 + 10;

  gc.print_left(Fonts::chalk_normal,  left_x,  y, _("Saved: "));
  gc.print_right(Fonts::chalk_normal, right_x, y, to_string(result.saved)
                 + "/" + to_string(result.needed));;

  gc.print_left(Fonts::chalk_normal,  left_x,  y+=30, _("Died: "));
  gc.print_right(Fonts::chalk_normal, right_x, y, to_string(result.killed));


  gc.print_left(Fonts::chalk_normal,   left_x, y+=30, _("Time left: "));
  gc.print_right(Fonts::chalk_normal, right_x, y, time_str);
}

ResultScreen::ResultScreen(Result arg_result)
  : result(arg_result)
{
  ResDescriptor ok_desc("result/ok", "core");
  ResDescriptor cancel_desc("result/retry", "core");

  ResultScreenComponent* comp = new ResultScreenComponent(result);
  gui_manager->add(comp);

  if (result.success())
    {
      gui_manager->add(new ResultScreenOkButton(this));
    }
  else
    {
      gui_manager->add(new ResultScreenAbortButton(this));
      gui_manager->add(new ResultScreenRetryButton(this));
    }

  //gui_manager->add(new GUI::SurfaceButton(500, 500, cancel_desc, cancel_desc, cancel_desc));
}

void
ResultScreen::on_startup()
{
  if (result.success())
    {
      Sound::PingusSound::play_music("success_1.it");
    }
  else
    {
      Sound::PingusSound::play_music("pingus-2.it");
    }
}

void
ResultScreen::retry_level()
{
  ScreenManager::instance()->replace_screen(new PingusGameSession (result.plf, true),
                                            true);
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

} // namespace Pingus

/* EOF */
