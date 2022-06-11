// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pingus/screens/story_screen.hpp"

#include <strut/utf8.hpp>

#include "engine/display/display.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/gui/surface_button.hpp"
#include "engine/screen/screen_manager.hpp"
#include "engine/sound/sound.hpp"
#include "pingus/fonts.hpp"
#include "pingus/gettext.h"
#include "pingus/globals.hpp"
#include "pingus/screens/credits.hpp"
#include "util/pathname.hpp"

namespace pingus {

class StoryScreenComponent : public pingus::gui::Component
{
private:
  Sprite background;
  Sprite blackboard;

  std::string display_text;
  float time_passed;

  bool page_displayed_completly;

  pingus::worldmap::WorldmapStory* story;
  std::vector<pingus::worldmap::StoryPage> pages;
  Sprite page_surface;
  pingus::worldmap::StoryPage  current_page;
  bool m_credits;

public:
  StoryScreenComponent (pingus::worldmap::WorldmapStory *arg_pages, bool credits);
  ~StoryScreenComponent () override {}

  void draw (DrawingContext& gc) override;
  void update(float delta) override;

  void skip_story();
  /** starts to display the next text page */
  void next_text();
  pingus::worldmap::WorldmapStory* get_story() const { return story; }

private:
  StoryScreenComponent(StoryScreenComponent const&);
  StoryScreenComponent & operator=(StoryScreenComponent const&);
};

class StoryScreenContinueButton : public pingus::gui::SurfaceButton
{
private:
  StoryScreenComponent* story_comp;
public:
  StoryScreenContinueButton(StoryScreenComponent* arg_story_comp, int x, int y)
    : pingus::gui::SurfaceButton(x, y,
                         "core/misc/next",
                         "core/misc/next",
                         "core/misc/next_hover"),
      story_comp(arg_story_comp)
  {
  }

  void on_pointer_enter() override
  {
    SurfaceButton::on_pointer_enter();
    pingus::sound::PingusSound::play_sound ("tick");
  }

  void on_click() override
  {
    story_comp->next_text();
  }

private:
  StoryScreenContinueButton(StoryScreenContinueButton const&);
  StoryScreenContinueButton & operator=(StoryScreenContinueButton const&);
};

class StoryScreenSkipButton : public pingus::gui::SurfaceButton
{
private:
  StoryScreenComponent* story_comp;

public:
  StoryScreenSkipButton(StoryScreenComponent* arg_story_comp, int x, int y)
    : pingus::gui::SurfaceButton(x, y, "", "", ""), // FIXME: Little fugly way to do a text button
      story_comp(arg_story_comp)
  {
  }

  void draw (DrawingContext& gc) override
  {
    gc.print_right(pingus::fonts::chalk_small, Vector2i(x_pos, y_pos), _("skip"));
  }

  bool is_at(int x, int y) override
  {
    return
      x > x_pos - static_cast<int>(pingus::fonts::chalk_small.get_width(_("skip"))) &&
      x < x_pos &&
      y > y_pos &&
      y < y_pos + pingus::fonts::chalk_small.get_height();
  }

  void on_pointer_enter() override
  {
    SurfaceButton::on_pointer_enter();
    pingus::sound::PingusSound::play_sound ("tick");
  }

  void on_click() override
  {
    story_comp->skip_story();
  }

private:
  StoryScreenSkipButton(StoryScreenSkipButton const&);
  StoryScreenSkipButton & operator=(StoryScreenSkipButton const&);
};

StoryScreen::StoryScreen(prio::ReaderMapping const& reader, bool credits) :
  story(new pingus::worldmap::WorldmapStory(reader)),
  story_comp(),
  continue_button(nullptr),
  skip_button(nullptr),
  m_credits(credits)
{
  story_comp = gui_manager->create<StoryScreenComponent>(story.get(), m_credits);
  continue_button = gui_manager->create<StoryScreenContinueButton>(story_comp,
                                                                   Display::get_width()/2 + 220 + 40,
                                                                   Display::get_height()/2 + 180 + 32);
  if (globals::developer_mode)
  {
    skip_button = gui_manager->create<StoryScreenSkipButton>(story_comp,
                                                             Display::get_width() - 4,
                                                             Display::get_height() - 26);
  }
}

StoryScreen::~StoryScreen()
{
}

StoryScreenComponent::StoryScreenComponent (pingus::worldmap::WorldmapStory *arg_story, bool credits) :
  background(),
  blackboard(),
  display_text(),
  time_passed(),
  page_displayed_completly(),
  story(arg_story),
  pages(),
  page_surface(),
  current_page(),
  m_credits(credits)
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

  gc.print_center(pingus::fonts::chalk_large,
                  Vector2i(gc.get_width()/2, gc.get_height()/2 - 200),
                  story->get_title());
  gc.draw(page_surface, Vector2i(gc.get_width()/2, gc.get_height()/2 - 65));

  gc.print_left(pingus::fonts::chalk_normal,
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
    std::string::size_type text_len = strut::utf8::length(current_page.text);

    display_text = strut::utf8::substr(current_page.text, 0, std::min(text_len, len));

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
  pingus::sound::PingusSound::play_music(story_comp->get_story()->get_music(), .7f);
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
      if (m_credits)
      {
        ScreenManager::instance()->replace_screen
          (std::make_shared<Credits>(Pathname("credits/pingus.credits", Pathname::DATA_PATH)));
      }
      else
      {
        ScreenManager::instance()->pop_screen();
      }
    }
  }
}

void
StoryScreen::resize(Size const& size_)
{
  GUIScreen::resize(size_);

  continue_button->set_pos(size.width()/2 + 220 + 40,
                           size.height()/2 + 180 +32);
  if (skip_button)
    skip_button->set_pos(size.width()  - 4,
                         size.height() - 26);
}

} // namespace pingus

/* EOF */
