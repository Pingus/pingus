//  $Id: plf_res_mgr.hxx,v 1.4 2003/06/04 17:22:33 torangan Exp $
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

#ifndef HEADER_PINGUS_PLF_RES_MGR_HXX
#define HEADER_PINGUS_PLF_RES_MGR_HXX

#include "plf_handle.hxx"

/** */
class PLFResMgr
{
private:
  struct PLFEntry {
    PLF* plf;
    unsigned int mtime;
  };

  typedef std::map<std::string, PLFEntry> PLFMap;
  static  PLFMap plf_map;
  /** Loads PLF from filename and stores it under 'res_name' in the
      map */
  static PLFHandle load_plf_raw(const std::string& res_name,
                                const std::string& filename);
public:
  /** @returns a handle to the PLF, which the caller *must not* delete

      @param res_name The resource name of the level, aka "snow11-grumbel"
   */
  static PLFHandle load_plf(const std::string& res_name);

  /** @return a handle to the PLF, instead of loading it from a
      res_name, load it from a system dependend filename

      @param filename The filename of the plf, aka "../data/levels/snow11-grumbel.xml" */
  static PLFHandle load_plf_from_filename(const std::string& filename);
  static  void free_plf_map();
};

#endif

/* EOF */
