//  $Id: pingus_error.hxx,v 1.10 2003/12/13 11:21:23 grumbel Exp $
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

#ifndef HEADER_PINGUS_PINGUS_ERROR_HXX
#define HEADER_PINGUS_PINGUS_ERROR_HXX

#include "pingus.hxx"
#include <string>
#include <exception>


/** A PingusError is thrown in situation where an error occured due to
    invalid user input, file not found events or similar stuff. */
class PingusError : public std::exception
{
protected:
  std::string message;
public:
  PingusError(const std::string& mes);
  ~PingusError() throw();

  const std::string& get_message () const;
  const char* what() const throw();

  static void raise (const std::string& msg);
};


#endif

/* EOF */
