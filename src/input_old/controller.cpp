//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include "SDL.h"
#include "../screen/screen_manager.hpp"

#include "../debug.hpp"
#include "../pingus_error.hpp"

#include "path_manager.hpp"
#include "controller.hpp"
#include "axes/dummy_axis.hpp"
#include "buttons/dummy_button.hpp"
#include "pointers/dummy_pointer.hpp"
#include "scrollers/dummy_scroller.hpp"
#include "pointer_factory.hpp"
#include "scroller_factory.hpp"
#include "button_factory.hpp"
#include "buttons/key_button.hpp"
#include "buttons/mouse_button.hpp"
#include "scrollers/axis_scroller.hpp"
#include "../global_event.hpp"

namespace Input {

using namespace Axes;
using namespace Buttons;
using namespace Pointers;
using namespace Scrollers;

Controller* Controller::current_controller = 0;
std::vector<Controller::key_callback_info> Controller::key_callbacks;
std::vector<Controller::mouse_callback_info> Controller::mouse_callbacks;

Controller::Controller(const std::string& configfile)
  : standard_pointer(0),
    scroller(0),
    std_pointer_x(0),
    std_pointer_y(0)
{
  FileReader reader = FileReader::parse(path_manager.complete(configfile));
  if (reader.get_name() != "pingus-controller")
    {
      PingusError::raise("Controller: invalid config file <" + configfile + ">");
    }
  else
    {
      const std::vector<FileReader>& sections = reader.get_sections();
      for (std::vector<FileReader>::const_iterator i = sections.begin();
          i != sections.end(); ++i)
        {
          if (i->get_name() == "standard-pointer")
            {
              standard_pointer = PointerFactory::create(i->get_sections()[0]);
            }
          else if (i->get_name() == "primary-button")
            {
              buttons[primary] = ButtonFactory::create(i->get_sections()[0]);
            }
          else if (i->get_name() == "secondary-button")
            {
              buttons[secondary] = ButtonFactory::create(i->get_sections()[0]);
            }
          else if (i->get_name() == "pause-button")
            {
              buttons[pause] = ButtonFactory::create(i->get_sections()[0]);
            }
          else if (i->get_name() == "fast-forward-button")
            {
              buttons[fast_forward] = ButtonFactory::create(i->get_sections()[0]);
            }
          else if (i->get_name() == "armageddon-button")
            {
              buttons[armageddon] = ButtonFactory::create(i->get_sections()[0]);
            }
          else if (i->get_name() == "escape-button")
            {
              buttons[escape] = ButtonFactory::create(i->get_sections()[0]);
            }
          else if (i->get_name() == "scroller")
            {
              scroller = ScrollerFactory::create(i->get_sections()[0]);
            }
          else if (i->get_name() == "action-buttons")
            {
              create_action_buttons(*i);
            }
          else if (i->get_name() == "action-up")
            {
              buttons[action_up] = ButtonFactory::create(i->get_sections()[0]);
            }
          else if (i->get_name() == "action-down")
            {
              buttons[action_down] = ButtonFactory::create(i->get_sections()[0]);
            }
          else
            {
              PingusError::raise(std::string("Unkown Element in Controller Config: ") 
                                 + i->get_name());
            }
        }
    }

  if (!standard_pointer)
    {
      standard_pointer = new DummyPointer;
      pwarn << "Controller: No standard pointer - inserting dummy" << std::endl;
    }
  else
    {
      std_pointer_x = standard_pointer->get_x_pos();
      std_pointer_y = standard_pointer->get_y_pos();
    }

  if (!buttons.count(primary))
    {
      buttons[primary] = new DummyButton;
      pwarn << "Controller: No primary button - inserting dummy" << std::endl;
    }

  if (!buttons.count(secondary))
    {
      buttons[secondary] = new DummyButton;
      pwarn << "Controller: No secondary button - inserting dummy" << std::endl;
    }

  if (!buttons.count(pause))
    {
      buttons[pause] = new DummyButton;
      pwarn << "Controller: No pause button - inserting dummy" << std::endl;
    }

  if (!buttons.count(fast_forward))
    {
      buttons[fast_forward] = new DummyButton;
      pwarn << "Controller: No fast_forward button - inserting dummy" << std::endl;
    }

  if (!buttons.count(armageddon))
    {
      buttons[armageddon] = new DummyButton;
      pwarn << "Controller: No armageddon button - inserting dummy" << std::endl;
    }

  if (!buttons.count(escape))
    {
      buttons[escape] = new DummyButton;
      pwarn << "Controller: No escape button - inserting dummy" << std::endl;
    }

  if (!scroller)
    {
      scroller = new DummyScroller;
      pwarn << "Controller: No scroller - inserting dummy" << std::endl;
    }

  if (!buttons.count(action_up))
    {
      buttons[action_up] = new DummyButton;
      pwarn << "Controller: No action up button - inserting dummy" << std::endl;
    }

  if (!buttons.count(action_down))
    {
      buttons[action_down] = new DummyButton;
      pwarn << "Controller: No action down button - inserting dummy" << std::endl;
    }

  for (std::map<ButtonName, Button*>::iterator it = buttons.begin(); it != buttons.end(); ++it)
    {
      static int button_names[max_button];
      button_names[it->first] = it->first;
      it->second->add_down_callback(&Controller::on_button_down, &button_names[it->first]);
      it->second->add_up_callback(&Controller::on_button_up, &button_names[it->first]);
    }

}

Controller::~Controller()
{
  delete scroller;
  delete standard_pointer;

  std::map<ButtonName, Button*>::iterator it;
  for (it = buttons.begin(); it != buttons.end(); ++it)
    delete it->second;
}

void
Controller::create_action_buttons(FileReader reader)
{
  int count = 0;
  
  const std::vector<FileReader>& sections = reader.get_sections();
  for(std::vector<FileReader>::const_iterator i = sections.begin();
      i != sections.end(); ++i)
    {
      if (i->get_name() == "action-button")
      {
        buttons[static_cast<ButtonName>(action_1 + count)] 
          = ButtonFactory::create(i->get_sections()[0]);
      }
      else
      {
        PingusError::raise(std::string("Wrong Element in Controller Config (action-buttons): ") 
                           + i->get_name());
      }
      count++;
    }
}

void
Controller::update(float delta)
{
  // Let SDL fetch events
  SDL_Event event;
  while (SDL_PollEvent(&event))
    {
      switch(event.type)
        {
        case SDL_QUIT:
          ScreenManager::instance()->pop_all_screens();
          break;

        case SDL_MOUSEMOTION:
	  {
            std::vector<mouse_callback_info>::iterator i;
            for (i = mouse_callbacks.begin(); i != mouse_callbacks.end(); ++i)
              {
                i->callback(event, i->userdata);
              }
            break;
	  }

        case SDL_MOUSEBUTTONDOWN:
	  {
            std::vector<mouse_callback_info>::iterator i;
            for (i = mouse_callbacks.begin(); i != mouse_callbacks.end(); ++i)
              {
                i->callback(event, i->userdata);
              }
            break;
	  }

        case SDL_MOUSEBUTTONUP:
	  {
            std::vector<mouse_callback_info>::iterator i;
            for (i = mouse_callbacks.begin(); i != mouse_callbacks.end(); ++i)
              {
                i->callback(event, i->userdata);
              }
            break;
	  }
          
        case SDL_KEYDOWN:
          {
            global_event.on_button_press(event.key);
 
            std::vector<key_callback_info>::iterator i;
            for (i = key_callbacks.begin(); i != key_callbacks.end(); ++i)
              {
                i->callback(event, i->userdata);
              }

            events.push_back(makeKeyboardEvent(event.key.keysym.unicode));
            break;
          }

        case SDL_KEYUP:
          {
            global_event.on_button_release(event.key);
      
            std::vector<key_callback_info>::iterator i;
            for (i = key_callbacks.begin(); i != key_callbacks.end(); ++i)
              {
                i->callback(event, i->userdata);
              }

            events.push_back(makeKeyboardEvent(event.key.keysym.unicode));
            break;
          }

        default:
          // FIXME: feed other events to the input manager
          break;
        }
    }

  scroller->update(delta);
  standard_pointer->update(delta);

  for (std::map<ButtonName, Button*>::iterator it = buttons.begin();
      it != buttons.end(); ++it)
    it->second->update(delta);

  // FIXME: Busy checking of button status and other events is *VERY EVIL*
  if (std_pointer_x != standard_pointer->get_x_pos()
      || std_pointer_y != standard_pointer->get_y_pos())
    {
      std_pointer_x = standard_pointer->get_x_pos();
      std_pointer_y = standard_pointer->get_y_pos();

      events.push_back(makePointerEvent(standard, std_pointer_x, std_pointer_y));
    }

  // FIXME: Busy checking of button status and other events is *VERY EVIL*
  if (scroller->get_x_delta() || scroller->get_y_delta())
    events.push_back(makeScrollEvent(scroller->get_x_delta(), scroller->get_y_delta()));
}

void
Controller::clear()
{
  events.clear();
}

void
Controller::add_key_callback(key_callback_func callback, void *userdata)
{
  key_callback_info cb;
  cb.callback = callback;
  cb.userdata = userdata;
  key_callbacks.push_back(cb);
}

void
Controller::add_mouse_callback(mouse_callback_func callback, void *userdata)
{
  mouse_callback_info cb;
  cb.callback = callback;
  cb.userdata = userdata;
  mouse_callbacks.push_back(cb);
}

const Button*
Controller::get_button(ButtonName name)
{
  if (buttons.count(name))
    return buttons[name];

  return 0;
}

void
Controller::on_button_down(void* userdata)
{
  ButtonName name = *(ButtonName*)userdata;
  Controller::get_current()->events.push_back(makeButtonEvent(name, pressed));
}

void
Controller::on_button_up(void* userdata)
{
  ButtonName name = *(ButtonName*)userdata;
  Controller::get_current()->events.push_back(makeButtonEvent(name, released));
}


} // namespace Input

/* EOF */
