//  $Id: ResDescriptor.hh,v 1.5 2000/06/18 22:19:48 grumbel Exp $
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

#ifndef RESDESCRIPTOR_HH
#define RESDESCRIPTOR_HH

#include <string>

/// Resource descriptor, tells were to find a resource.
class ResDescriptor
{
public:
  ///
  enum ResourceType { RESOURCE, FILE, AUTO };
  
  /// Where to get the data from?
  ResourceType type; 
  
  /// The name of the resource file
  std::string filename;
  
  /// The name of the data, filename or resourcename.
  std::string res_name;

  ///
  ResDescriptor();
  ///
  ResDescriptor(std::string cast, std::string value);
  ///
  ResDescriptor(std::string str);
};

#endif

/* EOF */
