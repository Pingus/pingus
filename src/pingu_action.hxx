//  $Id: pingu_action.hxx,v 1.21 2002/10/22 00:21:02 grumbel Exp $
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

#include "pingu_enums.hxx"

class ActionHolder;
class GraphicContext;
class Pingu;

enum ActionType
{
  INSTANT,
  WALL_TRIGGERED,
  FALL_TRIGGERED,
  COUNTDOWN_TRIGGERED
};

/** This class provides an abstract interface for pingu actions. It is 
    used to inherit classes which represent the actions. The actions
    are stored in a seperate library, have a look in actions/ for some
    examples. */
class PinguAction
{
protected:
  /** A pointer to the pingu, which hold the action. */
  Pingu*      pingu;

  /// Height of Pingu.  Used in head collision.
  static const int pingu_height;

public:
  PinguAction(Pingu* p);
  virtual ~PinguAction();

  /// Gives the PinguAction class access to the data of the Pingu.
  void set_pingu (Pingu*);

  /** Get the pixel from the colmap, relative to the pingu position.

      @param x x is relative to the direction, so 1 is the pixel
      infront of the pingu

      @param y 1 is up, -1 is down
   */
  int  rel_getpixel (int x, int y);

  /** Checks if this action allows to be overwritten with the given new action */
  virtual bool change_allowed (Actions::ActionName) { return true; }
  
  /// The "AI" of the pingu.
  virtual void update () = 0;

  /** Draws the action */
  virtual void draw (GraphicContext& gc) =0;

  /// Returns the activation mode
  virtual ActionType get_activation_mode (void) const;

  /** The name of the action, this is used in the CaputreRectangle, so
      it can contain more than just the name */
  virtual std::string get_name () const =0;
  
  /// The type of the action
  virtual Actions::ActionName get_type () const =0;
  
  /** Return the character that is shown when a persitent action is
      activated in the CaptureRectangle. */
  virtual char get_persistent_char ();

  /// Catch another pingu and act on it (see blocker.hxx)
  virtual bool  need_catch();

  ///
  virtual void  catch_pingu (Pingu*) { /* do nothing */}

  /// The time the action needs to get activated (see bomber.cxx)
  virtual int   activation_time () { return -1; };

  /** Return true if the pingu can be catched with the mouse and
      another action can be applied, false otherwise (exiter,
      splashed, etc.) */
  virtual bool catchable () { return true; }

  /// True if Pingu in specified position would bang its head if it were walking
  bool head_collision_on_walk (int x, int y);

  void move_with_forces (float x_to_add, float y_to_add);

  
  /** Called if the action was successfully applied via request_set_action */
  virtual void on_successfull_apply () { }

  /** Called if the request_set_action failded to apply this action */
  virtual void on_failed_apply (Pingu*) { }
  
  /** FIXME: document me */
  void move_with_forces (float x_to_add, float y_to_add);

private:
  PinguAction (const PinguAction&);
  PinguAction& operator= (const PinguAction&);
};

#endif /* PINGU_ACTION_HH */

/* EOF */
