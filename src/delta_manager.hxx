//  $Id: delta_manager.hxx,v 1.4 2002/08/23 15:49:48 torangan Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_DELTA_MANAGER_HXX
#define HEADER_PINGUS_DELTA_MANAGER_HXX

#include "pingus.hxx"
#include <ClanLib/Core/System/system.h>

class DeltaManager
{
private:
  unsigned int last_time;
public:
  DeltaManager ()
    : last_time (CL_System::get_time ())
  {}

  float getset ()
  {
    float ret = get ();
    set ();
    return ret;
  }
  
  void set () 
  {
    last_time = CL_System::get_time ();
  }
  
  float get () 
  {
    return (CL_System::get_time () - last_time) / 1000.0f;
  }
  
private:
  DeltaManager (const DeltaManager&);
  DeltaManager operator= (const DeltaManager&);
};

#endif

/* EOF */



