//  $Id: plf_res_mgr.cxx,v 1.1 2003/03/04 10:26:18 grumbel Exp $
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

#include "plf.hxx"
#include "globals.hxx"
#include "debug.hxx"
#include "system.hxx"
#include "path_manager.hxx"
#include "plf_res_mgr.hxx"

PLFResMgr::PLFMap PLFResMgr::plf_map;

PLFHandle 
PLFResMgr::load_plf_raw(const std::string& res_name,
                        const std::string& filename)
{
  pout(PINGUS_DEBUG_LOADING) << "PLFResMgr: Loading level from disk: '" << res_name << "' -> '" << filename << "'" << std::endl;

  PLF* plf = PLF::create(filename);
  
  plf_map[res_name]  = plf;

  return PLFHandle (plf);
}

PLFHandle
PLFResMgr::load_plf_from_filename(const std::string& filename)
{
  // FIXME: This should reload levels if they have changed on disk

  // FIXME: Might cut of the 'playable/' or 'wip/' directories, not so
  // good. saving the PLF's under filenames would also lead to
  // throuble, since multiple filenames can map to the same file
  // (../../data/levels/bla.xml, ../levels/bla.xml, etc.)
  std::string res_name = System::basename(filename); 
  res_name = res_name.substr(0, res_name.length()-4);
  
  PLFMap::iterator i = plf_map.find(res_name);

  if (i == plf_map.end ())
    {
      return load_plf_raw (res_name, filename);
    }
  else
    {
      pout(PINGUS_DEBUG_LOADING) 
        << "PLFResMgr: Loading level from cache: '" << res_name << "' -> '" << filename << "'" << std::endl;
      return PLFHandle (i->second);
    }
}

PLFHandle
PLFResMgr::load_plf(const std::string& res_name)
{
  std::string filename = path_manager.complete("levels/" + res_name + ".xml");
  PLFMap::iterator i   = plf_map.find(res_name);

  if (i == plf_map.end ())
    {
      pout(PINGUS_DEBUG_LOADING) 
        << "PLFResMgr: Loading level from cache: '" << res_name << "' -> '" << filename << "'" << std::endl;
      return load_plf_raw (res_name, filename);
    }
  else
    {
      return PLFHandle (i->second);
    }
}

/* EOF */
