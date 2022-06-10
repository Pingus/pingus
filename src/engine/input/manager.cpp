// Pingus - A free Lemmings clone
// Copyright (C) 2007 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "engine/input/manager.hpp"

#include <stdexcept>

#include <fmt/format.h>
#include <logmich/log.hpp>
#include <prio/reader_collection.hpp>
#include <prio/reader_document.hpp>
#include <prio/reader_mapping.hpp>

#include "engine/input/control.hpp"
#include "engine/input/controller.hpp"
#include "engine/input/driver.hpp"
#include "engine/input/driver_factory.hpp"

namespace pingus::input {

Manager::Manager(DriverFactory& driver_factory, ControllerDescription desc) :
  m_driver_factory(driver_factory),
  m_controllers(),
  m_desc(std::move(desc))
{
}

Manager::~Manager()
{
}

static std::string get_driver_part(std::string const& fullname)
{
  std::string::size_type i = fullname.find_first_of(':');
  if (i != std::string::npos)
  {
    return fullname.substr(0, i);
  }
  else
  {
    return "core";
  }
}

ControllerPtr
Manager::create_controller(std::filesystem::path const& filename)
{
  ControllerPtr controller = std::make_unique<Controller>(m_desc);

  auto doc = prio::ReaderDocument::from_file(filename);
  if (doc.get_name() != "pingus-controller") {
    throw std::runtime_error(fmt::format("Controller: invalid config file '{}'", filename));
  }

  prio::ReaderMapping reader = doc.get_mapping();

  prio::ReaderMapping controls_mapping;
  if (!reader.read("controls", controls_mapping))
  {
    log_warn("{}: 'controls' section missing", filename);
  }
  else
  {
    for (auto const& key : controls_mapping.get_keys())
    {
      prio::ReaderCollection collection;
      if (!controls_mapping.read(key.c_str(), collection))
      {
        log_error("{}: mapping must contain object at {}", filename, key);
      }
      else
      {
        if (key.ends_with("pointer"))
        {
          int id = m_desc.get_definition(key).id;
          ControllerPointer* ctrl_pointer = controller->get_pointer(id);
          for(auto const& object : collection.get_objects())
          {
            auto pointer = create_pointer(object, ctrl_pointer);
            if (pointer)
            {
              ctrl_pointer->add_pointer(std::move(pointer));
            }
            else
            {
              log_error("Manager: pointer: Couldn't create pointer {}", object.get_name());
            }
          }
        }
        else if (key.ends_with("scroller"))
        {
          int id = m_desc.get_definition(key).id;
          ControllerScroller* ctrl_scroller = controller->get_scroller(id);
          for(auto const& object : collection.get_objects())
          {
            auto scroller = create_scroller(object, ctrl_scroller);
            if (scroller)
            {
              ctrl_scroller->add_scroller(std::move(scroller));
            }
            else
            {
              log_error("Manager: scroller: Couldn't create scroller {}", object.get_name());
            }
          }
        }
        else if (key.ends_with("button"))
        {
          int id = m_desc.get_definition(key).id;
          ControllerButton* ctrl_button = controller->get_button(id);
          for(auto const& object : collection.get_objects())
          {
            auto button = create_button(object, ctrl_button);
            if (button)
            {
              ctrl_button->add_button(std::move(button));
            }
            else
            {
              log_error("Manager: button: Couldn't create button {}", object.get_name());
            }
          }
        }
        else if (key.ends_with("axis"))
        {
          int id = m_desc.get_definition(key).id;
          ControllerAxis* ctrl_axis = controller->get_axis(id);
          for(auto const& object : collection.get_objects())
          {
            auto axis = create_axis(object, ctrl_axis);
            if (axis)
            {
              ctrl_axis->add_axis(std::move(axis));
            }
            else
            {
              log_error("Manager: axis: Couldn't create axis {}", object.get_name());
            }
          }
        }
        else if (key.ends_with("keyboard"))
        {
          int id = m_desc.get_definition(key).id;
          ControllerKeyboard* ctrl_keyboard = controller->get_keyboard(id);
          for(auto const& object : collection.get_objects())
          {
            std::unique_ptr<Keyboard> keyboard = create_keyboard(object, ctrl_keyboard);
            if (keyboard)
            {
              ctrl_keyboard->add_keyboard(std::move(keyboard));
            }
            else
            {
              log_error("Manager: keyboard: Couldn't create keyboard {}", object.get_name());
            }
          }
        }
        else
        {
          throw std::runtime_error(fmt::format("Manager: Unkown Element in Controller Config: {}", key));
        }
      }
    }
  }

  m_controllers.push_back(controller);
  return controller;
}

void
Manager::refresh()
{
  for(auto i = m_controllers.begin(); i != m_controllers.end(); ++i)
    (*i)->refresh();
}

void
Manager::update(float delta)
{
  m_driver_factory.update(delta);

  for(auto i = m_controllers.begin(); i != m_controllers.end(); ++i)
    (*i)->update(delta);
}

std::unique_ptr<Button>
Manager::create_button(prio::ReaderObject const& reader, Control* parent)
{
  std::string driver = get_driver_part(reader.get_name());

  Driver* drv = m_driver_factory.get(driver, this);
  if (drv)
  {
    return drv->create_button(reader, parent);
  }
  else
  {
    log_error("couldn't find driver: '{}'", driver);
    return {};
  }
}

std::unique_ptr<Axis>
Manager::create_axis(prio::ReaderObject const& reader, Control* parent)
{
  std::string driver = get_driver_part(reader.get_name());

  Driver* drv = m_driver_factory.get(driver, this);
  if (drv)
  {
    return drv->create_axis(reader, parent);
  }
  else
  {
    log_error("couldn't find driver: '{}'", driver);
    return {};
  }
}

std::unique_ptr<Pointer>
Manager::create_pointer(prio::ReaderObject const& reader, Control* parent)
{
  std::string driver = get_driver_part(reader.get_name());

  Driver* drv = m_driver_factory.get(driver, this);
  if (drv)
  {
    return drv->create_pointer(reader, parent);
  }
  else
  {
    log_error("couldn't find driver: '{}'", driver);
    return {};
  }
}

std::unique_ptr<Scroller>
Manager::create_scroller(prio::ReaderObject const& reader, Control* parent)
{
  std::string driver = get_driver_part(reader.get_name());

  Driver* drv = m_driver_factory.get(driver, this);
  if (drv)
  {
    return drv->create_scroller(reader, parent);
  }
  else
  {
    log_error("couldn't find driver: '{}'", driver);
    return {};
  }
}

std::unique_ptr<Keyboard>
Manager::create_keyboard(prio::ReaderObject const& reader, Control* parent)
{
  std::string driver = get_driver_part(reader.get_name());

  Driver* drv = m_driver_factory.get(driver, this);
  if (drv)
  {
    return drv->create_keyboard(reader, parent);
  }
  else
  {
    log_error("couldn't find driver: '{}'", driver);
    return {};
  }
}

} // namespace pingus::input

/* EOF */
