//  $Id: AnimCounter.cc,v 1.5 2002/06/11 09:55:12 torangan Exp $
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

#include "AnimCounter.hh"
#include <ClanLib/Core/System/system.h>

AnimCounter::AnimCounter() : last_time(0), speed(100)
{
}

AnimCounter::AnimCounter(int s) : last_time(0), speed(s)
{
}

bool 
AnimCounter::enough_time_passed(void) const
{
  if (speed < CL_System::get_time() - last_time) 
    return true;
  else
    return false;  
}

int
AnimCounter::operator=(int i)
{
  return Counter::operator=(i);
}

int
AnimCounter::operator++()
{
  if (enough_time_passed()) {
    last_time = CL_System::get_time();
    Counter::operator++();
  }
  return int(*this);
}

void
AnimCounter::set_speed(int s)
{
  speed = s;
}

int
AnimCounter::get_speed() const
{
  return speed;
}

/* EOF */



