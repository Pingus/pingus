//  $Id: controller.cxx,v 1.29 2003/10/20 19:28:55 grumbel Exp $
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

#include "SDL.h"
#include "../gui/screen_manager.hxx"

#include "../debug.hxx"
#include "../pingus_error.hxx"

#include "controller.hxx"
#include "axes/dummy_axis.hxx"
////#include "buttons/dummy_button.hxx"
#include "pointers/dummy_pointer.hxx"
#include "scrollers/dummy_scroller.hxx"
#include "pointer_factory.hxx"
////#include "../xml_file_reader.hxx"
#include "scroller_factory.hxx"
////#include "button_factory.hxx"

namespace Input {

using namespace Axes;
////using namespace Buttons;
using namespace Pointers;
using namespace Scrollers;

Controller* Controller::current_controller = 0;

Controller::Controller (const std::string& configfile)
  : standard_pointer(0),
    scroller(0),
    std_pointer_x(0),
    std_pointer_y(0)
{
#if 0
  CL_InputSourceProvider_File provider(".");
  CL_DomDocument doc(provider.open_source(configfile), true);

  XMLFileReader reader(doc.get_document_element());

  if (reader.get_name() != "pingus-controller")
    {
      PingusError::raise("Controller: invalid config file <" + configfile + ">");
    }
  else
    {
      const std::vector<FileReader>& sections = reader.get_sections();
      for(std::vector<FileReader>::const_iterator i = sections.begin();
          i != sections.end(); ++i)
        {
          if (i->get_name() ==  "standard-pointer")
            standard_pointer = PointerFactory::create(i->get_sections()[0]);
          
          else if (i->get_name() == "scroller")
            scroller = ScrollerFactory::create(i->get_sections()[0]);

          else if (i->get_name() == "primary-button")
            buttons[primary] = ButtonFactory::create(i->get_sections()[0]);

          else if (i->get_name() == "secondary-button")
            buttons[secondary] = ButtonFactory::create(i->get_sections()[0]);

          else if (i->get_name() == "pause-button")
            buttons[pause] = ButtonFactory::create(i->get_sections()[0]);

          else if (i->get_name() == "fast-forward-button")
            buttons[fast_forward] = ButtonFactory::create(i->get_sections()[0]);

          else if (i->get_name() == "armageddon-button")
            buttons[armageddon] = ButtonFactory::create(i->get_sections()[0]);

          else if (i->get_name() == "escape-button")
            buttons[escape] = ButtonFactory::create(i->get_sections()[0]);

          else if (i->get_name() == "action-buttons")
            create_action_buttons(*i);

          else if (i->get_name() == "action-up")
            buttons[action_up] = ButtonFactory::create(i->get_sections()[0]);

          else if (i->get_name() == "action-down")
            buttons[action_down] = ButtonFactory::create(i->get_sections()[0]);

          else
            PingusError::raise(std::string("Unkown Element in Controller Config: ") 
                               + i->get_name());
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

  if (!scroller)
    {
      scroller = new DummyScroller;
      pwarn << "Controller: No scroller - inserting dummy" << std::endl;
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
      slots.push_back(it->second->sig_button_down().connect(this, &Controller::on_button_down, it->first));
      slots.push_back(it->second->sig_button_up().connect(this, &Controller::on_button_up, it->first));
    }
	
	// Catch all keyboard input
	CL_InputDevice keyboard = CL_Display::get_current_window()->get_ic()->get_keyboard();
	slots.push_back(keyboard.sig_key_down().connect(this, &Controller::key_pressed));
#endif 
}

Controller::~Controller ()
{
#if 0
  delete scroller;
  delete standard_pointer;

  for (std::map<ButtonName, Button*>::iterator it = buttons.begin(); it != buttons.end(); ++it)
    delete it->second;
#endif
}

void
Controller::create_action_buttons (FileReader reader)
{
#if 0
  int count = 0;
  
  const std::vector<FileReader>& sections = reader.get_sections();
  for(std::vector<FileReader>::const_iterator i = sections.begin();
      i != sections.end(); ++i)
    {
      if (i->get_name() == "action-button")
        buttons[static_cast<ButtonName>(action_1 + count)] 
          = ButtonFactory::create(i->get_sections()[0]);
      else
        PingusError::raise(std::string("Wrong Element in Controller Config (action-buttons): ") 
                           + i->get_name());
      count += 1;
    }
#endif
}

void
Controller::update (float delta)
{
  // Let SDL fetch events
  SDL_Event event;
  while(SDL_PollEvent(&event))
    {
      switch(event.type)
        {
        case SDL_QUIT:
          ScreenManager::instance()->pop_screen();
          break;

        case SDL_MOUSEMOTION:
          events.push_back(makePointerEvent(Input::standard, event.motion.x, event.motion.y));
          break;

        case SDL_MOUSEBUTTONDOWN:
          if (event.button.button == SDL_BUTTON_LEFT)
            events.push_back(makeButtonEvent(Input::primary, Input::pressed));
          else if (event.button.button == SDL_BUTTON_MIDDLE)
            events.push_back(makeButtonEvent(Input::pause, Input::pressed));
          else if (event.button.button == SDL_BUTTON_RIGHT)
            events.push_back(makeButtonEvent(Input::secondary, Input::pressed));
          break;

        case SDL_MOUSEBUTTONUP:
          if (event.button.button == SDL_BUTTON_LEFT)
            events.push_back(makeButtonEvent(Input::primary, Input::released));
          else if (event.button.button == SDL_BUTTON_MIDDLE)
            events.push_back(makeButtonEvent(Input::pause, Input::released));
          else if (event.button.button == SDL_BUTTON_RIGHT)
            events.push_back(makeButtonEvent(Input::secondary, Input::released));
          break;
          
        case SDL_KEYDOWN:
          break;

        case SDL_KEYUP:
          break;          

        default:
          // FIXME: feed other events to the input manager
          break;
        }
    }

#if 0
  scroller        ->update(delta);
  standard_pointer->update(delta);

  for (std::map<ButtonName, Button*>::iterator it = buttons.begin(); it != buttons.end(); ++it)
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
	
	std::string keys_pressed = get_keys_pressed();
	for (unsigned i = 0; i < (unsigned)keys_pressed.length(); i++) 
	{
		events.push_back(makeKeyboardEvent( keys_pressed[i] ));
	}
#endif
}

void
Controller::clear()
{
  events.clear ();
}

const Button*
Controller::get_button (ButtonName name)
{
  if (buttons.count(name))
    return buttons[name];

  return 0;
}

void
Controller::on_button_down(ButtonName name)
{
  events.push_back(makeButtonEvent(name, pressed));
}

void
Controller::on_button_up(ButtonName name)
{
  events.push_back(makeButtonEvent(name, released));
}
#if 0
void
Controller::key_pressed(const CL_InputEvent &event)
{
	keys += event.str;
}
#endif 
std::string
Controller::get_keys_pressed()
{
	std::string ret_keys = keys;
	keys.clear();
	return ret_keys;
}
 

} // namespace Input

/* EOF */
