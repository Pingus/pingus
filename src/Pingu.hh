//  $Id: Pingu.hh,v 1.12 2000/06/18 18:06:56 grumbel Exp $
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

#ifndef PINGU_HH
#define PINGU_HH

#include <ClanLib/core.h>

#include "ColMap.hh"
#include "PinguMap.hh"
#include "particles/ParticleHolder.hh"
#include "Direction.hh"
#include "PinguAction.hh"
#include "ActionHolder.hh"
#include "PinguEnums.hh"

///
class ActionHolder;
class PinguAction; 

class Pingu
{
protected:
  friend class PinguAction;

  ///
  PinguAction* action;
  ///
  PinguAction* sec_action;
  ///
  std::vector<PinguAction* > persist;
  ///
  int id;

  ///
  static bool init;
  ///
  static CL_Surface* walker; 
  ///
  static CL_Surface* faller;
  ///
  static CL_Surface* tumble;
  ///
  static CL_Font* font;
  ///
  static CL_ResourceManager* local_res_p;

  ///
  AnimCounter walker_c;
  ///
  AnimCounter faller_c;
  ///
  AnimCounter tumble_c;
  ///
  int action_time;

  ///
  PinguStatus status;
  ///
  PinguEnvironment environment;

public:
  ///
  int x_pos;
  ///
  int y_pos;
  ///
  int falling;
  ///
  bool tumbling;

  ///
  Pingu();
  ///
  Pingu(int x, int y);
  ///
  ~Pingu();
  
  /// Should be friends
  static ParticleHolder* particle;
  ///
  static void SetParticleHolder(ParticleHolder* );
  ///
  static ColMap* colmap;
  ///
  static PinguMap*  map;
  ///
  static ActionHolder* action_holder;

  ///
  Direction direction;

  // Force Vector stuff:

  /// Current velocity
  CL_Vector velocity; 

  /// Returns the x position of the pingu
  int  get_x(void);

  /// Returns the y position of the pingu
  int  get_y(void);

  ///
  int  x_offset(void);
  ///
  int  y_offset(void);
  ///
  PinguEnvironment get_environment(); 

  /// Check if the pingu is still alive
  bool is_alive(void);

  /// Return the status of the pingu
  PinguStatus get_status(void) const; 

  ///
  PinguStatus set_status(PinguStatus);

  /// Returns the unique id of the pingu
  int  get_id(void); 
  
  /// Set's the unique id of the pingu
  int  set_id(int);
  
  /// Set the pingu to the given coordinates
  void set_pos(int x, int y);
 
  // Set the pingu in the gives direction
  void set_direction(Direction d);

  ///
  int  set_action(PinguAction*);
  ///
  int  set_paction(PinguAction*);
  ///
  PinguAction* get_action();
  ///
  int  rel_getpixel(int x, int y);
  ///
  void catch_pingu(Pingu* pingu);
  ///
  bool need_catch();
  
  ///
  void draw_offset(int x, int y, float s = 1.0) const;
  ///
  void apply_force(CL_Vector);

  ///
  void let_move();
  ///
  void do_persistent();
  ///
  void do_normal();
  ///
  void do_falling();
  ///
  void do_walking();

  ///
  bool   is_over(int x, int y);
  ///
  double dist(int x, int y);

  ///
  static CL_ResourceManager* local_res();
  
  ///
  static void set_colmap(ColMap*);
  ///
  static void set_map(PinguMap*);
  ///
  static void set_action_holder(ActionHolder*);
};

#endif /* PINGU_HH */

/* EOF */
