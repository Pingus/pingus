//  $Id: story_screen.cxx,v 1.4 2003/03/25 00:37:44 grumbel Exp $
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

#include <algorithm>
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
#include "res_descriptor.hxx"
#include "sound/sound.hxx"

class StoryPage
{
public:
  StoryPage() {}

  StoryPage(ResDescriptor arg_image, std::string arg_text)
    : image(arg_image), text(arg_text)
      
  {}

  ResDescriptor image;
  std::string   text;
};

class StoryScreenComponent : public GUI::Component
{
private:
  CL_Surface background;
  std::string display_text;
  float time_passed;

  bool page_displayed_completly;

  //std::string current_page.text;
  std::vector<StoryPage> pages;
  CL_Surface page_surface;
  StoryPage  current_page;
public:
  StoryScreenComponent ();
  virtual ~StoryScreenComponent () {}

  void draw (GraphicContext& gc);
  void update(float delta);

  /** starts to display the next text page */
  void next_text();
};

class StoryScreenContinueButton : public GUI::SurfaceButton
{
private:
  StoryScreenComponent* story_comp;
public:
  StoryScreenContinueButton(StoryScreenComponent* arg_story_comp)
    : GUI::SurfaceButton(600, 550, 
                         ResDescriptor("result/ok", "core", ResDescriptor::RD_RESOURCE),
                         ResDescriptor("result/ok", "core", ResDescriptor::RD_RESOURCE),
                         ResDescriptor("result/ok", "core", ResDescriptor::RD_RESOURCE)),
      story_comp(arg_story_comp)
  {
  }

  void on_click() 
  {
    story_comp->next_text();
    //ScreenManager::instance()->replace_screen (PingusMenuManager::instance (), false);
  }
};


StoryScreen::StoryScreen()
{
  story_comp = new StoryScreenComponent();
  gui_manager->add (story_comp);
  gui_manager->add (new StoryScreenContinueButton(story_comp));
}

StoryScreen::~StoryScreen()
{
}

StoryScreenComponent::StoryScreenComponent ()
{
  page_displayed_completly = false;
  time_passed  = 0;

  pages.push_back(StoryPage(ResDescriptor("Story/story0", "story"), 
                            "Page 1Dies ist ein Tet blabla, sehr lang blabla und mit umbruechen blabal\n"
                            "Zeile zwei mehr text blabla umbrauchuauaneh usnthu snaotehu nostehusano\n"
                            "authh nth pte thuat nth auntheun tahneuth oentuh oentuhontu hoantuhetuh\n"
                            "aoeuth htnh30  003 0238p[ 8eu ntuhnt hnthneat'hur  rcg rg th th tnh "));

  pages.push_back(StoryPage(ResDescriptor("Story/story1", "story"), 
                            "Page 2Dies ist ein Tet blabla, sehr lang blabla und mit umbruechen blabal\n"
                            "Zeile zwei mehr text blabla umbrauchuauaneh usnthu snaotehu nostehusano\n"
                            "authh nth pte thuat nth auntheun tahneuth oentuh oentuhontu hoantuhetuh\n"
                            "aoeuth htnh30  003 0238p[ 8eu ntuhnt hnthneat'hur  rcg rg th th tnh "));

  pages.push_back(StoryPage(ResDescriptor("Story/story2", "story"), 
                            "Page 3Dies ist ein Tet blabla, sehr lang blabla und mit umbruechen blabal\n"
                            "Zeile zwei mehr text blabla umbrauchuauaneh usnthu snaotehu nostehusano\n"
                            "authh nth pte thuat nth auntheun tahneuth oentuh oentuhontu hoantuhetuh\n"
                            "aoeuth htnh30  003 0238p[ 8eu ntuhnt hnthneat'hur  rcg rg th th tnh "));  

  pages.push_back(StoryPage(ResDescriptor("Story/story3", "story"), 
                            "Page 4Dies ist ein Tet blabla, sehr lang blabla und mit umbruechen blabal\n"
                            "Zeile zwei mehr text blabla umbrauchuauaneh usnthu snaotehu nostehusano\n"
                            "authh nth pte thuat nth auntheun tahneuth oentuh oentuhontu hoantuhetuh\n"
                            "aoeuth htnh30  003 0238p[ 8eu ntuhnt hnthneat'hur  rcg rg th th tnh "));

  std::reverse(pages.begin(), pages.end());

  current_page = pages.back();
  page_surface = PingusResource::load_surface(current_page.image);
  background   = PingusResource::load_surface("Story/background", "story");
}

void
StoryScreenComponent::draw (GraphicContext& gc)
{
  gc.draw(background, 0, 0);
  gc.print_center(Fonts::pingus_large, 400, 20, "Chapter 1 - Enter the unknown...");
  gc.draw(page_surface,  gc.get_width()/2 - page_surface.get_width()/2, 90);
  gc.print_left(Fonts::pingus_small, 100, 315, display_text);
}

void
StoryScreenComponent::update(float delta)
{
  time_passed += delta;

  if (!page_displayed_completly)
    {
      int i = Math::min(current_page.text.length(), (unsigned int)(15.0f * time_passed));
      display_text = current_page.text.substr(0, i);

      if (current_page.text.length() < (unsigned int)(15.0f * time_passed))
        {
          page_displayed_completly = true;
        }
    }
}

void
StoryScreen::on_fast_forward_press ()
{
  story_comp->next_text();
}

void 
StoryScreen::on_startup()
{
  PingusSound::play_sound ("letsgo");
}

void
StoryScreenComponent::next_text()
{
  if (!page_displayed_completly)
    {
      page_displayed_completly = true;
      display_text = current_page.text;  
    }
  else
    {
      pages.pop_back();
      if (!pages.empty())
        {
          current_page = pages.back();
          page_surface = PingusResource::load_surface(current_page.image);
          display_text = "";
          time_passed = 0;
          page_displayed_completly = false;
        }
      else
        {
          std::cout << "StoryScreenComponent: Out of story pages" << std::endl;
          ScreenManager::instance()->replace_screen (PingusMenuManager::instance (), false);
        }
    }
}

/* EOF */
