//  $Id: PinguAction.hh,v 1.11 2000/12/16 23:11:20 grumbel Exp $
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

#ifndef PINGU_ACTION_HH
#define PINGU_ACTION_HH

#include <ClanLib/core.h>
#include "boost/smart_ptr.hpp"

using namespace boost;

#include "AnimCounter.hh"
#include "Pingu.hh"
#include "PinguEnums.hh"

///
class Pingu;
class ActionHolder;

enum ActionType
{
  ///
  ONCE = 1<<1,
  WALL = 1<<2,
  FALL = 1<<3
};

// This class provides an abstract interface for pingu actions. It is 
// used to inherit classes which represent the actions. The actions
// are stored in a seperate library, have a look in actions/ for some
/// examples.
class PinguAction
{
protected:
  // True if the animation differ in both direction. draw_offset() then
  /// handle the case apropried.
  bool is_multi_direct;

  /** A pointer to the pingu, which hold the action. */
  Pingu*      pingu;

  /** The basic surface used to display an action, the inherit action
      needs to load it in PinguAction::init(). */
  CL_Surface surface;
  
  /// The font used for counting (used by the bomber)
  CL_Font*    font; 

  /** Sets the speed and number of frames, PinguAction::init() needs to
      set this to the correct values. */
  AnimCounter counter;

  /// The enviroment of the action (see PinguEnvironment).
  PinguEnvironment environment;

  /// The name of the action, needs to be set in PinguAction::init().
  std::string action_name;

  /// See is_active()
  bool        active;  

public:
  /** Indicate if the action should be canceled at the next possible
      point. Bug: Only keeped public for lazyness. */
  bool is_finished;

  ///
  PinguAction();
  ///
  virtual ~PinguAction();

  /// Gives the PinguAction class access to the data of the Pingu.
  void set_pingu(Pingu*);

  /// Get the pixel from the colmap, relative to the pingu position. 
  int  rel_getpixel(int x, int y);

  /// True if the action was used the last let_move()
  bool is_active(void);
  
  /** Returns the enviroment, used to check if an action can be
      applied. */
  PinguEnvironment get_environment();
  
  /** Used to load all data, which is needed by the action, its
      seperated and called in set_pingu(), because some data will be
      only valid if set_pingu() is called. */
  virtual void  init(void) {};

  /// The "AI" of the pingu. The walker and faller is in class Pingu
  virtual void  let_move(void) = 0;

  /** Draws the surfaced defined by the action, can be overwritten if
      the action needs a more complicated way of drawing. */
  virtual void  draw_offset(int x, int y, float s);

  ///
  virtual int    x_offset(void);
  ///
  virtual int    y_offset(void);

  /// Returns the action type
  virtual ActionType get_type(void);

  /// The name of the action, used in PinguInfo.hh
  const std::string& name(void);
  
  /// Catch another pingu and act on it (see blocker.hh)
  virtual bool  need_catch();

  ///
  virtual void  catch_pingu(Pingu*) { /* do nothing */}

  /// The time the action needs to get activated (see bomber.cc)
  virtual int   activation_time() { return -1; };
};

#endif /* PINGU_ACTION_HH */

/* EOF */
