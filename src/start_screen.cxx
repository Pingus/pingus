//  $Id: start_screen.cxx,v 1.15 2003/04/11 15:15:34 torangan Exp $
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
#include "sprite.hxx"
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
  Sprite background;
  std::string time_str;
  std::string description;
  
public:
  StartScreenComponent(PLFHandle plf);
  void draw(GraphicContext& gc);
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
    : GUI::SurfaceButton(CL_Display::get_width()/2 + 225,
                         CL_Display::get_height()/2 + 125, 
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
    : GUI::SurfaceButton(CL_Display::get_width()/2 - 278,
                         CL_Display::get_height()/2 + 144, 
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
  background = Sprite("menu/startscreenbg", "core");
  background.set_align_center();
  time_str = GameTime::ticks_to_realtime_string(plf->get_time());
}

void
StartScreenComponent::draw(GraphicContext& gc)
{
  //gc.clear(0,0,0);
  background.put_screen(CL_Display::get_width()/2,CL_Display::get_height()/2);
  
  int left_x  = CL_Display::get_width()/2 - 120;
  int right_x = CL_Display::get_width()/2 + 120;
  int y = CL_Display::get_height()/2 + 10;

  gc.print_center(Fonts::chalk_large, 
                  gc.get_width()/2, 
                  CL_Display::get_height()/2 - 200,
                  System::translate(plf->get_levelname()));

  gc.print_left(Fonts::chalk_normal,
                CL_Display::get_width()/2 - 290,
                CL_Display::get_height()/2 - 140,
                format_description(800 - 230));

  gc.print_left (Fonts::chalk_normal, left_x,  y, _("Number of Pingus: "));
  gc.print_right(Fonts::chalk_normal, right_x, y, to_string(plf->get_pingus()));

  gc.print_left (Fonts::chalk_normal, left_x,  y += 30, _("Number to Save: "));
  gc.print_right(Fonts::chalk_normal, right_x, y, to_string(plf->get_number_to_save()));

  gc.print_left (Fonts::chalk_normal, left_x,  y += 30, _("Time: "));
  gc.print_right(Fonts::chalk_normal, right_x, y, time_str);
  
  gc.print_left (Fonts::chalk_normal, left_x,  y += 30, _("Difficulty:"));
  gc.print_right(Fonts::chalk_normal, right_x, y, to_string(plf->get_difficulty()) + "/100");

  /*for (int i = 0; plf->get_difficulty())
    {
    }*/

  gc.print_center(Fonts::chalk_small, CL_Display::get_width()/2,
                  CL_Display::get_height()/2 + 270, _("Author: ") + plf->get_author());

  if (maintainer_mode)
    gc.print_left(Fonts::chalk_small, 110, 430, _("Filename: ") + plf->get_resname());
    
  CL_System::sleep(30);
}

const std::string&
StartScreenComponent::format_description(int length)
{
  if (description != "")
    return description;
    
  description = System::translate(plf->get_description());

  unsigned int pos = 0;
  while ((pos = description.find('\t', pos)) != std::string::npos)
    description.replace(pos, 1, 1, ' ');
  
  pos = 0;
  while ((pos = description.find("  ", pos)) != std::string::npos)
    description.replace(pos, 2, 1, ' ');

  pos = 0;  
  while ((pos = description.find('\n', pos)) != std::string::npos)
    {
      if (description[pos + 1] == '\n')          // double enter marks paragraph
        description.replace(pos++, 2, 1, '\n');  // replace the two \n by one and move pos behind it
      else
        description.replace(pos, 1, 1, ' ');
    }
        
  pos = 0;
  while ((pos = description.find("  ", pos)) != std::string::npos)
    description.replace(pos, 2, 1, ' ');

  int start_pos      = 0;
  int previous_space = 0;
  pos = 0;
  while ((pos = description.find(' ', pos + 1)) != std::string::npos)
    {
      if (Fonts::chalk_normal->get_text_width(description.substr(start_pos, pos - start_pos)) > length)
        {
	  description[previous_space] = '\n';
	  start_pos = previous_space + 1;
	}
      else if (Fonts::chalk_normal->get_text_width(description.substr(start_pos, description.length())) <= length)
        break;

      previous_space = pos;
    }
  return description;
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
