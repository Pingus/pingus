// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_WORLDOBJ_FACTORY_HPP
#define HEADER_PINGUS_PINGUS_WORLDOBJ_FACTORY_HPP

#include <map>
#include <vector>

#include "util/reader.hpp"
#include "fwd.hpp"

namespace pingus {

/** WorldObjFactory which can be used to create all kinds of
    WorldObj's by given its id */
class WorldObjFactory
{
private:
  std::map<std::string, std::unique_ptr<WorldObjAbstractFactory> > factories;

  static std::unique_ptr<WorldObjFactory> instance_;

  WorldObjFactory();
  void free_factories();

public:
  /** Return the singleton instance */
  static WorldObjFactory& instance();
  static void deinit();
  /** Register a factory for object creation */
  void register_factory(std::string const& id,
                        std::unique_ptr<WorldObjAbstractFactory> factory);

  /** Create a WorldObj type from a given piece of xml, use the
      'type' property for determinating the object type. */
  std::vector<WorldObj*> create(ReaderObject const& reader);
  std::vector<WorldObj*> create(std::string const& id, ReaderMapping const& reader);

private:
  WorldObjFactory (WorldObjFactory const&);
  WorldObjFactory& operator= (WorldObjFactory const&);
};

} // namespace pingus

#endif

/* EOF */
