//  $Id: PingusResource.cc,v 1.1 2000/02/04 23:45:18 mbn Exp $
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
#include "globals.hh"
#include "algo.hh"
#include "PingusResource.hh"

vector<PingusResource::Res> PingusResource::resource;

PingusResource::PingusResource()
{
  cout << "PingusResource Constructor called" << endl;
}

void
PingusResource::open(string filename)
{
  string file_n;

  if (verbose)
    cout << "PingusResource: Searching for file: " << filename << endl;

  file_n = find_file(pingus_datadir, "data/" + filename);

  if (verbose)
    cout << "PingusResource: Found file: " << file_n << endl;

  if (filename != file_n) {
    Res res;
    res.res = CL_ResourceManager::create(file_n.c_str(), true);
    res.filename = filename;
    
    resource.push_back(res);
  } else {
    throw PingusError("PingusResource: Couldn't find resource file: " + filename);
  }
}

CL_ResourceManager*
PingusResource::get(string filename)
{
  for(vector<Res>::iterator i = resource.begin(); i != resource.end(); i++) 
    {
      if (filename == i->filename)
	{
	  return i->res;
	}
    }

  open(filename);
  
  return get(filename);
}

/* EOF */
