//  $Id: Sprite.hh,v 1.6 2001/04/10 10:45:14 grumbel Exp $
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
public:
  enum Direction { NONE, LEFT, RIGHT };
  enum LoopType { ENDLESS, ONCE, };
  
private:
  float frame;
  float frames_per_second;
  CL_Surface sur;
  Direction direction;
  LoopType looptype;
  bool is_finished;

  int x_align;
  int y_align;
  
  int max_frames ();
  
public:
  Sprite (const Sprite& sprite);
  
  Sprite (std::string arg_sur_name,
	  std::string arg_datafile,
	  float arg_frames_per_second = 10.0f,
	  Sprite::Direction dir = NONE,
	  LoopType arg_loop_type = ENDLESS);

  Sprite (const CL_Surface& sur,
	  float frames_per_second = 10.0f,
	  Sprite::Direction dir = NONE,
	  LoopType arg_loop_type = ENDLESS);

  Sprite (const ResDescriptor&,
	  float frames_per_second = 10.0f,
	  Sprite::Direction dir = NONE,
	  LoopType arg_loop_type = ENDLESS);

  /** High level version of put_screen (), it handles the frame count
      and the aligment, might be used when you don't have a CL_Vector
      at hand. */
  void put_screen (int x, int y);

  /** High level version put_screen (), it handles the framecount and
      the alignment */
  void put_screen (const CL_Vector& pos);

  /** Set the alignment (aka offset) of the surface 
   @param arg_x The x offset by which the surface drawn
   @param arg_y The y offset by which the surface drawn */
  void set_align (int arg_x, int arg_y);
  
  /** Shortcut for setting the aligment to the center of the surface */
  void set_align_center ();
  
  /** Go to the next frame */
  void next_frame ();

  /** Go to the previous frame */
  void previous_frame ();

  /** Get the currently displaed frame 
      @return the currently displayed frame */
  int get_frame ();

  /** Get the current position in percent @return The current
      animation position in percent relative to max_frames () */
  float get_progress ();

  /** Set the current direction of the surface, WARNING: Does only
      work in a few case... FIXME: rewrite when
      CL_Surface::get_num_xframes () exists 
  @param dir Direction: LEFT, RIGHT, NONE */
  void set_direction (Sprite::Direction dir) { direction = dir; }

  /** Update the sprites status and go one speed further if needed
      @param delta The amount of time which has been pasted since the
      last update in seconds */
  void update (float delta);

  // @return width of the sprite
  int get_width () { return sur.get_width (); }

  // @return height of the sprite
  int get_height () { return sur.get_height (); }

  /// @return true when the animation is played/finished
  bool finished ();

  /// @return the surface which is used internally
  CL_Surface get_surface ();
};

#endif

/* EOF */
