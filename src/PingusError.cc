//  $Id: PingusError.cc,v 1.4 2000/12/30 23:54:05 grumbel Exp $
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

#include <iostream>

#include "PingusError.hh"

PingusException::PingusException ()
{
}

PingusException::~PingusException ()
{
}

PingusBug::PingusBug(std::string mes)
{
  message = mes;
  std::cout << "PingusBug: " << message << std::endl;
}

std::string
PingusBug::get_message ()
{
  return "PingusBug: " + message;
}

PingusError::PingusError(std::string mes)
{
  message = mes;
  std::cout << "PingusError: " << message << std::endl;
}

std::string
PingusError::get_message ()
{
  return "PingusError: " + message;
}

/* EOF */
