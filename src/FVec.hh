//  $Id: FVec.hh,v 1.1 2000/02/04 23:45:18 mbn Exp $
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

// Force Vectors implementation. Made by Peter Todd

#ifndef FVEC_HH
#define FVEC_HH

#include <vector>
#include <ClanLib/core.h>

// Gravity
extern CL_Vector grav;

class GravityForce {
protected:
  // The force vector
  CL_Vector ifv; 
  
public:
  GravityForce(CL_Vector fv){
    ifv = fv;
  }
  
  // Applies the force to a velocity vector, v, a position p and
  // returns the new velicty vector. Just adds fv to v, p is ignored
  // as gravity is universal.
  CL_Vector apply_forces(CL_Vector p,CL_Vector v){
    return v + ifv;
  }
};

// The ExplosionForce class.
class ExplosionForce {
protected:
  // The intensity
  float iinten;
  
  // The size
  float isize;

  // The position
  CL_Vector ip;

public:
  ExplosionForce(float inten, float size,CL_Vector p){
    iinten = inten;
    isize = size;
    ip = p;
  }

  CL_Vector apply_forces(CL_Vector p,CL_Vector v);
};

// The force holder
class ForcesHolder {
public:
  // The gravity force array
  static vector<GravityForce> grav_array;
  typedef vector<GravityForce>::iterator GForceIter;
  
  // The explosion force array
  static vector<ExplosionForce> explo_array;
  typedef vector<ExplosionForce>::iterator EForceIter;

public:
  
  // Add a gravity force
  static void add_force(const GravityForce& f);

  // Add a explosion force
  static void add_force(const ExplosionForce& f);

  // Clear all forces
  static void clear_explo_list();

  // Clear all forces
  static void clear_all_forces();

  // Apply forces
  static CL_Vector apply_forces(CL_Vector p,CL_Vector v);
};

#endif /* FVEC_HH */

/* EOF */

