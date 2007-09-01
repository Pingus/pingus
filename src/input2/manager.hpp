//  $Id$
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

#ifndef HEADER_PINGUS_INPUT_MANAGER_HXX
#define HEADER_PINGUS_INPUT_MANAGER_HXX

#include <vector>
#include "controller.hpp"
#include "controller_description.hpp"
#include "driver.hpp"

namespace Input {

class Manager 
{
private:
  typedef std::vector<Driver*> Drivers;
  Drivers drivers;

  Controller controller;
  ControllerDescription desc;
public:
  Manager();

  void load(const std::string& filename);
  void update(float delta);

  Driver* load_driver(const std::string& name);
  Driver* get_driver(const std::string& name);

  Button*   create_button  (const FileReader& reader, Control* parent);
  Axis*     create_axis    (const FileReader& reader, Control* parent);
  Pointer*  create_pointer (const FileReader& reader, Control* parent);
  Scroller* create_scroller(const FileReader& reader, Control* parent);
};

} // namespace Input

#endif

/* EOF */
