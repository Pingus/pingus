//  $Id: story_screen.cxx,v 1.2 2003/03/22 23:28:51 grumbel Exp $
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

#include <ClanLib/Display/Display/surface.h>
#include "pingus_resource.hxx"
#include "gui/gui_manager.hxx"
#include "gui/surface_button.hxx"
#include "gui/screen_manager.hxx"
#include "gui/component.hxx"
#include "math.hxx"
#include "pingus_menu_manager.hxx"
#include "fonts.hxx"
#include "story_screen.hxx"
#include "sound/sound.hxx"

class StoryScreenComponent : public GUI::Component
{
private:
  CL_Surface sur;
  CL_Surface background;
  std::string text;
  std::string display_text;
  float time_passed;
public:
  StoryScreenComponent ();
  virtual ~StoryScreenComponent () {}
  void draw (GraphicContext& gc);
  void update(float delta);
};

class StoryScreenContinueButton : public GUI::SurfaceButton
{
private:
public:
  StoryScreenContinueButton()
    : GUI::SurfaceButton(600, 550, 
                         ResDescriptor("result/ok", "core", ResDescriptor::RD_RESOURCE),
                         ResDescriptor("result/ok", "core", ResDescriptor::RD_RESOURCE),
                         ResDescriptor("result/ok", "core", ResDescriptor::RD_RESOURCE))
  {
  }

  void on_click() 
  {
    std::cout << "Continue: Got CLICK!!!" << std::endl;
    ScreenManager::instance()->replace_screen (PingusMenuManager::instance (), false);
  }
};


StoryScreen::StoryScreen()
{
  StoryScreenComponent* comp = new StoryScreenComponent();
  gui_manager->add (comp);
  gui_manager->add (new StoryScreenContinueButton());
}

StoryScreen::~StoryScreen()
{
}

StoryScreenComponent::StoryScreenComponent ()
{
  time_passed = 0;
  sur = PingusResource::load_surface("Story/story1", "story");
  background = PingusResource::load_surface("Story/background", "story");

  text = 
    "Dies ist ein Tet blabla, sehr lang blabla und mit umbruechen blabal\n"
    "Zeile zwei mehr text blabla umbrauchuauaneh usnthu snaotehu nostehusano\n"
    "authh nth pte thuat nth auntheun tahneuth oentuh oentuhontu hoantuhetuh\n"
    "aoeuth htnh30  003 0238p[ 8eu ntuhnt hnthneat'hur  rcg rg th th tnh ";
}

void
StoryScreenComponent::draw (GraphicContext& gc)
{
  //gc.clear(0,0,0);
  gc.draw(background, 0, 0);
  gc.print_center(Fonts::pingus_large, 400, 20, "Chapter 1 - Enter the unknown...");
  sur.put_screen(400-320, 90);
  gc.print_left(Fonts::pingus_small, 100, 315, display_text);
}

void
StoryScreenComponent::update(float delta)
{
  time_passed += delta;

  int i = Math::min(text.length(), (unsigned int)(15.0f * time_passed));

  display_text = text.substr(0, i);
}

void 
StoryScreen::on_startup()
{
  PingusSound::play_sound ("letsgo");
}

/* EOF */
