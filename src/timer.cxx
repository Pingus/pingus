//  $Id: timer.cxx,v 1.6 2003/10/20 13:33:43 grumbel Exp $
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

#include <ClanLib/Core/System/system.h>
#include "debug.hxx"
#include "globals.hxx"
#include "timer.hxx"


Timer::Timer(const char* desc)
  : description(desc),
    start_time (CL_System::get_time())
{
}

void
Timer::stop()
{
  unsigned int time = CL_System::get_time() - start_time;
  pout(PINGUS_DEBUG_LOADING)
    << "Timer: '" << description << "' took "
    << time << "msec" << std::endl;
}


/* EOF */
