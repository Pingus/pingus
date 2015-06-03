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

namespace WorldmapNS {
class WorldmapStory;
} // namespace WorldmapNS

namespace GUI {
class SurfaceButton;
}

class StoryScreen : public GUIScreen
{
private:
  std::unique_ptr<WorldmapNS::WorldmapStory> story;
  StoryScreenComponent* story_comp;
  GUI::SurfaceButton* continue_button;
  GUI::SurfaceButton* skip_button;
  bool m_credits;

public:
  StoryScreen(const ReaderMapping& reader, bool credits = false);
  ~StoryScreen();

  void on_startup();
  void on_fast_forward_press ();
  void on_escape_press ();

  void resize(const Size& size);

private:
  StoryScreen (const StoryScreen&);
  StoryScreen& operator= (const StoryScreen&);
};

#endif

/* EOF */
