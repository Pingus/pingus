//  $Id: sprite.hxx,v 1.15 2003/10/22 12:35:47 grumbel Exp $
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

#ifndef HEADER_PINGUS_SPRITE_HXX
#define HEADER_PINGUS_SPRITE_HXX

#include "pingus.hxx"
#include <ClanLib/display.h>

namespace Pingus {

class Vector;
class ResDescriptor;
class GraphicContext;

class Sprite
{
public:
  typedef enum { NONE, LEFT, RIGHT } Direction;
  typedef enum { ENDLESS, ONCE } LoopType;

private:
  CL_Sprite sprite;
  float frame;
  float frames_per_second;
  Direction direction;
  LoopType looptype;
  bool is_finished;

  int max_frames ();

public:
  /// Creates an empty sprite, drawing and updating does nothing
  Sprite ();

  Sprite (std::string arg_sur_name,
	  std::string arg_datafile,
	  float arg_frames_per_second = 10.0f,
	  Direction dir = NONE,
	  LoopType arg_loop_type = ENDLESS);

  /** High level version of draw (), it handles the frame count
      and the aligment, might be used when you don't have a Vector
      at hand. */
  void draw (int x, int y);

  /** High level version draw (), it handles the framecount and
      the alignment */
  void draw (const Vector& pos);

  /** Equal to draw() except that it draws to a GraphicContext */
  void draw(GraphicContext& gc, const Vector& pos );

  /** Set the alignment (aka offset) of the surface
   @param arg_x The x offset by which the surface drawn
   @param arg_y The y offset by which the surface drawn */
  void set_align (int arg_x, int arg_y);

  /** Shortcut for setting the aligment to the center of the surface */
  void set_align_center ();

  /** Shortcut for setting the aligment to the center of x-axis and to
      the bottom of the y-axis. */
  void set_align_center_bottom ();

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
      work in a few cases... FIXME: rewrite when
      CL_Sprite::get_num_frames () exists
  @param dir Direction: LEFT, RIGHT, NONE */
  void set_direction (Sprite::Direction dir) { direction = dir; }

  /** Update the sprites status and go one speed further if needed
      @param delta The amount of time which has been pasted since the
      last update in seconds */
  void update (float delta);

  /** Update the sprite with the global delta value, note this is not
      a goto_next_frame, but instead a update(game_speed), so it might
      actually skip frames */
  void update ();

  // @return width of the sprite
  int get_width (); 

  // @return height of the sprite
  int get_height ();

  /// @return true when the animation is played/finished
  bool finished ();

  /** Once an animation is finished it can be reseted to its initial
      position */
  void reset ();

  /** Set the sprite to the n'th frame */
  void set_frame (int n);

  /// @return the surface which is used internally
  CL_Sprite& get_sprite();
};

} // namespace Pingus

#endif

/* EOF */

