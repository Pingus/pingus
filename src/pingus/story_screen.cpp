//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pingus/story_screen.hpp"

#include <algorithm>
#include <vector>
#include "gettext.h"
#include "pingus/globals.hpp"
#include "pingus/resource.hpp"
#include "gui/gui_manager.hpp"
#include "gui/surface_button.hpp"
#include "screen/screen_manager.hpp"
#include "gui/component.hpp"
#include "math/math.hpp"
#include "pingus/fonts.hpp"
#include "pingus/res_descriptor.hpp"
#include "worldmap/worldmap.hpp"
#include "worldmap/worldmap_screen.hpp"
#include "worldmap/worldmap_story.hpp"
#include "pingus/stat_manager.hpp"
#include "pingus/credits.hpp"
#include "display/display.hpp"
#include "util/utf8.hpp"
#include "sound/sound.hpp"

class StoryScreenComponent : public GUI::Component
{
private:
  Sprite background;
  Sprite blackboard;

  std::string display_text;
  float time_passed;

  bool page_displayed_completly;

  WorldmapNS::WorldmapStory* story;
  std::vector<StoryPage> pages;
  Sprite page_surface;
  StoryPage  current_page;

public:
  StoryScreenComponent (WorldmapNS::WorldmapStory *arg_pages);
  virtual ~StoryScreenComponent () {}

  void draw (DrawingContext& gc);
  void update(float delta);

  void skip_story();
  /** starts to display the next text page */
  void next_text();
  WorldmapNS::WorldmapStory* get_story() const { return story; }

private:
  StoryScreenComponent(const StoryScreenComponent&);
  StoryScreenComponent & operator=(const StoryScreenComponent&);
};

class StoryScreenContinueButton : public GUI::SurfaceButton
{
private:
  StoryScreenComponent* story_comp;
public:
  StoryScreenContinueButton(StoryScreenComponent* arg_story_comp, int x, int y)
    : GUI::SurfaceButton(x, y,
                         "core/misc/next",
                         "core/misc/next",
                         "core/misc/next_hover"),
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

private:
  StoryScreenContinueButton(const StoryScreenContinueButton&);
  StoryScreenContinueButton & operator=(const StoryScreenContinueButton&);
};

class StoryScreenSkipButton : public GUI::SurfaceButton
{
private: 
  StoryScreenComponent* story_comp;

public:
  StoryScreenSkipButton(StoryScreenComponent* arg_story_comp, int x, int y)
    : GUI::SurfaceButton(x, y, "", "", ""), // FIXME: Little fugly way to do a text button
      story_comp(arg_story_comp)
  {
  }

  void draw (DrawingContext& gc)
  {
    gc.print_right(Fonts::chalk_small, Vector2i(x_pos, y_pos), _("skip"));
  }

  bool is_at(int x, int y) 
  {
    return
      x > x_pos - static_cast<int>(Fonts::chalk_small.get_width(_("skip"))) && 
      x < x_pos &&
      y > y_pos &&
      y < y_pos + static_cast<int>(Fonts::chalk_small.get_height());
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

private:
  StoryScreenSkipButton(const StoryScreenSkipButton&);
  StoryScreenSkipButton & operator=(const StoryScreenSkipButton&);
};

StoryScreen::StoryScreen(FileReader reader) :
  story(new WorldmapNS::WorldmapStory(reader)),
  story_comp(),
  continue_button(0),
  skip_button(0)
{
  story_comp = new StoryScreenComponent(story.get());
  gui_manager->add(story_comp);
  gui_manager->add(continue_button = new StoryScreenContinueButton(story_comp, 
                                                                   Display::get_width()/2 + 220 + 40, 
                                                                   Display::get_height()/2 + 180 + 32));
  if (maintainer_mode)
    gui_manager->add(skip_button     = new StoryScreenSkipButton(story_comp, 
                                                                 Display::get_width() - 4, 
                                                                 Display::get_height() - 26));
}

StoryScreen::~StoryScreen()
{
}

StoryScreenComponent::StoryScreenComponent (WorldmapNS::WorldmapStory *arg_story) :
  background(),
  blackboard(),
  display_text(),
  time_passed(),
  page_displayed_completly(),
  story(arg_story),
  pages(),
  page_surface(),
  current_page()
{
  page_displayed_completly = false;
  time_passed  = 0;
  pages = story->get_pages();

  current_page = pages.back();
  page_surface = Sprite(current_page.image);

  background = Sprite("core/menu/wood");
  blackboard = Sprite("core/menu/blackboard");
}

void
StoryScreenComponent::draw (DrawingContext& gc)
{
  // Paint the background wood panel
  for(int y = 0; y < gc.get_height(); y += background.get_height())
    for(int x = 0; x < gc.get_width(); x += background.get_width())
      gc.draw(background, Vector2i(x, y));

  gc.draw(blackboard, Vector2i(gc.get_width()/2, gc.get_height()/2));

  gc.print_center(Fonts::chalk_large, 
                  Vector2i(gc.get_width()/2, gc.get_height()/2 - 200), 
                  story->get_title());
  gc.draw(page_surface, Vector2i(gc.get_width()/2, gc.get_height()/2 - 65));

  gc.print_left(Fonts::chalk_normal,
                Vector2i(gc.get_width()/2  - 280,
                         gc.get_height()/2 + 35),
                display_text);
}

void
StoryScreenComponent::update(float delta)
{
  time_passed += delta;

  if (!page_displayed_completly)
    {
      std::string::size_type len = static_cast<std::string::size_type>(20.0f * time_passed);
      std::string::size_type text_len = UTF8::length(current_page.text);
      
      display_text = UTF8::substr(current_page.text, 0, Math::min(text_len, len));
       
      if (text_len < len)
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
  ScreenManager::instance()->pop_screen();
}

void
StoryScreen::on_startup()
{
  // FIXME: Load the song from the WorldmapStory
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
          page_surface = Sprite(current_page.image);
          display_text = "";
          time_passed = 0;
          page_displayed_completly = false;
        }
      else
        {
#if 0 // FIXME: Sat Jul  5 04:17:01 2008    
          //Out of story pages - figure out which one this was (start or end)
          std::string which_story = "start";

          //Out of story pages - figure out which one this was (start or end)
          if (story == WorldmapNS::WorldmapScreen::instance()->get_worldmap()->get_intro_story())
            which_story = "start";
          else
            which_story = "end";

          // Record that player has seen this story.
          StatManager::instance()->set_bool(WorldmapNS::WorldmapScreen::instance()->get_worldmap()->get_shortname()
                                            + "-" + which_story + "story-seen", true);
          bool credits_seen = false;
          //Check if this is the last worldmap
          if (which_story == "end" &&
              WorldmapNS::WorldmapScreen::instance()->get_worldmap()->is_final_map())
            {
              // Check if final credits have been seen
              StatManager::instance()->get_bool("credits-seen", credits_seen);
              if (!credits_seen)
                ScreenManager::instance()->replace_screen(new Credits(), true);
              else
                ScreenManager::instance()->replace_screen(new WorldmapNS::WorldmapScreen(), true);
            }
          else
            {
              ScreenManager::instance()->replace_screen(new WorldmapNS::WorldmapScreen(), true);
            }
#endif 
          ScreenManager::instance()->pop_screen();
        }
    }
}

void
StoryScreen::resize(const Size& size_)
{
  GUIScreen::resize(size_);

  continue_button->set_pos(size.width/2 + 220 + 40, 
                           size.height/2 + 180 +32);
  if (skip_button)
    skip_button->set_pos(size.width  - 4, 
                         size.height - 26);
}

/* EOF */
