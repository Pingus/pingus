//  $Id: Background.hh,v 1.6 2000/07/30 01:47:35 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef BACKGROUND_HH
#define BACKGROUND_HH

#include <ClanLib/core.h>

#include "AnimCounter.hh"
#include "GameCounter.hh"
#include "ResDescriptor.hh"
#include "BackgroundData.hh"

///
class Background : public BackgroundData
{
private:
  ///
  bool surface_need_deletion;
  ///
  CL_Surface* bg_surface_raw;
  ///
  CL_Surface* bg_surface;
  ///
  AnimCounter scroll;
  ///
  GameCounter counter;
  ///
  int x1, x2, y1, y2;
  ///
  float scroll_ox, scroll_oy;
public:
  ///
  Background(BackgroundData);
  ///
  ~Background();

  ///
  void load (BackgroundData);
  
  ///
  void let_move(void);
  ///
  void draw_offset(int x_of, int y_of, float s = 1.0);
}///
;

#endif

/* EOF */
