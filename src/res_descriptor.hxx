//  $Id: res_descriptor.hxx,v 1.8 2002/09/11 12:45:57 grumbel Exp $
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

#ifndef HEADER_PINGUS_RES_DESCRIPTOR_HXX
#define HEADER_PINGUS_RES_DESCRIPTOR_HXX

#include "resource_modifier.hxx"

/// Resource descriptor, tells were to find a resource.
class ResDescriptor
{
public:
  enum ResourceType { RD_RESOURCE, RD_FILE, RD_AUTO };
  
  /// Where to get the data from?
  ResourceType type; 
  
  /// The name of the resource (.dat) file
  std::string datafile;
  
  /// The name of the data, filename or resourcename ("Textures/desert")
  std::string res_name;
  
  ResourceModifierNS::ResourceModifier modifier;

  ResDescriptor ();
  ResDescriptor (const ResDescriptor& old);
  ResDescriptor operator= (const ResDescriptor&);

  ResDescriptor (const std::string& res_name, const std::string& datafile,
		 ResourceType type,
		 ResourceModifierNS::ResourceModifier modifier 
		 = ResourceModifierNS::ROT0);
  ResDescriptor (const std::string& cast, const std::string& value);
  ResDescriptor (const std::string& str);

  bool operator< (const ResDescriptor&) const;
  
};

/// Our own function to print out the ResDescriptor
std::ostream& operator<<(std::ostream& s, const ResDescriptor& desc);

#endif

/* EOF */
