//  $Id: force_vector.cxx,v 1.8 2002/10/14 11:15:15 torangan Exp $
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

// Force Vectors Implementation - Made by Peter Todd

#include <math.h>
#include "force_vector.hxx"

Vector grav(0.0,1.0);

std::vector<GravityForce>   ForcesHolder::grav_array;
std::vector<ExplosionForce> ForcesHolder::explo_array;


GravityForce& GravityForce::operator= (const GravityForce& old)
{
  if (this == &old)
    return *this;
    
  ifv = old.ifv;
  
  return *this;
}

ExplosionForce& ExplosionForce::operator= (const ExplosionForce& old)
{
  if (this == &old)
    return *this;
  
  iinten = old.iinten;
  isize  = old.isize;
  ip     = old.ip;
  
  return *this;
}

// Apply the explosion force
Vector
ExplosionForce::apply_forces(const Vector& p, const Vector& v)
{
  // Is p within the radius of the explosion?
  if (!(   (p.x - ip.x < isize && p.x - ip.x > -isize)
	&& (p.y - ip.y < isize && p.y - ip.y > -isize)))
    {
      return v;
    }

  Vector tmpv = v;
  float imod,dist;

  dist = fabs(p.x - ip.x);
  imod = iinten * ((isize - dist) / isize);

  // Apply the explosion to p
  if (p.x > ip.x)
    tmpv.x += imod;
  else
    tmpv.x -= imod;

  dist = fabs(p.y - ip.y);
  imod = -(iinten * ((isize - dist) / isize));

  if (p.y > ip.y)
    tmpv.y += (imod / 2);
  else
    tmpv.y -= (imod / 2);

  return tmpv;
}

void
ForcesHolder::add_force(const GravityForce& f)
{
  grav_array.push_back(f);
}

void
ForcesHolder::add_force(const ExplosionForce& f)
{
  explo_array.push_back(f);
}

void 
ForcesHolder::clear_explo_list()
{
  explo_array.clear();
}

void
ForcesHolder::clear_all_forces()
{
  grav_array.clear();
  clear_explo_list();
}

Vector
ForcesHolder::apply_forces(const Vector& p, const Vector& v)
{
  Vector tv = v;

  // Go through all of the forces and apply them all
  for (GForceIter i = grav_array.begin(); i != grav_array.end(); ++i)
    tv = i->apply_forces(p,tv);
  
  // Explosion array...
  for (EForceIter j = explo_array.begin(); j != explo_array.end(); ++j)
    tv = j->apply_forces(p,tv);
  
  return tv;
}

/* EOF */
