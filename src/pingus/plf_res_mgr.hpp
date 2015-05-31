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

#ifndef HEADER_PINGUS_PINGUS_PLF_RES_MGR_HPP
#define HEADER_PINGUS_PINGUS_PLF_RES_MGR_HPP

#include "pingus/pingus_level.hpp"
#include <SDL.h>

class Pathname;

class PLFResMgr
{
private:
  struct PLFEntry
  {
    PingusLevel plf;
    uint64_t mtime;

    PLFEntry() :
      plf(),
      mtime()
    {}
  };

  typedef std::map<std::string, PLFEntry> PLFMap;
  static  PLFMap plf_map;
  /** Loads PLF from filename and stores it under 'res_name' in the
      map */
  static PingusLevel load_plf_raw(const std::string& res_name,
                                  const Pathname& pathname);
public:
  /** @returns a handle to the PLF, which the caller *must not* delete

      @param res_name The resource name of the level, aka "snow11-grumbel"
  */
  static PingusLevel load_plf(const std::string& res_name);

  /** @return a handle to the PLF, instead of loading it from a
      res_name, load it from a system dependend filename

      @param filename The filename of the plf, aka "../data/levels/snow11-grumbel.pingus" */
  static PingusLevel load_plf_from_filename(const Pathname& filename);
};

#endif

/* EOF */
