//  $Id: WeatherObj.cc,v 1.1 2000/08/05 18:52:22 grumbel Exp $
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

#include "../PingusResource.hh"
#include "WeatherObj.hh"

WeatherObj::WeatherObj(const WeatherObj& obj)
{
  surf = obj.surf;
  type = obj.type;
  init();
  pos = Position(0,0,200);
}

WeatherObj::WeatherObj(WeatherData data)
{
  type = data.type;
  pos = Position(0,0,200);

  if (type == "rain")
    {
      surf = PingusResource::load_surface("Objects/weather_rain", "editor");
    }
  else if (type == "snow")
    {
      surf = PingusResource::load_surface("Objects/weather_snow", "editor");
    }  
  else 
    {
      std::cout << "WeatherObj: Unknown weather type: " << type << std::endl;
      surf = PingusResource::load_surface("Objects/weather_rain", "editor");
    }
  init();
}

WeatherObj::~WeatherObj()
{
}
  
void   
WeatherObj::save_xml(std::ofstream* xml)
{
  (*xml) << "  <weather>\n"
	 << "    <type>" << type << "</type>\n";
  save_position_xml(xml, pos);
  (*xml) << "  </weather>\n"
	 << std::endl;
}
 
EditorObj* 
WeatherObj::duplicate()
{
  return new WeatherObj(*this);
}

/* EOF */
