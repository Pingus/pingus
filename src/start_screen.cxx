//  $Id: start_screen.cxx,v 1.10 2003/04/06 14:37:07 grumbel Exp $
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
#include "gui/gui_manager.hxx"
#include "gui/surface_button.hxx"
#include "gui/component.hxx"
#include "gui/screen_manager.hxx"
#include "my_gettext.hxx"
#include "game_session.hxx"
#include "globals.hxx"
#include "string_converter.hxx"
#include "system.hxx"
#include "fonts.hxx"
#include "plf.hxx"
#include "pingus_resource.hxx"
#include "start_screen.hxx"
#include "game_time.hxx"
#include "sound/sound.hxx"

class StartScreenComponent : public GUI::Component
{
private:
  PLFHandle plf;
  CL_Surface background;
  std::string time_str;
public:
  StartScreenComponent(PLFHandle plf);
  virtual ~StartScreenComponent() {}
  void draw(GraphicContext& gc);
};

class StartScreenOkButton : public GUI::SurfaceButton
{
private:
  StartScreen* parent;
public:
  StartScreenOkButton(StartScreen* p)
    : GUI::SurfaceButton(625, 425, 
                         ResDescriptor("start/ok", "core", ResDescriptor::RD_RESOURCE),
                         ResDescriptor("start/ok_clicked", "core", ResDescriptor::RD_RESOURCE),
                         ResDescriptor("start/ok_hover", "core", ResDescriptor::RD_RESOURCE)),
      parent(p)
  {
  }

  void draw(GraphicContext& gc) {
    SurfaceButton::draw(gc);
    gc.print_center(Fonts::chalk_normal, x_pos + 32, y_pos - 17, _("Ok"));
  }

  void on_click() 
  {
    PingusSound::play_sound("yipee");
    parent->start_game();
  }


  void on_pointer_enter()
  {
    SurfaceButton::on_pointer_enter();
    PingusSound::play_sound ("tick");
  }
};


class StartScreenAbortButton
  : public GUI::SurfaceButton
{
private:
  StartScreen* parent;
public:
  StartScreenAbortButton(StartScreen* p)
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
    parent->cancel_game();
  }

  void on_pointer_enter()
  {
    SurfaceButton::on_pointer_enter();
    PingusSound::play_sound ("tick");
  }
};

StartScreen::~StartScreen()
{
  
}

StartScreenComponent::StartScreenComponent(PLFHandle p)
  : plf(p)
{
  background = PingusResource::load_surface("menu/startscreenbg", "core");
  time_str = GameTime::ticks_to_realtime_string(plf->get_time());
}

void
StartScreenComponent::draw(GraphicContext& gc)
{
  //gc.clear(0,0,0);
  background.put_screen(0,0);
  
  gc.print_center(Fonts::chalk_large, gc.get_width()/2, 100, System::translate(plf->get_levelname()));
  gc.print_left(Fonts::chalk_normal, 130, 160, System::translate(plf->get_description()));

  gc.print_left (Fonts::chalk_normal, 300, 310, _("Number of Pingus: "));
  gc.print_right(Fonts::chalk_normal, 500, 310, to_string(plf->get_pingus()));

  gc.print_left (Fonts::chalk_normal, 300, 340, _("Number to Save: "));
  gc.print_right(Fonts::chalk_normal, 500, 340, to_string(plf->get_number_to_save()));

  gc.print_left (Fonts::chalk_normal, 300, 370, _("Time: "));
  gc.print_right(Fonts::chalk_normal, 500, 370, time_str);
  
  gc.print_left (Fonts::chalk_normal, 300, 400, _("Difficulty:"));
  gc.print_right(Fonts::chalk_normal, 500, 400, to_string(plf->get_difficulty()) + "/100");

  /*for (int i = 0; plf->get_difficulty())
    {
    }*/

  gc.print_center(Fonts::chalk_small, 400, 470, _("Author: ") + plf->get_author());

  if (maintainer_mode)
    gc.print_left(Fonts::chalk_small, 110, 430, _("Filename: ") + plf->get_resname());
}

StartScreen::StartScreen(PLFHandle arg_plf)
  : plf(arg_plf)
{
  StartScreenComponent* comp = new StartScreenComponent(plf);
  gui_manager->add(comp);
  gui_manager->add(new StartScreenOkButton(this));
  gui_manager->add(new StartScreenAbortButton(this));
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
  PingusGameSession* game_session = new PingusGameSession(plf, true);
  ScreenManager::instance()->replace_screen(game_session, true);
}

void
StartScreen::cancel_game()
{
  ScreenManager::instance()->pop_screen();
}

/* EOF */
