//  $Id: PinguHolder.cc,v 1.2 2000/02/09 21:43:40 grumbel Exp $
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

#include "PinguHolder.hh"

PinguHolder::PinguHolder()
{
  id_count = 0;
  z_pos = 10;
}

PinguHolder::~PinguHolder()
{
  // Deleting all Pingu objects
  for(PinguIter pingu = pingus.begin(); pingu != pingus.end(); ++pingu) 
    {
      delete (*pingu);
    }
}

void
PinguHolder::push_back(Pingu* pingu)
{
  pingu->set_id(id_count++);
  list<Pingu*>::push_back(pingu);
  pingus.push_back(pingu);
}

void
PinguHolder::draw_offset(int x_of, int y_of, float s)
{
  // No optimisation here for s==1.0, because this is done in Pingu
  // itself.
  for(PinguIter pingu = this->begin(); pingu != this->end(); pingu++)
    {
      if ((*pingu)->get_status() != dead
	  && (*pingu)->get_status() != exited) 
	{
	  if (!(*pingu)->get_action())
	    (*pingu)->draw_offset(x_of, y_of, s);
	}
      else 
	{
	  // Removing the dead pingu and setting the iterator back to
	  // the correct possition, no memory hole since pingus will
	  // keep track of the allocated Pingus
	  pingu = this->erase(pingu);
	  pingu--;
	}
    }

  for(PinguIter pingu = this->begin(); pingu != this->end(); pingu++)
    {
      if ((*pingu)->get_action()) 
	{
	  (*pingu)->draw_offset(x_of, y_of, s);
	}
    }
}

/* EOF */
