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

#ifndef HEADER_PINGUS_PINGUS_LEVELSET_HPP
#define HEADER_PINGUS_PINGUS_LEVELSET_HPP

#include "engine/display/sprite.hpp"
#include "pingus/pingus_level.hpp"
#include "util/pathname.hpp"

namespace pingus {

class Levelset
{
public:
  struct Level
  {
    std::string resname;
    bool accessible;
    bool finished;
    PingusLevel plf;

    Level() :
      resname(),
      accessible(),
      finished(),
      plf()
    {}
  };

private:
  std::string m_title;
  std::string m_description;
  bool m_developer_only;
  float m_priority;
  Sprite m_sprite;
  int  m_completion;
  std::vector<std::unique_ptr<Level> > m_levels;

public:
  static std::unique_ptr<Levelset> from_file(Pathname const& pathname);
  static std::unique_ptr<Levelset> from_directory(std::string const& title,
                                                  std::string const& description,
                                                  std::string const& image,
                                                  Pathname const& pathname);

public:
  Levelset();
  ~Levelset();

  void set_title(std::string const& title);
  void set_description(std::string const& description);
  void set_image(std::string const& image);
  void set_developer_only(bool developer_only);
  void set_priority(float priority);
  void add_level(std::string const& resname, bool accessible = false);

  bool get_developer_only() const;
  float  get_priority() const;
  std::string get_title() const;
  std::string get_description() const;
  Sprite get_image() const;

  Level* get_level(int num) const;
  int get_level_count() const;

  /** Return the number of completed levels */
  int get_completion()  const;

  void refresh();
private:
  Levelset (Levelset const&);
  Levelset& operator= (Levelset const&);
};

} // namespace pingus

#endif

/* EOF */
