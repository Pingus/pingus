//  $Id: WeatherObj.hh,v 1.5 2001/08/12 18:36:41 grumbel Exp $
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

#ifndef WEATHEROBJ_HH
#define WEATHEROBJ_HH

#include "../WeatherData.hh"
#include "SpriteEditorObj.hh"

class WeatherObj 
  : public SpriteEditorObj,
    public WeatherData
{
private:
  CL_Vector pos;
  bool dragging;
public:
   WeatherObj(const WeatherData& data);
  virtual ~WeatherObj();

  virtual void drag ();
  virtual void drop ();

  virtual void draw (boost::dummy_ptr<EditorView> view);
  virtual void write_xml(std::ofstream* xml);
  virtual boost::shared_ptr<EditorObj> duplicate();
};

#endif

/* EOF */
