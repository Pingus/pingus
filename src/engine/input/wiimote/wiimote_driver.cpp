//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmail.com>
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

#include "engine/input/wiimote/wiimote_driver.hpp"

#include "engine/input/wiimote/wiimote.hpp"
#include "pingus/globals.hpp"
#include "util/string_util.hpp"

namespace Input {

WiimoteDriver::WiimoteDriver() :
  button_bindings(),
  axis_bindings()
{
  Wiimote::init();
  wiimote->connect();
}

WiimoteDriver::~WiimoteDriver()
{
  Wiimote::deinit();
}

void
WiimoteDriver::update(float delta)
{
  if (!wiimote || !wiimote->is_connected())
    return;

  std::vector<WiimoteEvent> events = wiimote->pop_events();
  for(std::vector<WiimoteEvent>::iterator i = events.begin(); i != events.end(); ++i)
  {
    WiimoteEvent& event = *i;
    if (event.type == WiimoteEvent::WIIMOTE_BUTTON_EVENT)
    {
      log_debug("WiimoteDriver: (wiimote:button (button %1)) => %2", event.button.button, event.button.down);

      for (std::vector<ButtonBinding>::const_iterator j = button_bindings.begin();
           j != button_bindings.end();
           ++j)
      {
        if (event.button.button == j->button)
        {
          j->binding->set_state(event.button.down ? BUTTON_PRESSED : BUTTON_RELEASED);
        }
      }
    }
    else if (event.type == WiimoteEvent::WIIMOTE_AXIS_EVENT)
    {
      log_debug("WiimoteDriver: (wiimote:axis (axis %1)) => %2", event.axis.axis, event.axis.pos); // Fixme: should output string

      for (std::vector<AxisBinding>::const_iterator j = axis_bindings.begin();
           j != axis_bindings.end(); ++j)
      {
        if (event.axis.axis == j->axis)
        {
          j->binding->set_state(event.axis.pos);
        }
      }
    }
#if 0
    else if (event.type == WiimoteEvent::WIIMOTE_ACC_EVENT)
    {
      if (event.acc.accelerometer == 0)
      {
        if (0)
          printf("%d - %6.3f %6.3f %6.3f\n",
                 event.acc.accelerometer,
                 event.acc.x,
                 event.acc.y,
                 event.acc.z);

        float roll = atan(event.acc.x/event.acc.z);
        if (event.acc.z <= 0.0) {
          roll += M_PI * ((event.acc.x > 0.0) ? 1 : -1);
        }
        roll *= -1;

        float pitch = atan(event.acc.y/event.acc.z*cos(roll));

        add_axis_event(X2_AXIS, math::mid(-1.0f, -float(pitch / M_PI), 1.0f));
        add_axis_event(Y2_AXIS, math::mid(-1.0f, -float(roll  / M_PI), 1.0f));

        std::cout << boost::format("%|6.3f| %|6.3f|") % pitch % roll << std::endl;
      }
    }
    else
    {
      assert(!"Never reached");
    }
#endif
  }
}

Button*
WiimoteDriver::create_button(const FileReader& reader, Control* parent)
{
  std::string button;
  if (reader.get_name() == "wiimote:button")
  {
    if (!reader.read_string("button", button))
    {
      std::cout << "WiimoteDriver: 'button' tag missing" << std::endl;
      return 0;
    }
    else
    {
      int button_id = Wiimote::str2id(StringUtil::to_lower(button));

      if (button_id == Wiimote::UNKNOWN)
      {
        std::cout << "Error: WiimoteDriver: unknown button: " << button << std::endl;
        return 0;
      }

      ButtonBinding binding;
      binding.binding = new Button(parent);
      binding.button = button_id;
      button_bindings.push_back(binding);

      return binding.binding;
    }
  }
  else
  {
    return 0;
  }
}

Axis*
WiimoteDriver::create_axis(const FileReader& reader, Control* parent)
{
  std::string axis;
  if (reader.get_name() == "wiimote:axis")
  {
    if (!reader.read_string("axis", axis))
    {
      std::cout << "WiimoteDriver: 'axis' tag missing" << std::endl;
      return 0;
    }
    else
    {
      axis = StringUtil::to_lower(axis);
      int axis_id = 0;
      if (axis == "nunchuk:x")
        axis_id = 0;
      else if (axis == "nunchuk:y")
        axis_id = 1;
      else
      {
        std::cout << "WiimoteDriver: unknown axis name: " << axis << std::endl;
        return 0;
      }

      AxisBinding binding;
      binding.binding = new Axis(parent);
      binding.axis = axis_id;
      axis_bindings.push_back(binding);

      return binding.binding;
    }
  }
  else
  {
    return 0;
  }
}

Scroller*
WiimoteDriver::create_scroller(const FileReader& reader, Control* parent)
{
  return 0;
}

Pointer*
WiimoteDriver::create_pointer (const FileReader& reader, Control* parent)
{
  return 0;
}

} // namespace Input

/* EOF */
