//  $Id: result_screen.cxx,v 1.1 2003/03/10 11:29:49 grumbel Exp $
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
#include "string_converter.hxx"
#include "result_screen.hxx"

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

class ResultComponent : public GUI::Component
{
private:
  Result result;
public:
  ResultComponent(Result arg_result)
    : result(arg_result)
  {
  }

  void draw(GraphicContext& gc) {
    gc.print_left(Fonts::pingus_small, 100, 100, "Saved: ");
    gc.print_left(Fonts::pingus_small, 200, 100, to_string(result.saved));
    gc.print_left(Fonts::pingus_small, 100, 130, "Killed: ");    
    gc.print_left(Fonts::pingus_small, 200, 130, to_string(result.killed));
  }
};

ResultScreen::ResultScreen(Result result)
{
  ResDescriptor ok_desc("result/ok", "core", ResDescriptor::RD_RESOURCE);
  ResDescriptor cancel_desc("result/retry", "core", ResDescriptor::RD_RESOURCE);

  gui_manager->add(new ResultComponent(result));
  gui_manager->add(new ResultScreenOkButton());
  //gui_manager->add(new GUI::SurfaceButton(500, 500, cancel_desc, cancel_desc, cancel_desc));
}

/* EOF */
