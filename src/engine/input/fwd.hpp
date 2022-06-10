//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2021 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_ENGINE_INPUT_FWD_HPP
#define HEADER_PINGUS_ENGINE_INPUT_FWD_HPP

#include <memory>

namespace pingus::input {

class Axis;
class Button;
class Control;
class Controller;
class ControllerPointer;
class CoreDriver;
class Driver;
class DriverFactory;
class Keyboard;
class Manager;
class Pointer;
class SDLDriver;
class Scroller;
struct AxisEvent;
struct ButtonEvent;
struct Event;
struct ScrollEvent;

typedef std::shared_ptr<Controller> ControllerPtr;

} // namespace pingus::input

#endif

/* EOF */
