//  $Id: WeatherGenerator.hh,v 1.9 2002/06/10 13:03:35 torangan Exp $
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

#ifndef WEATHERGENERATOR_HH
#define WEATHERGENERATOR_HH

#include "../WorldObj.hh"

class WeatherData;

namespace boost {
  template <class T> class shared_ptr;
}

class WeatherGenerator : public WorldObj
{
private:
  
public:
  ///
  WeatherGenerator() {}
  ///
  virtual ~WeatherGenerator() {}
  ///
  virtual void update(float delta) =0;
  ///
  float get_z_pos() const { return 1000; }
  ///
  void draw_offset(int, int, float) {}


  /** Generate an WeatherGenerator which fits to the given data */
  static boost::shared_ptr<WorldObj> create(WeatherData);
};

#endif

/* EOF */
