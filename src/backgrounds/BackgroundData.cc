//  $Id: BackgroundData.cc,v 1.4 2001/05/18 19:17:08 grumbel Exp $
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

#include "SurfaceBackgroundData.hh"
#include "StarfieldBackground.hh"
#include "SolidColorBackground.hh"
#include "ThunderstormBackgroundData.hh"
#include "BackgroundData.hh"

BackgroundData::BackgroundData()
{
}

BackgroundData::~BackgroundData()
{
}

boost::shared_ptr<BackgroundData>
BackgroundData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  boost::shared_ptr<BackgroundData> data;
  char* type = (char*)xmlGetProp(cur, (xmlChar*)"type");

  if (type != 0)
    {
      if (strcmp(type, "surface") == 0)
	{
	  data = SurfaceBackgroundData::create(doc, cur);
	}
      else if (strcmp(type, "thunderstorm") == 0)
	{
	  data = ThunderstormBackgroundData::create (doc, cur);
	}
      else if (strcmp(type, "starfield") == 0)
	{
	  data = StarfieldBackgroundData::create (doc, cur);
	}
      else if (strcmp (type, "solidcolor") == 0)
	{
	  data = SolidColorBackgroundData::create (doc, cur);
	}
      else
	{
	  std::cout << "BackgroundData: Unhandled background type, default to surface-background, probally evil!" << std::endl;
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
