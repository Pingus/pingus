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

#include "pingus/worldmap/level_dot.hpp"

#include "engine/display/drawing_context.hpp"
#include "engine/input/control.hpp"
#include "engine/screen/screen_manager.hpp"
#include "pingus/event_name.hpp"
#include "pingus/fonts.hpp"
#include "pingus/gettext.h"
#include "pingus/globals.hpp"
#include "pingus/plf_res_mgr.hpp"
#include "pingus/savegame_manager.hpp"
#include "pingus/screens/start_screen.hpp"

namespace pingus::worldmap {

LevelDot::LevelDot(ReaderMapping const& reader) :
  Dot(reader.get<ReaderMapping>("dot")),
  green_dot_sur("core/worldmap/dot_green"),
  red_dot_sur("core/worldmap/dot_red"),
  inaccessible_dot_sur("core/worldmap/dot_invalid"),
  highlight_green_dot_sur("core/worldmap/dot_green_hl"),
  highlight_red_dot_sur("core/worldmap/dot_red_hl"),
  plf()
{
  std::string resname;
  reader.read("levelname", resname);

  plf = PLFResMgr::load_plf(resname);
}

void
LevelDot::draw(DrawingContext& gc)
{
  Savegame* const savegame = SavegameManager::instance()->get(plf.get_resname());
  if (savegame && (savegame->get_status() == Savegame::FINISHED ||
                   savegame->get_status() == Savegame::ACCESSIBLE))
  {
    if (savegame->get_status() == Savegame::FINISHED) {
      if (m_highlight) {
        gc.draw(highlight_green_dot_sur, m_pos, m_z_index);
      } else {
        gc.draw(green_dot_sur, m_pos, m_z_index);
      }
    } else {
      if (m_highlight) {
        gc.draw(highlight_red_dot_sur, m_pos, m_z_index);
      } else {
        gc.draw(red_dot_sur, m_pos, m_z_index);
      }
    }
  }
  else
  {
    gc.draw(inaccessible_dot_sur, m_pos, m_z_index);
  }
}

void
LevelDot::draw_hover(DrawingContext& gc)
{
  if (is_accessible())
  {
    gc.print_center(pingus::fonts::pingus_small,
                    Vector2i(static_cast<int>(m_pos.x()),
                             static_cast<int>(m_pos.y()) - 44),
                    _(get_plf().get_levelname()),
                    10000);
  }
  else
  {
    gc.print_center(pingus::fonts::pingus_small,
                    Vector2i(static_cast<int>(m_pos.x()),
                             static_cast<int>(m_pos.y()) - 44),
                    _("???"),
                    10000);
  }

  if (globals::developer_mode)
  {
    gc.print_center(pingus::fonts::pingus_small,
                    Vector2i(static_cast<int>(m_pos.x()), static_cast<int>(m_pos.y()) - 70),
                    get_plf().get_resname(),
                    10000);
  }
}

void
LevelDot::update(float delta)
{
}

void
LevelDot::on_click()
{
  ScreenManager::instance()->push_screen(std::make_shared<StartScreen>(plf));
}

bool
LevelDot::is_finished() const
{
  Savegame* savegame = SavegameManager::instance()->get(plf.get_resname());
  return savegame && savegame->get_status() == Savegame::FINISHED;
}

bool
LevelDot::is_accessible() const
{
  Savegame* const savegame = SavegameManager::instance()->get(plf.get_resname());
  return savegame && savegame->get_status() != Savegame::NONE;
}

void
LevelDot::unlock()
{
  Savegame* const savegame = SavegameManager::instance()->get(plf.get_resname());
  if (savegame == nullptr || savegame->get_status() == Savegame::NONE)
  {
    Savegame savegame_(plf.get_resname(),
                       Savegame::ACCESSIBLE,
                       0,
                       0);
    SavegameManager::instance()->store(savegame_);
  }
}

} // namespace pingus::worldmap

/* EOF */
