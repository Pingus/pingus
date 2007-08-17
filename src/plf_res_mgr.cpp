//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "globals.hpp"
#include "debug.hpp"
#include "system.hpp"
#include "path_manager.hpp"
#include "plf_res_mgr.hpp"


PLFResMgr::PLFMap PLFResMgr::plf_map;

PingusLevel
PLFResMgr::load_plf_raw(const std::string& res_name,
                        const std::string& filename)
{
  pout(PINGUS_DEBUG_LOADING) << "PLFResMgr: '" << res_name << "'  -> '" << filename << "'" << std::endl;

  PLFMap::iterator i = plf_map.find(res_name);

  if (i == plf_map.end())
    { // Entry not cached, so load it and add it to cache
      pout(PINGUS_DEBUG_LOADING) << "PLFResMgr: Loading level from DISK: '" << res_name << "' -> '" << filename << "'" << std::endl;

      PingusLevel plf(res_name, filename);

      PLFEntry entry;
      entry.plf   = plf;
      entry.mtime = System::get_mtime(filename);

      plf_map[res_name]  = entry;

      // FIXME: leaking pointers to the outsite work is not such a good
      // idea, could lead to trouble sooner or later

      return PingusLevel (entry.plf);
    }
  else
    {
      uint64_t current_mtime = System::get_mtime(filename);
      if (current_mtime != i->second.mtime)
        {
          pout(PINGUS_DEBUG_LOADING) << "PLFResMgr: level changed on DISK, reloading: '" << res_name << "' -> '" << filename << "'" << std::endl;

          // Reload the file since it has changed on disk
          PingusLevel plf(res_name, filename);
          PLFEntry entry;

          entry.plf   = plf;
          entry.mtime = System::get_mtime(filename);

          plf_map[res_name]  = entry;

          // FIXME: leaking pointers to the outsite work is not such a good
          // idea, could lead to trouble sooner or later
          return PingusLevel (entry.plf);
        }
      else
        { // File in cache is up to date, everything is all ready, return it
          pout(PINGUS_DEBUG_LOADING) << "PLFResMgr: Loading level from CACHE: '" << res_name << "' -> '" << filename << "'" << std::endl;

          return i->second.plf;
        }
    }
}

PingusLevel
PLFResMgr::load_plf_from_filename(const std::string& filename)
{
  std::string res_name = System::basename(filename);

  // This should give us the tutorial/, wip/, etc. part of the res_name
  std::string dirname  = System::basename(System::dirname(filename));

  return load_plf_raw(dirname + "/" + res_name.substr(0, res_name.length()-4),
                      filename);
}

PingusLevel
PLFResMgr::load_plf(const std::string& res_name)
{
  return load_plf_raw(res_name, path_manager.complete("levels/" + res_name + ".pingus"));
}


/* EOF */
