//  $Id: pingus_error.cxx,v 1.8 2003/04/08 19:56:40 torangan Exp $
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

#include "pingus_error.hxx"
#include "my_gettext.hxx"

PingusException::PingusException (const std::string& mes) : message(mes)
{
}

PingusException::~PingusException ()
{
}

PingusException::PingusException (const PingusException& old) : message(old.message)
{
}

PingusException&
PingusException::operator= (const PingusException& old)
{
  if (this != &old)
    message = old.message;
    
  return *this;
}


PingusBug::PingusBug (const std::string& mes) : PingusException("PingusBug: " + mes)
{
}

PingusBug::PingusBug (const PingusBug& old) : PingusException(old)
{
}

PingusBug&
PingusBug::operator= (const PingusBug& old)
{
  if (this == &old)
    return *this;
    
  PingusException::operator=(old);
  
  return *this;
}

void
PingusBug::raise (const std::string& msg)
{
  throw PingusBug(msg);
}

const std::string&
PingusBug::get_message () const
{
  return message;
}

PingusError::PingusError (const std::string& mes) : PingusException("PingusError: " + mes)
{
}

PingusError::PingusError (const PingusError& old) : PingusException(old)
{
}

PingusError&
PingusError::operator= (const PingusError& old)
{
  if (this == &old)
    return *this;
    
  PingusException::operator=(old);
  
  return *this;
}

void
PingusError::raise (const std::string& msg)
{
  throw PingusError(msg);
}

const std::string&
PingusError::get_message () const
{
  return message;
}

/* EOF */
