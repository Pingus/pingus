//  $Id: Pingu.hh,v 1.5 2000/02/28 17:54:21 grumbel Exp $
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
#include "PinguEnums.hh"

class PinguAction; 

class Pingu
{
protected:
  friend class PinguAction;

  PinguAction* action;
  PinguAction* sec_action;
  vector<PinguAction* > persist;
  int id;

  static bool init;
  static CL_Surface* walker; 
  static CL_Surface* faller;
  static CL_Surface* tumble;
  static CL_Font* font;
  static CL_ResourceManager* local_res_p;

  AnimCounter walker_c;
  AnimCounter faller_c;
  AnimCounter tumble_c;
  int action_time;

  PinguStatus status;
  PinguEnvironment environment;

public:
  int x_pos;
  int y_pos;
  int falling;
  bool tumbling;

  Pingu();
  Pingu(int x, int y);
  ~Pingu();
  
  // Should be friends
  static ParticleHolder* particle;
  static void SetParticleHolder(ParticleHolder* );
  static ColMap* colmap;
  static PinguMap*  map;

  Direction direction;

  // Force Vector stuff:
  CL_Vector velocity; // Current velocity and last velocity

  int  get_x(void);
  int  get_y(void);
  int  x_offset(void);
  int  y_offset(void);
  PinguEnvironment get_environment(); 
  bool is_alive(void); // Check if the pingu is still alive

  PinguStatus get_status(void) const; // Return the status of the pingu
  PinguStatus set_status(PinguStatus);
  int  get_id(void);   // Returns the unique id of the pingu
  int  set_id(int); // Set's the unique id of the pingu
  void set_pos(int x, int y); // Set the pingu to the given coordinates
  void set_direction(Direction d); // Set the pingu in the gives direction
  int  set_action(PinguAction*, int t = -2);
  int  set_paction(PinguAction*);
  PinguAction* get_action();
  int  rel_getpixel(int x, int y);
  void catch_pingu(Pingu* pingu);
  bool need_catch();
  
  void draw_offset(int x, int y, float s = 1.0) const;

  void let_move();
  void do_persistent();
  void do_normal();
  void do_falling();
  void do_walking();
  PinguEnvironment check_enviroment();

  bool   is_over(int x, int y);
  double dist(int x, int y);

  static CL_ResourceManager* local_res();
  
  static void set_colmap(ColMap*);
  static void set_map(PinguMap*);
};

#endif /* PINGU_HH */

/* EOF */
