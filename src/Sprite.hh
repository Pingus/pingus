//  $Id: Sprite.hh,v 1.1 2001/03/31 09:54:51 grumbel Exp $
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

#ifndef SPRITE_HH
#define SPRITE_HH

#include <ClanLib/display.h>

class Sprite
{
private:
  CL_Surface sur;
  int frame;

public:
  //  enum {     };

  Sprite (std::string arg_sur_name,
	  std::string arg_datafile,
	  int arg_loop_type = 0);

  Sprite (const CL_Surface& sur,
	  int arg_loop_type = 0);

  void put_screen (int x, int y);
  void put_screen (const CL_Vector& pos);

  void next_frame ();
  void previous_frame ();
};

#endif

/* EOF */
