//  $Id$
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
#include <vector>
#include "gettext.h"
#include "resource.hpp"
#include "gui/gui_manager.hpp"
#include "gui/surface_button.hpp"
#include "gui/screen_manager.hpp"
#include "gui/component.hpp"
#include "math.hpp"
#include "pingus_menu_manager.hpp"
#include "fonts.hpp"
#include "story_screen.hpp"
#include "res_descriptor.hpp"
#include "worldmap/worldmap.hpp"
#include "worldmap/manager.hpp"
#include "worldmap/worldmap_story.hpp"
#include "stat_manager.hpp"
#include "credits.hpp"
#include "sound/sound.hpp"

#define SKIP_TEXT _("skip")

class StoryScreenComponent : public GUI::Component
{
private:
  Sprite background;
  std::string display_text;
  float time_passed;

  bool page_displayed_completly;

  WorldMapNS::WorldMapStory *story;
  std::vector<StoryPage> pages;
  Sprite page_surface;
  StoryPage  current_page;
public:
  StoryScreenComponent (WorldMapNS::WorldMapStory *arg_pages);
  virtual ~StoryScreenComponent () {}

  void draw (DrawingContext& gc);
  void update(float delta);

  void skip_story();
  /** starts to display the next text page */
  void next_text();
  WorldMapNS::WorldMapStory* get_story() const { return story; }
};

class StoryScreenContinueButton : public GUI::SurfaceButton
{
private:
  StoryScreenComponent* story_comp;
public:
  StoryScreenContinueButton(StoryScreenComponent* arg_story_comp)
    : GUI::SurfaceButton(Display::get_width()/2 + 220, Display::get_height()/2 + 160,
                         ResDescriptor("core/misc/next"),
                         ResDescriptor("core/misc/next"),
                         ResDescriptor("core/misc/next_hover")),
      story_comp(arg_story_comp)
  {
  }

  void on_pointer_enter()
  {
    SurfaceButton::on_pointer_enter();
    Sound::PingusSound::play_sound ("tick", 0.3f);
  }

  void on_click()
  {
    story_comp->next_text();
  }
};

class StoryScreenSkipButton : public GUI::SurfaceButton
{
private:
  StoryScreenComponent* story_comp;
public:
  StoryScreenSkipButton(StoryScreenComponent* arg_story_comp)
    : GUI::SurfaceButton(Display::get_width() - 4, Display::get_height() - 26,
                         ResDescriptor(), ResDescriptor(), ResDescriptor()),
      story_comp(arg_story_comp)
  {
  }

  void draw (DrawingContext& gc)
  {
	  gc.print_right(Fonts::chalk_small, (float)x_pos, (float)y_pos, SKIP_TEXT);
  }

  bool is_at(int x, int y) {
	  return x > x_pos - Fonts::chalk_small.get_width(SKIP_TEXT) && x < x_pos
		&& y > y_pos && y < y_pos + Fonts::chalk_small.get_height();
  }

  void on_pointer_enter()
  {
    SurfaceButton::on_pointer_enter();
    Sound::PingusSound::play_sound ("tick", 0.3f);
  }

  void on_click()
  {
	story_comp->skip_story();
  }
};

StoryScreen::StoryScreen(WorldMapNS::WorldMapStory *arg_pages)
{
  story_comp = new StoryScreenComponent(arg_pages);
  gui_manager->add (story_comp, true);
  gui_manager->add (new StoryScreenContinueButton(story_comp), true);
  gui_manager->add (new StoryScreenSkipButton(story_comp), true);
}

StoryScreen::~StoryScreen()
{
}

StoryScreenComponent::StoryScreenComponent (WorldMapNS::WorldMapStory *arg_story)
  : story(arg_story)
{
  page_displayed_completly = false;
  time_passed  = 0;
  pages = story->get_pages();

  current_page = pages.back();
  page_surface = Resource::load_sprite(current_page.image);
  background   = Resource::load_sprite("core/menu/startscreenbg");
  
  background.scale(Display::get_width(), Display::get_height());
}

void
StoryScreenComponent::draw (DrawingContext& gc)
{
  gc.draw(background, Vector3f(gc.get_width()/2, gc.get_height()/2));

  gc.print_center(Fonts::chalk_large, static_cast<float>(Display::get_width()/2),
                  static_cast<float>(Display::get_height()/2 - 200), story->get_title());
  gc.draw(page_surface, Vector3f(gc.get_width()/2, gc.get_height()/2 - 65));

  gc.print_left(Fonts::chalk_normal,
                static_cast<float>(Display::get_width()/2  - 280),
                static_cast<float>(Display::get_height()/2 + 35),
                display_text);
}

void
StoryScreenComponent::update(float delta)
{
  time_passed += delta;

  if (!page_displayed_completly)
    {
      std::string::size_type len = static_cast<std::string::size_type>(20.0f * time_passed);
      display_text = current_page.text.substr(0, Math::min(current_page.text.length(), len));

      if (current_page.text.length() < len)
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
  ScreenManager::instance()->replace_screen(WorldMapNS::WorldMapManager::instance ());
}

void
StoryScreen::on_startup()
{
  // FIXME: Load the song from the WorldMapStory
  Sound::PingusSound::play_music(story_comp->get_story()->get_music(), .7f);
}

void StoryScreenComponent::skip_story()
{
	page_displayed_completly = true;
	while (pages.size() > 1)
		pages.pop_back();
	next_text();
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
          page_surface = Resource::load_sprite(current_page.image);
          display_text = "";
          time_passed = 0;
          page_displayed_completly = false;
        }
      else
        {
          //Out of story pages - figure out which one this was (start or end)
          std::string which_story;
          if (story == WorldMapNS::WorldMapManager::instance()->get_worldmap()->get_intro_story())
            which_story = "start";
          else
            which_story = "end";

          // Record that player has seen this story.
          StatManager::instance()->set_bool(WorldMapNS::WorldMapManager::instance()->get_worldmap()->get_shortname()
                                            + "-" + which_story + "story-seen", true);

          bool credits_seen = false;
          //Check if this is the last worldmap
          if (which_story == "end" &&
              WorldMapNS::WorldMapManager::instance()->get_worldmap()->is_final_map())
            {
              // Check if final credits have been seen
              StatManager::instance()->get_bool("credits-seen", credits_seen);
              if (!credits_seen)
                ScreenManager::instance()->replace_screen(Credits::instance(), false);
              else
                ScreenManager::instance()->replace_screen(WorldMapNS::WorldMapManager::instance());
            }
          else
            {
              ScreenManager::instance()->replace_screen(WorldMapNS::WorldMapManager::instance());
            }
        }
    }
}


// Local Variables: ***
// coding:iso-latin-1
// End: ***

/* EOF */
