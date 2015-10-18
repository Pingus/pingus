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

#include "pingus/worldmap/story_dot.hpp"

#include "engine/display/drawing_context.hpp"
#include "engine/screen/screen_manager.hpp"
#include "pingus/fonts.hpp"
#include "pingus/gettext.h"
#include "pingus/screens/story_screen.hpp"
#include "util/reader.hpp"
#include "util/reader.hpp"
#include "util/log.hpp"
#include "util/pathname.hpp"

namespace WorldmapNS {

StoryDot::StoryDot(const ReaderMapping& reader) :
  Dot(reader.read_mapping("dot")),
  m_story_dot_highlight("core/worldmap/story_dot_highlight"),
  m_story_dot("core/worldmap/story_dot"),
  m_name(),
  m_story(),
  m_credits(false)
{
  reader.read_string("name", m_name);
  reader.read_string("story", m_story);
  reader.read_bool("credits", m_credits);
}

void
StoryDot::draw(DrawingContext& gc)
{
  gc.draw (m_story_dot, pos);
}

void
StoryDot::draw_hover(DrawingContext& gc)
{
  gc.draw (m_story_dot_highlight, pos);

  gc.print_center(Fonts::pingus_small,
                  Vector2i(static_cast<int>(pos.x),
                           static_cast<int>(pos.y) - 44),
                  _(m_name),
                  10000);
}

void
StoryDot::update(float delta)
{
}

void
StoryDot::on_click()
{
  try
  {
    ReaderObject reader = Reader::parse(Pathname(m_story, Pathname::DATA_PATH));
    ScreenManager::instance()->push_screen(std::make_shared<StoryScreen>(reader.get_mapping(), m_credits));
  }
  catch(const std::exception& err)
  {
    log_error("%1%", err.what());
  }
}

} // namespace WorldmapNS

/* EOF */
