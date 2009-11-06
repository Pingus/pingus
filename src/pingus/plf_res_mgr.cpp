//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pingus/plf_res_mgr.hpp"

#include "pingus/debug.hpp"
#include "pingus/globals.hpp"
#include "util/pathname.hpp"
#include "util/system.hpp"

PLFResMgr::PLFMap PLFResMgr::plf_map;

PingusLevel
PLFResMgr::load_plf_raw(const std::string& res_name,
                        const Pathname& pathname)
{
  pout(PINGUS_DEBUG_LOADING) << "PLFResMgr: '" << res_name << "'  -> '" << pathname.str() << "'" << std::endl;

  PLFMap::iterator i = plf_map.find(res_name);

  if (i == plf_map.end())
    { // Entry not cached, so load it and add it to cache
      pout(PINGUS_DEBUG_LOADING) << "PLFResMgr: Loading level from DISK: '" << res_name << "' -> '"
                                 << pathname.str() << "'" << std::endl;

      PingusLevel plf(res_name, pathname);

      PLFEntry entry;
      entry.plf   = plf;
      entry.mtime = pathname.mtime();

      plf_map[res_name]  = entry;

      // FIXME: leaking pointers to the outsite work is not such a good
      // idea, could lead to trouble sooner or later

      return PingusLevel (entry.plf);
    }
  else
    {
      uint64_t current_mtime = pathname.mtime();
      if (current_mtime != i->second.mtime)
        {
          pout(PINGUS_DEBUG_LOADING) << "PLFResMgr: level changed on DISK, reloading: '" << res_name
                                     << "' -> '" << pathname.str() << "'" << std::endl;

          // Reload the file since it has changed on disk
          PingusLevel plf(res_name, pathname);
          PLFEntry entry;

          entry.plf   = plf;
          entry.mtime = pathname.mtime();

          plf_map[res_name]  = entry;

          // FIXME: leaking pointers to the outsite work is not such a good
          // idea, could lead to trouble sooner or later
          return PingusLevel (entry.plf);
        }
      else
        { // File in cache is up to date, everything is all ready, return it
          pout(PINGUS_DEBUG_LOADING) << "PLFResMgr: Loading level from CACHE: '" << res_name << "' -> '"
                                     << pathname.str() << "'" << std::endl;

          return i->second.plf;
        }
    }
}

PingusLevel
PLFResMgr::load_plf_from_filename(const Pathname& pathname)
{
  // FIXME: Ugly resname guessing is ugly
  std::string res_name = System::basename(pathname.get_sys_path());

  // This should give us the tutorial/, wip/, etc. part of the res_name
  std::string dirname  = System::basename(System::dirname(pathname.get_sys_path()));

  return load_plf_raw(dirname + "/" + res_name.substr(0, res_name.length()-7),
                      pathname);
}

PingusLevel
PLFResMgr::load_plf(const std::string& res_name)
{
  return load_plf_raw(res_name, Pathname("levels/" + res_name + ".pingus", Pathname::DATA_PATH));
}


/* EOF */
