//  $Id: PinguHolder.cc,v 1.12 2000/12/31 00:48:34 grumbel Exp $
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

#include "boost/smart_ptr.hpp"
#include "PinguAction.hh"
#include "PinguHolder.hh"

using namespace boost;

PinguHolder::PinguHolder()
{
  id_count = 0;
  total_size_count = 0;
  saved_pingus = 0;
}

PinguHolder::~PinguHolder()
{
  // Deleting all Pingu objects
  std::cout << "PinguHolder: Deleting pingus" << std::endl;
  /*  for(PinguIter pingu = pingus.begin(); pingu != pingus.end(); ++pingu) 
      delete (*pingu);*/
}

int
PinguHolder::total_size()
{
  return total_size_count;
}

void
PinguHolder::push_back(boost::shared_ptr<Pingu> pingu)
{
  total_size_count++;
  pingu->set_id(id_count++);
#ifndef WIN32
  std::list<shared_ptr<Pingu> >::push_back(pingu);
#else
  push_front(pingu);
#endif
  pingus.push_back(pingu);
}

void
PinguHolder::draw_offset(int x_of, int y_of, float s)
{
  for(PinguIter pingu = this->begin(); pingu != this->end(); pingu++)
    {
      if ((*pingu)->get_status() == dead)
	{
	  // Removing the dead pingu and setting the iterator back to
	  // the correct possition, no memory hole since pingus will
	  // keep track of the allocated Pingus
	  pingu = this->erase(pingu);
	  pingu--;
	}
      else if ((*pingu)->get_status() == exited) 
	{
	  saved_pingus++;
	  pingu = this->erase(pingu);
	  pingu--;	  
	}
      else 
	{
	  // We don't draw the actions here, since we want them above
	  // all other pingus, for better visibility
	  if (!((*pingu)->get_action().get()))
	    (*pingu)->draw_offset(x_of, y_of, s);
	}
    }

  // We draw all actions here, so we have them above all others
  for(PinguIter pingu = this->begin(); pingu != this->end(); pingu++)
    {
      if ((*pingu)->get_action().get()) 
	(*pingu)->draw_offset(x_of, y_of, s);
    }
}

/* EOF */
