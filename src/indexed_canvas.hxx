//  $Id: indexed_canvas.hxx,v 1.1 2002/10/12 00:24:26 grumbel Exp $
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

#ifndef HEADER_PINGUS_INDEXED_CANVAS_HXX
#define HEADER_PINGUS_INDEXED_CANVAS_HXX

#include <ClanLib/Display/SurfaceProviders/generic_surfaceprovider.h>
#include <ClanLib/Display/Display/palette.h>

/** This class is analog to CL_Canvas, but instead of being true
    color, it is indexed */
class IndexedCanvas : public CL_SurfaceProvider_Generic
{
private:
  unsigned int width;
  unsigned int height;
  unsigned char* data;
  CL_Palette palette;
public:
  IndexedCanvas(int width, int height);
  ~IndexedCanvas();

  bool is_indexed() const { return true; }
  unsigned int get_width()  const { return width; }
  unsigned int get_pitch()  const { return width; }
  unsigned int get_height() const { return height; }

  unsigned int   get_num_frames() const { return 1; }
  void* get_data() const { return data; }
  void  set_palette(CL_Palette*);
  CL_Palette* get_palette() const;

  void perform_lock() {}
  void perform_unlock() {}

  bool         uses_src_colorkey() const { return false; }
  unsigned int get_src_colorkey() const { return 0; }

  unsigned int get_red_mask() const { return 0; }
  unsigned int get_green_mask() const { return 0; }
  unsigned int get_blue_mask() const { return 0; }
  unsigned int get_alpha_mask() const { return 0; }
private:
  IndexedCanvas (const IndexedCanvas&);
  IndexedCanvas& operator= (const IndexedCanvas&);
};

#endif

/* EOF */
