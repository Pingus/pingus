// Pingus - A free Lemmings clone
// Copyright (C) 2007 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/levelset.hpp"

#include <stdexcept>

#include <logmich/log.hpp>

#include "math/math.hpp"
#include "pingus/plf_res_mgr.hpp"
#include "pingus/globals.hpp"
#include "pingus/savegame_manager.hpp"
#include "util/raise_exception.hpp"

namespace pingus {

std::unique_ptr<Levelset>
Levelset::from_directory(std::string const& title,
                         std::string const& description,
                         std::string const& image,
                         Pathname const& pathname)
{
  auto levelset = std::make_unique<Levelset>();

  levelset->set_title(title);
  levelset->set_description(description);
  levelset->set_image(image);

  std::vector<Pathname> files = pathname.opendir_recursive();
  for(auto it = files.begin(); it != files.end(); ++it)
  {
    if (it->has_extension(".pingus"))
    {
      std::string resname = it->get_raw_path();

      // FIXME: we need the resname, not the filename, so we do some
      // ugly mess, cutting away "levels/" and the ".pingus" suffix
      resname = resname.substr(7, resname.size() - 7 - 7);

      levelset->add_level(resname, true);
    }
  }

  return levelset;
}

std::unique_ptr<Levelset>
Levelset::from_file(Pathname const& pathname)
{
  auto doc = ReaderDocument::from_file(pathname.get_sys_path());
  if (doc.get_name() != "pingus-levelset")
  {
    raise_exception(std::runtime_error, "Error: " << pathname.str() << ": not a 'pingus-levelset' file");
  }
  else
  {
    ReaderMapping reader = doc.get_mapping();
    auto levelset = std::make_unique<Levelset>();

    std::string tmp;
    if (reader.read("title", tmp))
    {
      levelset->set_title(tmp);
    }

    if (reader.read("description", tmp))
    {
      levelset->set_description(tmp);
    }

    if (reader.read("image", tmp))
    {
      levelset->set_image(tmp);
    }

    bool tmp_bool;
    if (reader.read("developer-only", tmp_bool))
    {
      levelset->set_developer_only(tmp_bool);
    }

    float tmp_float;
    if (reader.read("priority", tmp_float))
    {
      levelset->set_priority(tmp_float);
    }

    bool locked = true;
    reader.read("locked", locked);

    // skip level loading when levels won't be used
    if (!levelset->get_developer_only() || globals::developer_mode)
    {
      ReaderCollection level_reader;
      reader.read("levels", level_reader);
      std::vector<ReaderObject> sections = level_reader.get_objects();
      for(auto i = sections.begin(); i != sections.end(); ++i)
      {
        if (i->get_name() == "level")
        {
          ReaderMapping mapping = i->get_mapping();

          if (!mapping.read("filename", tmp))
          {
            log_error("Levelset: {} is missing filename tag", pathname.str());
          }
          else
          {
            levelset->add_level(tmp, !locked);
          }
        }
      }
    }

    levelset->refresh();

    return levelset;
  }
}

Levelset::Levelset() :
  m_title(),
  m_description(),
  m_developer_only(false),
  m_priority(0),
  m_sprite(),
  m_completion(0),
  m_levels()
{
}

Levelset::~Levelset()
{
}

void
Levelset::set_title(std::string const& title)
{
  m_title = title;
}

void
Levelset::set_description(std::string const& description)
{
  m_description = description;
}

void
Levelset::set_image(std::string const& image)
{
  m_sprite = Sprite(image);
}

void
Levelset::set_developer_only(bool developer_only)
{
  m_developer_only = developer_only;
}

void
Levelset::set_priority(float priority)
{
  m_priority = priority;
}

void
Levelset::add_level(std::string const& resname, bool accessible)
{
  try
  {
    auto level = std::make_unique<Level>();

    level->resname    = resname;
    level->plf        = PLFResMgr::load_plf(level->resname);

    level->accessible = accessible;
    level->finished   = false;

    m_levels.push_back(std::move(level));
  }
  catch(std::exception const& err)
  {
    log_error("failed to load: {}: {}", resname, err.what());
  }
}

std::string
Levelset::get_title() const
{
  return m_title;
}

std::string
Levelset::get_description() const
{
  return m_description;
}

Levelset::Level*
Levelset::get_level(int num) const
{
  if (num >= 0 && num < int(m_levels.size()))
  {
    return m_levels[static_cast<size_t>(num)].get();
  }
  else
  {
    return nullptr;
  }
}

int
Levelset::get_level_count() const
{
  return static_cast<int>(m_levels.size());
}

int
Levelset::get_completion()  const
{
  return m_completion;
}

Sprite
Levelset::get_image() const
{
  return m_sprite;
}

bool
Levelset::get_developer_only() const
{
  return m_developer_only;
}

float
Levelset::get_priority() const
{
  return m_priority;
}

void
Levelset::refresh()
{
  for(auto it = m_levels.begin(); it != m_levels.end(); ++it)
  {
    Savegame* savegame = SavegameManager::instance()->get((*it)->resname);

    if (savegame)
    {
      (*it)->accessible = (savegame->get_status() != Savegame::NONE);
      (*it)->finished   = (savegame->get_status() == Savegame::FINISHED);
    }
  }

  // unlock the next level
  if (!m_levels.empty())
  {
    m_levels[0]->accessible = true;
    for(size_t i = 0; i < m_levels.size()-1; ++i)
    {
      if (m_levels[i]->finished)
      {
        m_levels[i+1]->accessible = true;
      }
    }
  }

  // update completion count
  m_completion = 0;
  for(auto it = m_levels.begin(); it != m_levels.end(); ++it)
  {
    if ((*it)->finished)
    {
      m_completion += 1;
    }
  }

  if (!m_levels.empty())
  {
    m_completion = std::clamp(m_completion * 100 / int(m_levels.size()), 0, 100);
  }
}

} // namespace pingus

/* EOF */
