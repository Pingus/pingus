//  $Id: pingu.hxx,v 1.20 2002/10/02 19:20:19 grumbel Exp $
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

#ifndef HEADER_PINGUS_PINGU_HXX
#define HEADER_PINGUS_PINGU_HXX

#include "direction.hxx"
#include "pingu_enums.hxx"

// Forward declarations
class Vector;
class ActionHolder;
class PinguAction;
class GraphicContext;

/** The class for managing one of the many penguins which are walking
    around in the World. All actions are handled by PinguAction
    objects. */
class Pingu
{
private:
  /** The primary action with is currently in use */
  PinguAction* action;

  /** A secondary action which will turn active after a given amount of time
      The only example is currently the bomber. */
  PinguAction* countdown_action;

  /** the action that gets triggered when the pingu hits a wall */
  PinguAction* wall_action;

  /** the action that gets triggered when the pingu falls */
  PinguAction* fall_action;

  /** The uniq id of the Pingu, this is used to refer to the Pingu in
      a demo file or in a network connection */
  int id;

  int action_time;
  int owner_id;

  /// The stat of the pingu, these can be modified by PinguActions
  PinguStatus status;
  
  float pos_x;
  float pos_y;
  
  Vector* const velocity; 

public:

  //FIXME make me private
  Direction direction;

  bool request_set_action (PinguAction*);
  
  void  set_action (PinguAction*);

  /** Creates a new Pingu at the given coordinates
      @param arg_id The uniq id of the pingu
      @param pos The start position of the pingu
      @param owner The owner id of the pingu (used for multiplayer) */
  Pingu (int arg_id, const Vector& pos, int owner);
  
  /** Destruct the pingu... */
  ~Pingu ();
  
  /** Return the logical pingus position, this is the position which
      is used for collision detection to the ground (the pingus
      feet) */
  Vector get_pos () const;

  /** Returns the visible position of the pingu, the graphical center
      of the pingu. */
  Vector get_center_pos () const;

  /** Returns the x position of the pingu
   * For backward comp. only
   */
  const float& get_x () const { return pos_x; }

  /** Returns the y position of the pingu
      For backward comp. only */
  const float& get_y () const { return pos_y; }

  /** Checks if this action allows to be overwritten with the given new action */
  bool change_allowed (Actions::ActionName new_action);

  /// Check if the pingu is still alive
  bool is_alive (void);

  /// Return the status of the pingu
  PinguStatus get_status (void) const; 

  PinguStatus set_status (PinguStatus);

  PinguAction* get_action ();

  /// Returns the unique id of the pingu
  int  get_id (void); 
    
  /// Set the pingu to the given coordinates
  void set_pos (float x, float y);

  void set_x (float x);
  
  void set_y (float y);

  /// Set the pingu to the given coordinates
  void set_pos (const Vector& arg_pos);

  const Vector& get_velocity () const { return *velocity; }
  
  void set_velocity (const Vector& velocity_);
      
  // Set the pingu in the gives direction
  void set_direction (Direction d);

  /** Request an action to be set to the pingu, if its a persistent
      action, it will be hold back for later execution, same with a
      timed action, normal action will be applied if the current
      action allows that. */
  bool request_set_action (Actions::ActionName action_name);

  /** Set an action without any checking, the action will take
      instantly control. */
  void set_action (Actions::ActionName action_name);
  
  /// set the wall action if we have one
  bool request_wall_action ();
  
  /// set the fall action if we have one
  bool request_fall_action ();

  PinguAction* get_wall_action () { return wall_action; }
  
  PinguAction* get_fall_action () { return fall_action; }

  /** Returns the `color' of the colmap in the walking direction 
      Examples: 
      (0, -1) is the pixel under the pingu
      (1, 0)  is the pixel in front of the pingu
  */
  int  rel_getpixel (int x, int y);

  /** Let the pingu catch another pingu, so that an action can be
      applied (i.e. let a blocker change the direction f another
      pingu) */
  void catch_pingu (Pingu* pingu);

  /** Returns true if the pingu needs to catch another pingu */
  bool need_catch ();
  
  void draw (GraphicContext& gc);
  void apply_force (Vector);
  
  void update();
  
  /** Indicate if the pingu's speed is above the deadly velocity */
  //bool is_tumbling () const;
  
  float get_z_pos () const { return 0; }

  /** @return The owner_id of the owner, only used in multiplayer
      configurations, ought to bed single player */
  int get_owner ();

  bool   is_over (int x, int y);

  bool   is_inside (int x1, int y1, int x2, int y2);

  double dist (int x, int y);

  /** Return true if the pingu can be catched with the mouse and
      another action can be applied, false otherwise (exiter,
      splashed, etc.) */
  bool catchable ();
  
private:
  Pingu (const Pingu&);
  Pingu& operator= (const Pingu&);  
};

#endif /* PINGU_HH */

/* EOF */
