//  $Id: sprite.cxx,v 1.20 2003/12/13 16:23:39 grumbel Exp $
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
#include <iostream>
#include "vector.hxx"
#include "pingus_resource.hxx"
#include "sprite.hxx"
#include "math.hxx"
#include "gui/graphic_context.hxx"
#include "globals.hxx"

namespace Pingus {

Sprite::Sprite ()
{
}

Sprite::Sprite (std::string arg_sprite_name,
		std::string arg_datafile,
		float arg_frames_per_second,
		Sprite::Direction dir,
		LoopType arg_loop_type)
  : //sprite(PingusResource::load_sprite(arg_sprite_name, arg_datafile)),
    frame (0.0f),
    frames_per_second (arg_frames_per_second),
    direction (dir),
    looptype (arg_loop_type),
    is_finished (false),
    x_align (0), y_align (0)
{
}

void
Sprite::draw (int x, int y)
{
#ifdef CLANLIB_0_6
  // FIXME: HACK
  update (0.0f);

  switch (direction)
    {
    case Sprite::NONE:
      sprite.draw(x + x_align, y + y_align, Math::round(frame));
      break;
    case Sprite::LEFT:
      sprite.draw(x + x_align, y + y_align, Math::round(frame));
      break;
    case Sprite::RIGHT:
      sprite.draw (x + x_align, y + y_align, Math::round(frame) + max_frames ());
      break;
    default:
      std::cout << "Direction: " << direction << std::endl;
      assert(0);
    }
#endif
}

void
Sprite::draw (const Vector& pos)
{
  draw (int(pos.x), int(pos.y));
}

void
Sprite::draw(GraphicContext& gc, const Vector& pos)
{
#ifdef CLANLIB_0_6
  if (!sprite)
    return;

  // FIXME: HACK
  update (0.0f);

  int x = int(pos.x);
  int y = int(pos.y);

  switch (direction)
    {
    case Sprite::NONE:
    case Sprite::LEFT:
      gc.draw(sprite, x + x_align, y + y_align, Math::round(frame));
      break;

    case Sprite::RIGHT:
      gc.draw(sprite, x + x_align, y + y_align, Math::round(frame) + max_frames ());
      break;

    default:
      std::cout << "Direction: " << direction << std::endl;
      assert(0);
    }
#endif
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
#ifdef CLANLIB_0_6
  x_align = -int(sprite.get_width ())/2;
  y_align = -int(sprite.get_height ())/2;
#endif
}

void
Sprite::set_align_center_bottom ()
{
#ifdef CLANLIB_0_6
  x_align = -int(sprite.get_width ())/2;
  y_align = -int(sprite.get_height ());
#endif
}


void
Sprite::next_frame ()
{
#ifdef CLANLIB_0_6
  ++frame;

  if (Math::round(frame) >= int(sprite.get_frame_count()))
    frame = 0;
#endif
}

void
Sprite::previous_frame ()
{
#ifdef CLANLIB_0_6
  --frame;

  if (Math::round(frame) < 0)
    frame = sprite.get_frame_count() - 1;
#endif
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
#ifdef CLANLIB_0_6
  switch (direction)
    {
    case NONE:
      return sprite.get_frame_count();
    case LEFT:
    case RIGHT:
      return sprite.get_frame_count()/2;
    default:
      assert (0);
      return 0;
    }
#else
  return 0;
#endif
}

void
Sprite::update ()
{
  // FIXME: game_speed is incorrect, but should work
  update(game_speed / 1000.0f);
}

void
Sprite::update (float delta)
{
  if (!sprite)
    return;

  // The sprite contains no frames, so we have nothing to update
  if (max_frames () <= 1)
    return;

  switch (looptype)
    {
    case ENDLESS:
      frame += frames_per_second * delta;

      if (Math::round(frame) < 0) {
	std::cout << "frame below zero: " << frame << std::endl;
	frame  = (max_frames ()-1.0f);
      }

      if (Math::round(frame) >= max_frames ()) {
	is_finished = true;
	frame = 0;
      }
      break;

    case ONCE:
      frame += frames_per_second * delta;
      if (Math::round(frame) >= max_frames ())
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

CL_Sprite&
Sprite::get_sprite ()
{
  return sprite;
}

void
Sprite::set_frame (int n)
{
  frame = n;
}

int
Sprite::get_width ()
{
  if (!sprite)
    return 0;
  else
    return sprite.get_width();
}

int
Sprite::get_height ()
{
  if (!sprite)
    return 0;
  else
    return sprite.get_height();
}

} // namespace Pingus

/* EOF */
