//  $Id: pingu_action.hxx,v 1.6 2002/06/28 15:12:22 torangan Exp $
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

#ifndef HEADER_PINGUS_PINGU_ACTION_HXX
#define HEADER_PINGUS_PINGU_ACTION_HXX

#include "worldobj.hxx"
#include "pingu_enums.hxx"
#include "pingu.hxx"

using namespace Pingus::Actions;

class ActionHolder;

enum ActionType
{
  INSTANT             = 1<<1,
  WALL_TRIGGERED      = 1<<2,
  FALL_TRIGGERED      = 1<<3,
  COUNTDOWN_TRIGGERED = 1<<4
};

// This class provides an abstract interface for pingu actions. It is 
// used to inherit classes which represent the actions. The actions
// are stored in a seperate library, have a look in actions/ for some
/// examples.
class PinguAction : public WorldObj
{
protected:
  /** A pointer to the pingu, which hold the action. */
  Pingu*      pingu;

  /// Height of Pingu.  Used in head collision.
  static const int pingu_height;

public:

  PinguAction();
  virtual ~PinguAction();

  /// Gives the PinguAction class access to the data of the Pingu.
  void set_pingu(Pingu*);

  /** Get the pixel from the colmap, relative to the pingu position.

      @param x x is relative to the direction, so 1 is the pixel
      infront of the pingu

      @param y 1 is up, -1 is down
   */
  int  rel_getpixel(int x, int y);

  /** Checks if this action allows to be overwritten with the given new action */
  virtual bool change_allowed (ActionName) { return true; }
  
  /** Used to load all data, which is needed by the action, its
      seperated and called in set_pingu(), because some data will be
      only valid if set_pingu() is called. */
  virtual void  init(void) {};

  /// The "AI" of the pingu.
  virtual void  update(float delta) = 0;

  /** Draws the surfaced defined by the action, can be overwritten if
      the action needs a more complicated way of drawing. */
  virtual void  draw_offset(int x, int y, float s) =0;

  /// Returns the activation mode
  virtual ActionType get_activation_mode(void) const;

  /// The name of the action
  virtual std::string get_name(void) const =0;
  
  /// The type of the action
  virtual ActionName get_type() const =0;
  
  /** Return the character that is shown when a persitent action is
      activated in the CaptureRectangle. */
  virtual char get_persistent_char ();

  /// Catch another pingu and act on it (see blocker.hxx)
  virtual bool  need_catch();

  ///
  virtual void  catch_pingu(Pingu*) { /* do nothing */}

  /// The time the action needs to get activated (see bomber.cxx)
  virtual int   activation_time() { return -1; };

  /** Return true if the pingu can be catched with the mouse and
      another action can be applied, false otherwise (exiter,
      splashed, etc.) */
  virtual bool catchable () { return true; }

  // FIXME: z_pos is currently unused for pingu actions
  virtual float get_z_pos () const { return 0; }

  /// True if Pingu in specified position would bang its head if it were walking
  bool head_collision_on_walk (int x, int y);
};

#endif /* PINGU_ACTION_HH */

/* EOF */
