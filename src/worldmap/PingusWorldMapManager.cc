//  $Id: PingusWorldMapManager.cc,v 1.1 2000/09/19 10:33:23 grumbel Exp $
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

#include "PingusWorldMapManager.hh"

PingusWorldMapManager::PingusWorldMapManager ()
{
  is_init = false;
}

PingusWorldMapManager::~PingusWorldMapManager ()
{
}

void 
PingusWorldMapManager::init ()
{
  
}

void
PingusWorldMapManager::display ()
{
  while (true)
    {
      sur->put_screen();
    }
}

void 
PingusWorldMapManager::on_button_press ()
{
  
}

void 
PingusWorldMapManager::on_button_release ()
{
}

/* EOF */
