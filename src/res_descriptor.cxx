//  $Id: res_descriptor.cxx,v 1.11 2002/09/14 19:06:33 torangan Exp $
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

ResDescriptor ResDescriptor::operator= (const ResDescriptor& old)
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

ResDescriptor::ResDescriptor(const std::string& str) 
{
  modifier = ResourceModifierNS::ROT0;

  std::string::size_type pos1;
  std::string::size_type pos2;

  //cout << "Resdes: " << str << std::endl;

  type = RD_RESOURCE;
  
  pos1 = str.find_first_of(':');
  pos2 = str.find_first_of(')');
  
  if (pos1 != std::string::npos && pos2 != std::string::npos)
    {
      datafile = str.substr(pos1 + 1, (pos2 - pos1 - 1));
      res_name = str.substr(pos2 + 1);
    }
  else
    {
      res_name = str;
      datafile = "global";
    }
}

ResDescriptor::ResDescriptor(const std::string& c_cast, const std::string& value) 
{
  modifier = ResourceModifierNS::ROT0;
  std::string cast;

  if (c_cast.find_first_of(":") == std::string::npos) {
    cast = c_cast;
    datafile = "global";
  } else {
    cast     = c_cast.substr(0, c_cast.find_first_of(":"));
    datafile = c_cast.substr(c_cast.find_first_of(":") + 1);
  }
  
  if (cast == "file") {
    type = RD_FILE;
  } else if (cast == "resource") {
    type = RD_RESOURCE;
  } else if (cast == "auto") {
    type = RD_AUTO;
  } else {
    PingusError::raise("ResDescriptor: Wrong cast '"+cast+"' for value '"+value+"'\n");
  }
  res_name = value;
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

std::ostream& operator<<(std::ostream& s, const ResDescriptor& desc)
{
  switch (desc.type)
    {
    case ResDescriptor::RD_RESOURCE:
      return s << "(datafile: " << desc.datafile << ", res_name: " << desc.res_name << ")";
      break;
    case ResDescriptor::RD_FILE:
      return s << "(file:" << desc.res_name << ")";
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
