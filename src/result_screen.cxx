//  $Id: result_screen.cxx,v 1.13 2003/04/09 21:57:24 grumbel Exp $
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
#include "my_gettext.hxx"
#include "gui/surface_button.hxx"
#include "gui/gui_manager.hxx"
#include "gui/screen_manager.hxx"
#include "res_descriptor.hxx"
#include "pingus_resource.hxx"
#include "fonts.hxx"
#include "plf.hxx"
#include "string_converter.hxx"
#include "game_session.hxx"
#include "system.hxx"
#include "sound/sound.hxx"
#include "game_time.hxx"
#include "result_screen.hxx"

class ResultScreenComponent : public GUI::Component
{
public:
  Result result;
  CL_Surface background;
  std::string time_str;

  std::vector<CL_Surface> chalk_pingus;

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
    : GUI::SurfaceButton(625, 425, 
                         ResDescriptor("start/ok", "core", ResDescriptor::RD_RESOURCE),
                         ResDescriptor("start/ok_clicked", "core", ResDescriptor::RD_RESOURCE),
                         ResDescriptor("start/ok_hover", "core", ResDescriptor::RD_RESOURCE)),
      parent(p)
  {
  }

  void on_pointer_enter ()
  {
    SurfaceButton::on_pointer_enter();
    PingusSound::play_sound("tick");
  }

  void on_click() {
    parent->close_screen();
    PingusSound::play_sound("yipee");
  }
};

class ResultScreenAbortButton
  : public GUI::SurfaceButton
{
private:
  ResultScreen* parent;
public:
  ResultScreenAbortButton(ResultScreen* p)
    : GUI::SurfaceButton(122, 444, 
                         ResDescriptor("start/back", "core", ResDescriptor::RD_RESOURCE),
                         ResDescriptor("start/back_clicked", "core", ResDescriptor::RD_RESOURCE),
                         ResDescriptor("start/back_hover", "core", ResDescriptor::RD_RESOURCE)),
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
    PingusSound::play_sound ("tick");
  }
};

class ResultScreenRetryButton 
  : public GUI::SurfaceButton
{
private:
  ResultScreen* parent;
public:
  ResultScreenRetryButton(ResultScreen* p)
    : GUI::SurfaceButton(625, 425, 
                         ResDescriptor("start/ok", "core", ResDescriptor::RD_RESOURCE),
                         ResDescriptor("start/ok_clicked", "core", ResDescriptor::RD_RESOURCE),
                         ResDescriptor("start/ok_hover", "core", ResDescriptor::RD_RESOURCE)),
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
    PingusSound::play_sound ("tick");
  }
};

ResultScreenComponent::ResultScreenComponent(Result arg_result)
  : result(arg_result)
{
  background = PingusResource::load_surface("menu/startscreenbg", "core");

  chalk_pingus.push_back(PingusResource::load_surface("misc/chalk_pingu1", "core"));
  chalk_pingus.push_back(PingusResource::load_surface("misc/chalk_pingu2", "core"));
  chalk_pingus.push_back(PingusResource::load_surface("misc/chalk_pingu3", "core"));
  chalk_pingus.push_back(PingusResource::load_surface("misc/chalk_pingu4", "core"));
  
  if (result.max_time == -1)
    time_str = "-";
  else
    time_str = GameTime::ticks_to_realtime_string(result.max_time - result.used_time);
}

void
ResultScreenComponent::draw(GraphicContext& gc) 
{
  gc.draw(background, 0, 0);

  if (!result.success())
    gc.print_right(Fonts::chalk_normal, 675, 410, _("Retry"));

  gc.print_center(Fonts::chalk_large, gc.get_width()/2, 100, 
                  System::translate(result.plf->get_levelname()));

  if (result.success())
    {
      gc.print_center(Fonts::chalk_large, gc.get_width()/2, 160, _("Success!"));
      /*gc.print_center(Fonts::pingus_small, gc.get_width()/2, gc.get_height()-30,
        "..:: Press Space to continue ::..");*/
    }
  else
    {
      gc.print_center(Fonts::chalk_large, gc.get_width()/2, 160, _("Failure!"));
      /*gc.print_center(Fonts::pingus_normal, gc.get_width()/2, gc.get_height()-30,
                      "..:: Press Space to retry the level ::..");*/
    }
  
  std::string message;
  if (result.success())
    {
      if (result.killed == 0 && result.saved == result.total)
        message = _("Perfect! You saved everybody possibly, great!");
      else if (result.killed == 0)
        message = _("None got killed, pretty good work");
      else if (result.saved == result.needed)
        message = _("You saved just what you needed, you made it, but\n"
                    "maybe you can do better?");
      else if (result.killed >= 5)
        message = _("Not everybody could get saved, but still good work!");
      else
        message = _("What can I say, you made it, congratulations!");
    }
  else
    {
      if (result.killed == result.total)
        message = _("You killed everybody, not good.");
      else if (result.saved == 0)
        message = _("None got saved, I know you can do better.");
      else if (result.saved > 0)
        message = _("You didn't saved enough, but still saved a few, next\n"
                    "time you might be better.");
      else if (result.saved + 1 >= result.needed)
        message = _("Only one more and you would have made it, try again!");
      else if (result.saved + 5 >= result.needed)
        message = _("Only a handfull more and you would have made it, try again!");
      else
        message = _("Better luck next time!");
    }
  gc.print_center(Fonts::chalk_normal, gc.get_width()/2, 230, message);


#if 0
  for (int i = 0; i < result.saved; ++i)
    {
      gc.draw(chalk_pingus[rand() % chalk_pingus.size()], 230 + i * 15, 210);
    }
#endif
  gc.print_left(Fonts::chalk_normal,  300, 310, _("Saved: "));
  gc.print_right(Fonts::chalk_normal, 500, 310, to_string(result.saved) 
                 + "/" + to_string(result.needed));;

  gc.print_left(Fonts::chalk_normal,  300, 340, _("Killed: "));
  gc.print_right(Fonts::chalk_normal, 500, 340, to_string(result.killed));


  gc.print_left(Fonts::chalk_normal,   300, 370, _("Time left: "));
  gc.print_right(Fonts::chalk_normal, 500, 370, time_str);
}

ResultScreen::ResultScreen(Result arg_result)
  : result(arg_result)
{
  ResDescriptor ok_desc("result/ok", "core", ResDescriptor::RD_RESOURCE);
  ResDescriptor cancel_desc("result/retry", "core", ResDescriptor::RD_RESOURCE);

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
      PingusSound::play_music("success_1.it");
    }
  else
    {
      PingusSound::play_music("pingus-2.it");
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

/* EOF */
