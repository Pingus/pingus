// $Id: background_data.hh,v 1.5 2000/06/18 22:19:48 grumbel Exp $
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

#ifndef BACKGROUND_DATA_HH
#define BACKGROUND_DATA_HH

#include "ResDescriptor.hh"

///
class background_data
{
public:
  ///
  ResDescriptor desc;
  ///
  float para_x;
  ///
  float para_y;

  /** The amount of pixel the background is scrolled each frame in x
      direction. */
  float scroll_x;

  /** The amount of pixel the background is scrolled each frame in x
      direction. */
  float scroll_y;

  /**@name fill_rect() components
     An fill_rect() can be drawn over the background, the following
     for components are passed to the fill_rect() call. */
  //@{
  /// The red component of the fill_rect() call
  float red;
  /// The green component of the fill_rect() call
  float green;
  /// The blue component of the fill_rect() call
  float blue;
  /// The alpha component of the fill_rect() call
  float alpha;
  //@}

  /// Stretch the background to the full screen size in x direction
  bool stretch_x;

  /// Stretch the background to the full screen size in x direction
  bool stretch_y;

  /// Init all fields with some usefull defaults values.
  background_data();
};

#endif

/* EOF */
