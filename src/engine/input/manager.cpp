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

#include "engine/input/driver_factory.hpp"
#include "util/log.hpp"
#include "util/pathname.hpp"
#include "util/raise_exception.hpp"
#include "util/string_util.hpp"

namespace Input {

Manager::Manager() :
  drivers(),
  controllers(),
  desc()
{
  desc.add_axis("action-axis",  ACTION_AXIS);

  desc.add_keyboard("standard-keyboard",  STANDARD_KEYBOARD);

  desc.add_pointer("standard-pointer",   STANDARD_POINTER);
  desc.add_scroller("standard-scroller", STANDARD_SCROLLER);

  desc.add_button("primary-button",      PRIMARY_BUTTON);
  desc.add_button("secondary-button",    SECONDARY_BUTTON);
  desc.add_button("fast-forward-button", FAST_FORWARD_BUTTON);
  desc.add_button("armageddon-button",   ARMAGEDDON_BUTTON);
  desc.add_button("pause-button",        PAUSE_BUTTON);
  desc.add_button("single-step-button",  SINGLE_STEP_BUTTON);
  desc.add_button("escape-button",       ESCAPE_BUTTON);

  desc.add_button("action-up-button",    ACTION_UP_BUTTON);
  desc.add_button("action-down-button",  ACTION_DOWN_BUTTON);

  desc.add_button("action-1-button",     ACTION_1_BUTTON);
  desc.add_button("action-2-button",     ACTION_2_BUTTON);
  desc.add_button("action-3-button",     ACTION_3_BUTTON);
  desc.add_button("action-4-button",     ACTION_4_BUTTON);
  desc.add_button("action-5-button",     ACTION_5_BUTTON);
  desc.add_button("action-6-button",     ACTION_6_BUTTON);
  desc.add_button("action-7-button",     ACTION_7_BUTTON);
  desc.add_button("action-8-button",     ACTION_8_BUTTON);
  desc.add_button("action-9-button",     ACTION_9_BUTTON);
  desc.add_button("action-10-button",    ACTION_10_BUTTON);
}

Manager::~Manager()
{
}

static std::string get_driver_part(const std::string& fullname)
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
Manager::create_controller(const Pathname& filename)
{
  ControllerPtr controller(new Controller(desc));

  ReaderObject reader_object = Reader::parse(filename);

  if (reader_object.get_name() != "pingus-controller")
  {
    raise_exception(std::runtime_error,
                    "Controller: invalid config file '" << filename.str() << "'");
  }
  else
  {
    ReaderMapping reader = reader_object.get_mapping();

    ReaderMapping controls_mapping;
    if (!reader.read_mapping("controls", controls_mapping))
    {
      log_warn("%1%: 'controls' section missing", filename);
    }
    else
    {
      for (const auto& key : controls_mapping.get_keys())
      {
        ReaderCollection collection;
        if (!controls_mapping.read_collection(key.c_str(), collection))
        {
          log_error("%1%: mapping must contain object at %2%", filename, key);
        }
        else
        {
          if (StringUtil::has_suffix(key, "pointer"))
          {
            int id = desc.get_definition(key).id;
            ControllerPointer* ctrl_pointer = controller->get_pointer(id);
            for(const auto& object : collection.get_objects())
            {
              auto pointer = create_pointer(object, ctrl_pointer);
              if (pointer)
              {
                ctrl_pointer->add_pointer(std::move(pointer));
              }
              else
              {
                log_error("Manager: pointer: Couldn't create pointer %1%", object.get_name());
              }
            }
          }
          else if (StringUtil::has_suffix(key, "scroller"))
          {
            int id = desc.get_definition(key).id;
            ControllerScroller* ctrl_scroller = controller->get_scroller(id);
            for(const auto& object : collection.get_objects())
            {
              auto scroller = create_scroller(object, ctrl_scroller);
              if (scroller)
              {
                ctrl_scroller->add_scroller(std::move(scroller));
              }
              else
              {
                log_error("Manager: scroller: Couldn't create scroller %1%", object.get_name());
              }
            }
          }
          else if (StringUtil::has_suffix(key, "button"))
          {
            int id = desc.get_definition(key).id;
            ControllerButton* ctrl_button = controller->get_button(id);
            for(const auto& object : collection.get_objects())
            {
              auto button = create_button(object, ctrl_button);
              if (button)
              {
                ctrl_button->add_button(std::move(button));
              }
              else
              {
                log_error("Manager: button: Couldn't create button %1%", object.get_name());
              }
            }
          }
          else if (StringUtil::has_suffix(key, "axis"))
          {
            int id = desc.get_definition(key).id;
            ControllerAxis* ctrl_axis = controller->get_axis(id);
            for(const auto& object : collection.get_objects())
            {
              auto axis = create_axis(object, ctrl_axis);
              if (axis)
              {
                ctrl_axis->add_axis(std::move(axis));
              }
              else
              {
                log_error("Manager: axis: Couldn't create axis %1%", object.get_name());
              }
            }
          }
          else if (StringUtil::has_suffix(key, "keyboard"))
          {
            int id = desc.get_definition(key).id;
            ControllerKeyboard* ctrl_keyboard = controller->get_keyboard(id);
            for(const auto& object : collection.get_objects())
            {
              std::unique_ptr<Keyboard> keyboard = create_keyboard(object, ctrl_keyboard);
              if (keyboard)
              {
                ctrl_keyboard->add_keyboard(std::move(keyboard));
              }
              else
              {
                log_error("Manager: keyboard: Couldn't create keyboard %1%", object.get_name());
              }
            }
          }
          else
          {
            raise_exception(std::runtime_error, "Manager: Unkown Element in Controller Config: "
                            << key);
          }
        }
      }
    }
  }

  controllers.push_back(controller);
  return controller;
}

void
Manager::refresh()
{
  for(auto i = controllers.begin(); i != controllers.end(); ++i)
    (*i)->refresh();
}

void
Manager::update(float delta)
{
  for(auto i = drivers.begin(); i != drivers.end(); ++i)
    (*i)->update(delta);

  for(auto i = controllers.begin(); i != controllers.end(); ++i)
    (*i)->update(delta);
}

Driver*
Manager::get_driver(const std::string& name)
{
  for(auto i = drivers.begin(); i != drivers.end(); ++i)
  {
    if ((*i)->get_name() == name)
    {
      return i->get();
    }
  }
  return 0;
}

Driver*
Manager::load_driver(const std::string& name)
{
  Driver* driver_p = get_driver(name);
  if (driver_p)
  {
    return driver_p;
  }
  else
  {
    log_info("loading driver '%1%'", name);

    auto driver = DriverFactory::create(name, this);
    if (!driver)
    {
      log_error("unknown driver: %1%", name);
      return 0;
    }
    else
    {
      drivers.push_back(std::move(driver));
      return drivers.back().get();
    }
  }
}

std::unique_ptr<Button>
Manager::create_button(const ReaderObject& reader, Control* parent)
{
  std::string driver = get_driver_part(reader.get_name());

  Driver* drv = load_driver(driver);
  if (drv)
  {
    return drv->create_button(reader, parent);
  }
  else
  {
    log_error("couldn't find driver: '%1%'", driver);
    return {};
  }
}

std::unique_ptr<Axis>
Manager::create_axis(const ReaderObject& reader, Control* parent)
{
  std::string driver = get_driver_part(reader.get_name());

  Driver* drv = load_driver(driver);
  if (drv)
  {
    return drv->create_axis(reader, parent);
  }
  else
  {
    log_error("couldn't find driver: '%1%'", driver);
    return {};
  }
}

std::unique_ptr<Pointer>
Manager::create_pointer(const ReaderObject& reader, Control* parent)
{
  std::string driver = get_driver_part(reader.get_name());

  Driver* drv = load_driver(driver);
  if (drv)
  {
    return drv->create_pointer(reader, parent);
  }
  else
  {
    log_error("couldn't find driver: '%1%'", driver);
    return {};
  }
}

std::unique_ptr<Scroller>
Manager::create_scroller(const ReaderObject& reader, Control* parent)
{
  std::string driver = get_driver_part(reader.get_name());

  Driver* drv = load_driver(driver);
  if (drv)
  {
    return drv->create_scroller(reader, parent);
  }
  else
  {
    log_error("couldn't find driver: '%1%'", driver);
    return {};
  }
}

std::unique_ptr<Keyboard>
Manager::create_keyboard(const ReaderObject& reader, Control* parent)
{
  std::string driver = get_driver_part(reader.get_name());

  Driver* drv = load_driver(driver);
  if (drv)
  {
    return drv->create_keyboard(reader, parent);
  }
  else
  {
    log_error("couldn't find driver: '%1%'", driver);
    return {};
  }
}

} // namespace Input

/* EOF */
