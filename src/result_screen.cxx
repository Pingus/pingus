//  $Id: result_screen.cxx,v 1.7 2003/03/30 15:34:57 grumbel Exp $
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
#include "result_screen.hxx"

class ResultScreenComponent : public GUI::Component
{
public:
  Result result;
  CL_Surface background;

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

  void on_click() {
    parent->close_screen();
  }
};

class ResultScreenRetryButton 
  : public GUI::SurfaceButton
{
private:
  ResultScreen* parent;
public:
  ResultScreenRetryButton(ResultScreen* p)
    : GUI::SurfaceButton(500, 500, 
                         ResDescriptor("result/retry", "core", ResDescriptor::RD_RESOURCE),
                         ResDescriptor("result/retry", "core", ResDescriptor::RD_RESOURCE),
                         ResDescriptor("result/retry", "core", ResDescriptor::RD_RESOURCE)),
      parent(p)
  {
  }

  void on_click() 
  {
    parent->retry_level();
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
}

void
ResultScreenComponent::draw(GraphicContext& gc) 
{
  gc.draw(background, 0, 0);
  gc.print_center(Fonts::chalk_large, gc.get_width()/2, 100, System::translate(result.plf->get_levelname()));
  gc.print_left(Fonts::chalk_large, 150, 180, _("Results:"));

  gc.print_left(Fonts::chalk_small, 150, 280, "Saved: ");
  for (int i = 0; i < result.saved; ++i)
    {
      gc.draw(chalk_pingus[rand() % chalk_pingus.size()], 230 + i * 15, 210);
    }
  gc.print_left(Fonts::chalk_small, 650, 280, to_string(result.saved));

  gc.print_left(Fonts::chalk_small, 150, 310, "Killed: ");    
  gc.print_left(Fonts::chalk_small, 250, 310, to_string(result.killed));


  gc.print_left(Fonts::chalk_small, 150, 340, "Needed: ");    
  gc.print_left(Fonts::chalk_small, 250, 340, to_string(result.needed));

  gc.print_left(Fonts::chalk_small, 150, 370, "Time left: ");    
  if (result.max_time == -1)
    gc.print_left(Fonts::chalk_small, 250, 370, "-");
  else
    gc.print_left(Fonts::chalk_small, 250, 370, to_string(result.max_time - result.used_time));

  if (result.success())
    {
      gc.print_center(Fonts::chalk_large, gc.get_width()/2, 450, "Success! =:-)");
      gc.print_center(Fonts::pingus_small, gc.get_width()/2, gc.get_height()-30,
                      "..:: Press Space to continue ::..");
    }
  else
    {
      gc.print_center(Fonts::chalk_large, gc.get_width()/2, 450, "Failure! :-(");
      gc.print_center(Fonts::pingus_small, gc.get_width()/2, gc.get_height()-30,
                      "..:: Press Space to retry the level ::..");
    }
}

ResultScreen::ResultScreen(Result arg_result)
  : result(arg_result)
{
  ResDescriptor ok_desc("result/ok", "core", ResDescriptor::RD_RESOURCE);
  ResDescriptor cancel_desc("result/retry", "core", ResDescriptor::RD_RESOURCE);

  ResultScreenComponent* comp = new ResultScreenComponent(result);
  gui_manager->add(comp);

  if (result.success())
    gui_manager->add(new ResultScreenOkButton(this));
  else
    gui_manager->add(new ResultScreenRetryButton(this));

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
  ScreenManager::instance()->replace_screen(new PingusGameSession (result.plf, false),
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
