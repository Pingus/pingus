//  $Id: result_screen.cxx,v 1.2 2003/03/16 22:54:32 grumbel Exp $
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
#include "gui/surface_button.hxx"
#include "gui/gui_manager.hxx"
#include "gui/screen_manager.hxx"
#include "res_descriptor.hxx"
#include "fonts.hxx"
#include "plf.hxx"
#include "string_converter.hxx"
#include "game_session.hxx"
#include "system.hxx"
#include "result_screen.hxx"

class ResultScreenComponent : public GUI::Component
{
public:
  Result result;

  ResultScreenComponent(Result arg_result);
  void draw(GraphicContext& gc) ;
};

class ResultScreenOkButton 
  : public GUI::SurfaceButton
{
public:
  ResultScreenOkButton()
    : GUI::SurfaceButton(100, 500, 
                         ResDescriptor("result/ok", "core", ResDescriptor::RD_RESOURCE),
                         ResDescriptor("result/ok", "core", ResDescriptor::RD_RESOURCE),
                         ResDescriptor("result/ok", "core", ResDescriptor::RD_RESOURCE))
  {
  }

  void on_click() {
    std::cout << "Got CLICK!!!" << std::endl;
    ScreenManager::instance()->pop_screen();    
  }
};

class ResultScreenRetryButton 
  : public GUI::SurfaceButton
{
private:
  ResultScreenComponent* parent;
public:
  ResultScreenRetryButton(ResultScreenComponent* p)
    : GUI::SurfaceButton(500, 500, 
                         ResDescriptor("result/retry", "core", ResDescriptor::RD_RESOURCE),
                         ResDescriptor("result/retry", "core", ResDescriptor::RD_RESOURCE),
                         ResDescriptor("result/retry", "core", ResDescriptor::RD_RESOURCE)),
      parent(p)
  {
  }

  void on_click() 
  {
    std::cout << "Got CLICK on Retry!!!" << std::endl;
    ScreenManager::instance()->replace_screen(new PingusGameSession (parent->result.plf), true);
  }
};

ResultScreenComponent::ResultScreenComponent(Result arg_result)
  : result(arg_result)
{
}

void
ResultScreenComponent::draw(GraphicContext& gc) 
{
  gc.clear(.0f, .0f, .0f);
  gc.print_center(Fonts::pingus_large, gc.get_width()/2, 20, "Results for ");
  gc.print_center(Fonts::pingus_large, gc.get_width()/2, 70, System::translate(result.plf->get_levelname()));

  gc.print_left(Fonts::pingus_small, 100, 180, "Saved: ");
  gc.print_left(Fonts::pingus_small, 200, 180, to_string(result.saved));
  gc.print_left(Fonts::pingus_small, 100, 210, "Killed: ");    
  gc.print_left(Fonts::pingus_small, 200, 210, to_string(result.killed));

  gc.print_left(Fonts::pingus_small, 100, 240, "Needed: ");    
  gc.print_left(Fonts::pingus_small, 200, 240, to_string(result.needed));

  gc.print_left(Fonts::pingus_small, 100, 270, "Time left: ");    
  if (result.max_time == -1)
    gc.print_left(Fonts::pingus_small, 200, 270, "-");
  else
    gc.print_left(Fonts::pingus_small, 200, 270, to_string(result.max_time - result.used_time));

  if (result.saved >= result.needed)
    gc.print_center(Fonts::pingus_large, gc.get_width()/2, 350, "Success! =:-)");
  else
    gc.print_center(Fonts::pingus_large, gc.get_width()/2, 350, "Failure! :-(");
}

ResultScreen::ResultScreen(Result result)
{
  ResDescriptor ok_desc("result/ok", "core", ResDescriptor::RD_RESOURCE);
  ResDescriptor cancel_desc("result/retry", "core", ResDescriptor::RD_RESOURCE);

  ResultScreenComponent* comp = new ResultScreenComponent(result);
  gui_manager->add(comp);
  gui_manager->add(new ResultScreenOkButton());
  gui_manager->add(new ResultScreenRetryButton(comp));
  //gui_manager->add(new GUI::SurfaceButton(500, 500, cancel_desc, cancel_desc, cancel_desc));
}

/* EOF */
