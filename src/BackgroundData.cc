//  $Id: BackgroundData.cc,v 1.1 2000/08/28 00:34:39 grumbel Exp $
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

#include "backgrounds/SurfaceBackgroundData.hh"
#include "backgrounds/ThunderstormBackgroundData.hh"
#include "BackgroundData.hh"

BackgroundData::BackgroundData()
{
}

BackgroundData::~BackgroundData()
{
}

BackgroundData* 
BackgroundData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  BackgroundData* data;
  char* type = (char*)xmlGetProp(cur, (xmlChar*)"type");

  if (type != 0)
    {
      if (strcmp(type, "surface-background") == 0)
	{
	  data = SurfaceBackgroundData::create(doc, cur);
	}
      else if (strcmp(type, "thunderstorm") == 0)
	{
	  data = ThunderstormBackgroundData::create (doc, cur);
	}
      else
	{
	  std::cout << "BackgroundData: Unhandeld background type, default to surface-background" << std::endl;
	  data = SurfaceBackgroundData::create(doc, cur);
	}
    }
  else
    {
      std::cout << "BackgroundData: No background type given, default to surface-background" << std::endl;
      data = SurfaceBackgroundData::create(doc, cur);
    }

  free (type);
  return data;
}

/* EOF */
