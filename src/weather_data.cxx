//  $Id: weather_data.cxx,v 1.4 2002/09/14 19:06:33 torangan Exp $
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

#include <iostream>
#include "weather_data.hxx"

WeatherData
WeatherData::operator= (const WeatherData& old)
{
  if (this == &old)
    return *this;
    
  type = old.type;
  
  return *this;
}

EditorObjLst
WeatherData::create_EditorObj ()
{
  EditorObjLst lst;
  std::cout << "WeatherData::create_EditorObj ():  FIXME: Not implemented" << std::endl;
  return lst;
}

/* EOF */
