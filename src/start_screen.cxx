//  $Id: start_screen.cxx,v 1.3 2003/03/28 16:16:00 grumbel Exp $
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

class StartScreenComponent : public GUI::Component
{
private:
  PLFHandle plf;
  CL_Surface background;
public:
  StartScreenComponent(PLFHandle plf);
  virtual ~StartScreenComponent() {}
  void draw(GraphicContext& gc);
};

class StartScreenOkButton : public GUI::SurfaceButton
{
private:
  PLFHandle plf;
public:
  StartScreenOkButton(PLFHandle p)
    : GUI::SurfaceButton(300, 500, 
                         ResDescriptor("result/ok", "core", ResDescriptor::RD_RESOURCE),
                         ResDescriptor("result/ok", "core", ResDescriptor::RD_RESOURCE),
                         ResDescriptor("result/ok", "core", ResDescriptor::RD_RESOURCE)),
      plf(p)
  {
  }

  void on_click() 
  {
    PingusGameSession* game_session = new PingusGameSession(plf, true);
    ScreenManager::instance()->replace_screen(game_session, true);
  }
};

StartScreen::StartScreen(PLFHandle plf)
{
  StartScreenComponent* comp = new StartScreenComponent(plf);
  gui_manager->add(comp);
  gui_manager->add(new StartScreenOkButton(plf));
}

StartScreen::~StartScreen()
{
  
}

StartScreenComponent::StartScreenComponent(PLFHandle p)
  : plf(p)
{
  background = PingusResource::load_surface("menu/startscreenbg", "core");
}

void
StartScreenComponent::draw(GraphicContext& gc)
{
  //gc.clear(0,0,0);
  background.put_screen(0,0);
  gc.print_center(Fonts::chalk_large, gc.get_width()/2, 100, System::translate(plf->get_levelname()));
  gc.print_center(Fonts::chalk_normal, gc.get_width()/2, 160, System::translate(plf->get_description()));

  gc.print_left(Fonts::chalk_normal, 250, 250, _("Number of Pingus: ") + to_string(plf->get_pingus()));
  gc.print_left(Fonts::chalk_normal, 250, 280, _("Number to Save: ") + to_string(plf->get_number_to_save()));
  gc.print_left(Fonts::chalk_normal, 250, 310, _("Time: ") + to_string(plf->get_time()));
  
  gc.print_left(Fonts::chalk_normal, 250, 340, _("Difficulty:"));
  /*for (int i = 0; plf->get_difficulty())
    {
    }*/

  gc.print_left(Fonts::chalk_small, 110, 460, _("Author: ") + plf->get_author());

  if (maintainer_mode)
    gc.print_left(Fonts::chalk_small, 110, 430, _("Filename: ") + plf->get_filename());
}

/* EOF */
