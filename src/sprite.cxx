//  $Id: sprite.cxx,v 1.5 2002/08/23 15:49:51 torangan Exp $
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

#include <assert.h>
#include <ClanLib/Core/Math/cl_vector.h>
#include "pingus_resource.hxx"
#include "sprite.hxx"

int round(float f) 
{
  if (f >= 0.0f)
    return int(f + 0.5f);
  else
    return int(f - 0.5f);
}

Sprite::Sprite ()
{
}

Sprite::Sprite (const Sprite& sprite) :
                frame (sprite.frame),
                frames_per_second (sprite.frames_per_second),
                sur (sprite.sur),
                direction (sprite.direction),
                looptype (sprite.looptype),
                is_finished (sprite.is_finished),
                x_align (sprite.x_align),
                y_align (sprite.y_align)
{
}

Sprite Sprite::operator= (const Sprite& sprite)
{
  if (this == &sprite)
    return *this;
    
  frame             = sprite.frame;
  frames_per_second = sprite.frames_per_second;
  sur               = sprite.sur;
  direction         = sprite.direction;
  looptype          = sprite.looptype;
  is_finished       = sprite.is_finished;
  x_align           = sprite.x_align;
  y_align           = sprite.y_align;
  
  return *this;
}

Sprite::Sprite (std::string arg_sur_name,
		std::string arg_datafile,
		float arg_frames_per_second,
		Sprite::Direction dir,
		LoopType arg_loop_type)
  : frame (0.0f),
    frames_per_second (arg_frames_per_second),
    sur(PingusResource::load_surface (arg_sur_name, arg_datafile)),
    direction (dir),
    looptype (arg_loop_type),
    is_finished (false),
    x_align (0), y_align (0)
{
}

Sprite::Sprite (const CL_Surface& arg_sur,
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
  sur = arg_sur;
}

Sprite::Sprite (const ResDescriptor& desc,
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
  sur = PingusResource::load_surface (desc);
}

void 
Sprite::put_screen (int x, int y)
{
  if (!sur)
    return;
  // FIXME: HACK
  update (0.0f);
  //std::cout << "Frame: " << round(frame) << " " << frame << " " << max_frames () << std::endl;

  switch (direction)
    {
    case Sprite::NONE:
      sur.put_screen (x + x_align, y + y_align, round(frame));
      break;
    case Sprite::LEFT:
      sur.put_screen (x + x_align, y + y_align, round(frame));
      break;
    case Sprite::RIGHT:
      sur.put_screen (x + x_align, y + y_align, round(frame) + max_frames ());
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
  x_align = -int(sur.get_width ())/2;
  y_align = -int(sur.get_height ())/2;
}

void 
Sprite::set_align_center_bottom ()
{
  x_align = -int(sur.get_width ())/2;
  y_align = -int(sur.get_height ()); 
}

void 
Sprite::next_frame ()
{
  ++frame;

  if (round(frame) >= int(sur.get_num_frames ()))
    frame = 0;
}

void 
Sprite::previous_frame ()
{
  --frame;

  if (round(frame) < 0)
    frame = sur.get_num_frames () - 1;  
}

int 
Sprite::get_frame ()
{
  return int(frame);
}

float 
Sprite::get_progress ()
{
  return float(frame)/max_frames ();
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
  if (!sur)
    return;

  // The sprite contains no frames, so we have nothing to update
  if (max_frames () <= 1)
    return;

  switch (looptype)
    {
    case ENDLESS:
      frame += frames_per_second * delta;

      if (round(frame) < 0) {
	std::cout << "frame below zero: " << frame << std::endl;
	frame  = (max_frames ()-1.0f);
      }

      if (round(frame) >= max_frames ()) {
	is_finished = true;
	frame = 0;
      }
      break;
      
    case ONCE:
      frame += frames_per_second * delta;
      if (round(frame) >= max_frames ()) 
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
      return is_finished;

    case ONCE:
      return is_finished;

    default:
      assert (!"Wrong looptype!");
      return false;
    }
}

void 
Sprite::reset ()
{
  frame = 0.0f;
  is_finished = false;
}

CL_Surface&
Sprite::get_surface ()
{
  return sur;
}

/* EOF */


