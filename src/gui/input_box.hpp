
//  Pingus - A free Lemmings clone
//  Copyright (C) 2006 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_GUI_INPUTBOX_HPP
#define HEADER_PINGUS_GUI_INPUTBOX_HPP

#include "component.hpp"
#include "../math/vector2i.hpp"
#include <string>

class DrawingContext;

namespace GUI {

/** This class allows keyboard input to be displayed in a box */
class InputBox : public GUI::Component {
protected:
  std::string str;
  Vector2i pos;
  int width;
  int height;
  bool  is_locked;

  /** Shrink the string to fit in the box */
  std::string shrink_string(const std::string& s) const;
  
  /** Optional label to display */
  std::string label;  

public:
  InputBox(int width_, Vector2i p, const std::string& 
           default_value = std::string(), bool locked = false,
           const std::string& label_ = std::string());

  void set_string(const std::string& s) { str = s; }
  std::string get_string() { return str; }
	
  void on_key_pressed(const unsigned short c);
	
  /// GUI Component functions:
  ///
  void update(float delta) { }
	
  bool is_at(int x, int y);
	
  void draw(DrawingContext &gc);
};	// InputBox class

} // namespace GUI

#endif

/* EOF */
