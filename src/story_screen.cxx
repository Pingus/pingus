//  $Id: story_screen.cxx,v 1.9 2003/04/05 18:36:50 grumbel Exp $
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
#include "my_gettext.hxx"
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
#include "worldmap/manager.hxx"
#include "stat_manager.hxx"
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

  pages.push_back
    (StoryPage
     (ResDescriptor("Story/story0", "story"), 
      _("For a long time now the Pingus have lived happily in peace on the\n"
        "south pole along with all the other animals, everything was in\n"
        "balance and it seemed like nothing could disrupt there peace.  The\n"
        "Pingus where happy and it seemed like this could never end.\n")));

  pages.push_back
    (StoryPage
     (ResDescriptor("Story/story1", "story"), 
      _("But then one day, things began to change slowly, the sky got darker\n"
        "and the earth got warmer, first of they thought this was just a\n"
        "normal fluctuation in the worlds climate, but things seemed to get\n"
        "worse and worse with every year.")));


  pages.push_back
    (StoryPage
     (ResDescriptor("Story/story2", "story"), 
      _("The snow began to melt away in a few regions and food became an\n"
        "issue. Other animals already tend to leave the region to search\n"
        "for colder areas, but the Pingus knew that this couldn't help,\n"
        "they knew that they had to do something about this problem.")));

  pages.push_back
    (StoryPage
     (ResDescriptor("Story/story3", "story"), 
      _("So the circle of the eldest came together to decide what to do\n"
        "about it. They decided to send out an expedition around the world\n"
        "to find the cause for this warming. The expedition consisted of\n"
        "hundreds of the bravest Pingus around on the south pole.")));
    
  pages.push_back
    (StoryPage
     (ResDescriptor("Story/story4", "story"), 
      _("And they picked you to lead them on there journey around the\n"
        "world. Since the journey will be dangerous and difficult. Your\n"
        "first goal is the Island of Mogorok, also know has Tutorial\n"
        "Island. According to the eldest, this island has always been the\n"
        "first stop of Pingus which where send out into the world.")));

  pages.push_back
    (StoryPage
     (ResDescriptor("Story/story5", "story"), 
      _("The island consists of many regions which are ideally to train the\n"
        "abilities of the Pingus. The Pingus can train there their ability\n"
        "to build bridges, climb, bash and all the rest of there talents\n"
        "which they will surly need on there long and dangerous journey.")));

  pages.push_back
    (StoryPage
     (ResDescriptor("Story/story6", "story"), 
      _("While you on the other site can practice there your ability to\n"
        "command and guide the Pingus. You can also get familiar there with\n"
        "all the abilities the Pingus provide and learn to master them,\n"
        "since you will need them all for sure an your journey around the\n"
        "world.")));

  pages.push_back
    (StoryPage
     (ResDescriptor("Story/story6", "story"), 
      _("Now that you and the Pingus have arrive at tutorial island it\n"
        "is time to take over the command and begin your mission\n"
        "\n"
        "                                                  Good Luck!")));


  /* 
     Now after you and the Pingus have learned the basics and
     practiced a bit it is time to move on and begin the journey into
     the world. Since the floe with which the Pingus traveled to the
     Tutorial Island isn't going to hold the whole way into the warmer
     climated regions the Pingus have to find something else to guide
     there on there journey.

     But as the eldest have said, the Tutorial Island provides not
     only a way to practive, but it is also the starting point into
     the world, cause the wood if the large tree at the end of the
     island is brings good wood to construct a float.
     
     So the Pingus set out and constructed a some large floats, enough
     to carry them all. After also packaging up a bunch of provisions
     they where prepared to finally start of there journey and leave
     their familiar ground and entering the unknown parts of the
     world.
     
     So the Pingus sat on there float, worrying whats to come and
     where to go, while floating into the sunset.

     To be continued...
   */

    std::reverse(pages.begin(), pages.end());

    current_page = pages.back();
    page_surface = PingusResource::load_surface(current_page.image);
    background   = PingusResource::load_surface("menu/startscreenbg", "core");
}

void
StoryScreenComponent::draw (GraphicContext& gc)
{
  gc.draw(background, 0, 0);
  gc.print_center(Fonts::chalk_large, 400, 100, "Chapter I - Tutorial Island");
  gc.draw(page_surface,  gc.get_width()/2 - page_surface.get_width()/2,
          160);
  gc.print_left(Fonts::chalk_normal, 120, 335, display_text);
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
StoryScreen::on_escape_press ()
{
  if (1 /* story_seen */)
    ScreenManager::instance()->replace_screen(WorldMapNS::WorldMapManager::instance ());
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
          StatManager::instance()->set_bool("story-seen", true);
          //ScreenManager::instance()->replace_screen (PingusMenuManager::instance (), false);
          ScreenManager::instance()->replace_screen(WorldMapNS::WorldMapManager::instance ());
        }
    }
}

// Local Variables: ***
// coding:iso-latin-1
// End: ***

/* EOF */
