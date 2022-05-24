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

#ifndef HEADER_PINGUS_PINGUS_SCREENS_STORY_SCREEN_HPP
#define HEADER_PINGUS_PINGUS_SCREENS_STORY_SCREEN_HPP

#include <memory>

#include "engine/screen/gui_screen.hpp"
#include "pingus/worldmap/worldmap_story.hpp"

class StoryScreenComponent;

namespace pingus::worldmap {
class WorldmapStory;
} // namespace pingus::worldmap

namespace pingus::gui {
class SurfaceButton;
}

class StoryScreen : public GUIScreen
{
private:
  std::unique_ptr<pingus::worldmap::WorldmapStory> story;
  StoryScreenComponent* story_comp;
  pingus::gui::SurfaceButton* continue_button;
  pingus::gui::SurfaceButton* skip_button;
  bool m_credits;

public:
  StoryScreen(const prio::ReaderMapping& reader, bool credits = false);
  ~StoryScreen() override;

  void on_startup() override;
  void on_fast_forward_press () override;
  void on_escape_press () override;

  void resize(const Size& size) override;

private:
  StoryScreen (const StoryScreen&);
  StoryScreen& operator= (const StoryScreen&);
};

#endif

/* EOF */
