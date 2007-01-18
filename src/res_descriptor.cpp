//  $Id: res_descriptor.cxx,v 1.18 2003/10/20 19:28:54 grumbel Exp $
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

#include <assert.h>
#include <iostream>
#include "res_descriptor.hpp"
#include "pingus_error.hpp"


/*
   uri -> file:///home/ingo/.pingus/images/...
   uri -> resource://core/result/ok
   uri -> file://bla.png (relative to ~/.pingus/images/)
   ResDescriptor(const std::string& uri);
*/

ResDescriptor::ResDescriptor()
{
  res_name = "";
  modifier = ResourceModifierNS::ROT0;
}

ResDescriptor::ResDescriptor(const std::string& arg_res_name,
			     ResourceModifierNS::ResourceModifier arg_modifier)
{
  res_name = arg_res_name;
  modifier = arg_modifier;
}

bool
ResDescriptor::operator<(const ResDescriptor& res_desc) const
{
  return (res_name < res_desc.res_name) && (modifier < res_desc.modifier);
}

std::ostream& operator<<(std::ostream& s, const ResDescriptor& desc)
{
  return s << "[" << desc.res_name << ", "
           << ResourceModifierNS::rs_to_string(desc.modifier) << "]";
}

/* EOF */
