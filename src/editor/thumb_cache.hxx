//  $Id: thumb_cache.hxx,v 1.8 2002/09/27 11:26:46 torangan Exp $
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

#ifndef HEADER_PINGUS_EDITOR_THUMB_CACHE_HXX
#define HEADER_PINGUS_EDITOR_THUMB_CACHE_HXX

#include "../pingus.hxx"
#include <string>

class CL_Surface;

namespace EditorNS {

/** This class is used inside the SurfaceSelector to load thumbnails
    of surfaces for selection */
class ThumbCache
{
public:
  /** Load a surface from its source, instead of from the cache */
  static CL_Surface uncached_load (const std::string & res_ident, const std::string & datafile);

  /** Load a thumbnail of a surface out of the cache. The thumbnail has
      the size 50x50, if the thumbnail is not available, the surface
      is loaded and added to the cache. */
  static CL_Surface load (const std::string & res_ident, const std::string & datafile);

  /** Add a surface as thumbnail to the cache, the surface will be
      resized to a 50x50 size, the return value is the resized surface */
  static CL_Surface cache (const CL_Surface&, const std::string & res_ident, const std::string & datafile);
  
private:
  ThumbCache (const ThumbCache&);
  ThumbCache& operator= (const ThumbCache&);  
};

} // namespace EditorNS

#endif

/* EOF */
