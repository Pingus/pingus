//  $Id: Sprite.cc,v 1.5 2001/04/06 15:04:45 grumbel Exp $
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

#include "PingusResource.hh"
#include "Sprite.hh"

Sprite::Sprite (std::string arg_sur_name,
		std::string arg_datafile,
		float arg_frames_per_second,
		Sprite::Direction dir,
		LoopType arg_loop_type)
  : frame (0.0f),
    frames_per_second (arg_frames_per_second),
    direction (dir),
    looptype (arg_loop_type),
    is_finished (false),
    x_align (0), y_align (0)
{
  sur = PingusResource::load_surface (arg_sur_name, arg_datafile);
}

Sprite::Sprite (const CL_Surface& arg_sur,
		float arg_frames_per_second = 10.0f,
		Sprite::Direction dir,
		LoopType arg_loop_type)
  : frame (0.0f),
    frames_per_second (arg_frames_per_second),
    direction (dir),
    looptype (arg_loop_type),
    is_finished (false),
    x_align (0), y_align (0)
{
  sur = arg_sur;
}

void 
Sprite::put_screen (int x, int y)
{
  switch (direction)
    {
    case Sprite::NONE:
      sur.put_screen (x + x_align, y + y_align, int(frame));
      break;
    case Sprite::LEFT:
      sur.put_screen (x + x_align, y + y_align, int(frame));
      break;
    case Sprite::RIGHT:
      sur.put_screen (x + x_align, y + y_align, int(frame) + max_frames ());
      break;
    default:
      std::cout << "Direction: " << direction << std::endl;
      assert(0);
    }
}

void 
Sprite::put_screen (const CL_Vector& pos)
{
  put_screen (int(pos.x), int(pos.y));
}

void 
Sprite::set_align (int arg_x, int arg_y)
{
  x_align = arg_x;
  y_align = arg_y;
}

void
Sprite::set_align_center ()
{
  x_align = sur.get_width ()/2;
  y_align = sur.get_height ()/2;
}

void 
Sprite::next_frame ()
{
  ++frame;

  if (frame >= int(sur.get_num_frames ()))
    frame = 0;
}

void 
Sprite::previous_frame ()
{
  --frame;

  if (frame < 0)
    frame = sur.get_num_frames () - 1;  
}

int 
Sprite::max_frames ()
{
  switch (direction)
    {
    case NONE:
      return sur.get_num_frames ();
    case LEFT:
    case RIGHT:
      return sur.get_num_frames ()/2;
    default:
      assert (0);
    }
}

void
Sprite::update (float delta)
{
  switch (looptype)
    {
    case ENDLESS:
      frame += frames_per_second * delta;

      if (frame < 0) {
	std::cout << "frame below zero: " << frame << std::endl;
	frame  = 0;
      }

      while (frame >= max_frames ())
	{
	  frame -= (max_frames ()-1);
	}   
      
      break;
    case ONCE:
      frame += frames_per_second * delta;
      if (frame >= max_frames ()) 
	{
	  is_finished = true;
	  frame = max_frames () - 1;
	}
      
      break;
    }
}

bool 
Sprite::finished ()
{
  switch (looptype)
    {
    case ENDLESS:
      return false;

    case ONCE:
      return is_finished;

    default:
      assert (!"Wrong looptype!");
      return false;
    }
}

/* EOF */
