//  $Id: res_descriptor.cxx,v 1.17 2003/10/18 23:17:27 grumbel Exp $
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
#include "res_descriptor.hxx"
#include "pingus_error.hxx"

namespace Pingus {

/*
   uri -> file:///home/ingo/.pingus/images/...
   uri -> resource://core/result/ok
   uri -> file://bla.png (relative to ~/.pingus/images/)
   ResDescriptor(const std::string& uri);
*/

ResDescriptor::ResDescriptor()
{
  type = RD_RESOURCE;
  res_name = "";
  datafile = "global";
  modifier = ResourceModifierNS::ROT0;
}

ResDescriptor::ResDescriptor (const ResDescriptor& res_desc)
                            : type(res_desc.type),
			      datafile(res_desc.datafile),
			      res_name(res_desc.res_name),
			      modifier(res_desc.modifier)
{
}

ResDescriptor&
ResDescriptor::operator= (const ResDescriptor& old)
{
  if (this == &old)
    return *this;

  type     = old.type;
  datafile = old.datafile;
  res_name = old.res_name;
  modifier = old.modifier;

  return *this;
}

ResDescriptor::ResDescriptor(const std::string& arg_res_name,
			     const std::string& arg_datafile,
			     ResourceType arg_type,
			     ResourceModifierNS::ResourceModifier arg_modifier)
{
  res_name = arg_res_name;
  datafile = arg_datafile;
  type     = arg_type;
  modifier = arg_modifier;
}

bool
ResDescriptor::operator<(const ResDescriptor& res_desc) const
{
  // FIXME: This is ugly and slow
  //return (datafile + res_name + to_string (type) + to_string (modifier))
  //  < (res_desc.datafile + res_desc.res_name + to_string (res_desc.type) + to_string (res_desc.modifier));

  if (datafile < res_desc.datafile)
    return true;
  else if (datafile > res_desc.datafile)
    return false;
  else
    {
      if (res_name < res_desc.res_name)
	return true;
      else if (res_name > res_desc.res_name)
	return false;
      else
	{
	  if (modifier < res_desc.modifier)
	    return true;
	  else if (modifier > res_desc.modifier)
	    return false;
	  else
	    {
	      if (type < res_desc.type)
		return true;
	      else if (type > res_desc.type)
		return false;
	      else
		return false;
	    }
	}
    }
}

} // namespace Pingus

std::ostream& operator<<(std::ostream& s, const Pingus::ResDescriptor& desc)
{
  switch (desc.type)
    {
    case ResDescriptor::RD_RESOURCE:
      return s << "[" << desc.res_name << ", " << desc.datafile
               << ", " << ResourceModifierNS::rs_to_string(desc.modifier) << "]";
      break;
    case ResDescriptor::RD_FILE:
      return s << "(plainfile:" << desc.res_name << ")";
      break;
    case ResDescriptor::RD_AUTO:
      return s << "(auto)";
      break;
    default:
      std::cout << "ResDescriptor: Unknown type: " << desc.type << std::endl;
      assert (!"Unknown type");
      return s;
    }
}

/* EOF */
