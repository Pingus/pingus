//  $Id$
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

#ifndef HEADER_PINGUS_PINGUS_HXX
#define HEADER_PINGUS_PINGUS_HXX

/**
 * Multiple hacks, workarounds and helper things which are included
 * everywhere and are used to fix broken compilers or supress
 * warrnings.
 *
 * @file pingus.hxx
 **/

#if defined(WIN32) && !defined(__GNUC__)
#  define for if(0);else for

#  define snprintf _snprintf

#  pragma warning(disable:4786) //over 255 characters in debug indentifier, due to STL instantiation
   //sadly this does not actually work on 6.0, but it does in .NET

#  pragma warning(disable:4355) //this used in constructor base

#  pragma warning(disable:4800) //'int' : forcing value to bool 'true' or 'false' (performance warning)

#  include <assert.h>
#endif

#endif

/* EOF */
