//  $Id: preview_renderer.hxx,v 1.2 2003/10/18 23:17:27 grumbel Exp $
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

#ifndef HEADER_PREVIEW_RENDERER_HXX
#define HEADER_PREVIEW_RENDERER_HXX

#include "plf_res_mgr.hxx"

namespace Pingus {

/** By given a PLF, the PreviewRenderer renders a CL_Canvas with a
    graphic of the complete level, it also allows to save it down to a
    file */
class PreviewRenderer
{
private:
public:
  /** Render the level given by levelfile to as pnm to the file given
   *  by output_file
   *
   *  @param plf          PLF that describes the level
   *  @param output_file  filename of the file to which the result should be written
   */
  static void render(const PingusLevel& plf, const std::string& output_file); 
};

} // namespace Pingus

#endif

/* EOF */
