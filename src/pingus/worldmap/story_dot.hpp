// Pingus - A free Lemmings clone
// Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_WORLDMAP_STORY_DOT_HPP
#define HEADER_PINGUS_PINGUS_WORLDMAP_STORY_DOT_HPP

#include <string>

#include <prio/fwd.hpp>

#include "engine/display/sprite.hpp"
#include "pingus/worldmap/dot.hpp"

namespace pingus::worldmap {

class StoryDot : public Dot
{
public:
  StoryDot(prio::ReaderMapping const& reader);

  void draw(DrawingContext& gc) override;
  void draw_hover(DrawingContext& gc) override;

  void update(float delta) override;
  void on_click() override;

  bool is_finished() const override { return true; }
  bool is_accessible() const override { return true; }
  void unlock() override {}

private:
  Sprite m_story_dot_highlight;
  Sprite m_story_dot;

  std::string m_name;
  std::string m_story;
  bool m_credits;

private:
  StoryDot(StoryDot const&);
  StoryDot& operator=(StoryDot const&);
};

} // namespace pingus::worldmap

#endif

/* EOF */
