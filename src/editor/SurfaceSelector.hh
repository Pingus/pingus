//  $Id: SurfaceSelector.hh,v 1.3 2000/04/24 13:15:43 grumbel Exp $
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

#ifndef SURFACESELECTOR_HH
#define SURFACESELECTOR_HH

#include <string>
#include <vector>

struct surface_obj
{
  CL_Surface* sur;
  std::string      name;
};

class SurfaceSelector
{
private:
  CL_Font* font;
  std::vector<surface_obj>* sur_list;
  int y_of;
  int width;
  int height;

  void   draw();
  void scroll();
  std::vector<surface_obj>::iterator get_current_obj();
public:
  SurfaceSelector(std::vector<surface_obj>*);
  ~SurfaceSelector();

  std::string select();
}; 


#endif

/* EOF */
