//  $Id: PingusError.cc,v 1.5 2001/12/05 09:15:51 grumbel Exp $
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

/* Headers needed for i18n / gettext */
#include <clocale>
#include <config.h>
#include "my_gettext.hh"


PingusException::PingusException ()
{
}

PingusException::~PingusException ()
{
}

PingusBug::PingusBug(std::string mes)
{
  message = mes;
  std::cout << _("PingusBug: ") << message << std::endl;
}

std::string
PingusBug::get_message ()
{
  return _("PingusBug: ") + message;
}

PingusError::PingusError(std::string mes)
{
  message = mes;
  std::cout << _("PingusError: ") << message << std::endl;
}

std::string
PingusError::get_message ()
{
  return _("PingusError: ") + message;
}

/* EOF */
