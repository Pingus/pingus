//  $Id: ThumbCache.hh,v 1.3 2001/03/18 17:45:05 grumbel Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef THUMBCACHE_HH
#define THUMBCACHE_HH

#include <string>
#include <ClanLib/core.h>
#include <ClanLib/display.h>

/** This class is used inside the SurfaceSelector to load thumbnails
    of surfaces for selection */
class ThumbCache
{
private:
  
public:
  /** Load a thumnail of a surface out of the cache. The thumbnail as
      the size 50x50, if the thumbnail is not available, the surface
      is loaded and added to the cache. */
  static CL_Surface load (std::string res_ident, std::string datafile);

  /** Add a surface as thumbnail to the cache, the surface will be
      resized to a 50x50 size */
  static void        cache (const CL_Surface&, std::string res_ident, std::string datafile);
};

#endif

/* EOF */
