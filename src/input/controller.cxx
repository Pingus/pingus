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

#include "../debug.hxx"
#include "../xml_helper.hxx"
#include "../pingus_error.hxx"

/*#include "axis_event.hxx"
  #include "axis_factory.hxx"
  #include "button_factory.hxx"
  #include "pointer_event.hxx"
*/
#include "controller.hxx"
#include "axes/dummy_axis.hxx"
#include "buttons/dummy_button.hxx"
#include "pointers/dummy_pointer.hxx"
#include "scrollers/dummy_scroller.hxx"
#include "pointer_factory.hxx"
#include "scroller_factory.hxx"
#include "button_factory.hxx"
//#include "scroll_event.hxx"

namespace Pingus {
namespace Input {

using namespace Axes;
using namespace Buttons;
using namespace Pointers;
using namespace Scrollers;

Controller* Controller::current_controller = 0;

Controller::Controller (const std::string& configfile)
  : standard_pointer(0),
    scroller(0),
    std_pointer_x(0),
    std_pointer_y(0)
{
  xmlDocPtr doc = xmlParseFile(configfile.c_str());

  if (!doc)
    PingusError::raise("Controller: config file <" + configfile + "> not found");

  xmlNodePtr cur = doc->ROOT;

  if (!cur || !XMLhelper::equal_str(cur->name, "pingus-controller"))
    PingusError::raise("Controller: invalid config file <" + configfile + ">");
  cur = XMLhelper::skip_blank(cur->children);

  if (!cur || !XMLhelper::equal_str(cur->name, "controller-config"))
    PingusError::raise("Controller: invalid config file <" + configfile + ">");
  cur = XMLhelper::skip_blank(cur->children);

  while (cur)
    {
      if (xmlIsBlankNode(cur)) // explicit check cause we need the continue to check for cur again
        {
          cur = cur->next;
          continue;
        }

      else if (XMLhelper::equal_str(cur->name, "standard-pointer"))
        standard_pointer = PointerFactory::create(XMLhelper::skip_blank(cur->children));

      else if (XMLhelper::equal_str(cur->name, "scroller"))
        scroller = ScrollerFactory::create(XMLhelper::skip_blank(cur->children));

      else if (XMLhelper::equal_str(cur->name, "primary-button"))
        buttons[primary] = ButtonFactory::create(XMLhelper::skip_blank(cur->children));

      else if (XMLhelper::equal_str(cur->name, "secondary-button"))
        buttons[secondary] = ButtonFactory::create(XMLhelper::skip_blank(cur->children));

      else if (XMLhelper::equal_str(cur->name, "pause-button"))
        buttons[pause] = ButtonFactory::create(XMLhelper::skip_blank(cur->children));

      else if (XMLhelper::equal_str(cur->name, "fast-forward-button"))
        buttons[fast_forward] = ButtonFactory::create(XMLhelper::skip_blank(cur->children));

      else if (XMLhelper::equal_str(cur->name, "armageddon-button"))
        buttons[armageddon] = ButtonFactory::create(XMLhelper::skip_blank(cur->children));

      else if (XMLhelper::equal_str(cur->name, "escape-button"))
        buttons[escape] = ButtonFactory::create(XMLhelper::skip_blank(cur->children));

      else if (XMLhelper::equal_str(cur->name, "action-buttons"))
        create_action_buttons(XMLhelper::skip_blank(cur->children));

      else if (XMLhelper::equal_str(cur->name, "action-up"))
        buttons[action_up] = ButtonFactory::create(XMLhelper::skip_blank(cur->children));

      else if (XMLhelper::equal_str(cur->name, "action-down"))
        buttons[action_down] = ButtonFactory::create(XMLhelper::skip_blank(cur->children));

      else
        PingusError::raise(std::string("Unkown Element in Controller Config: ") + ((cur->name) ? reinterpret_cast<const char*>(cur->name) : ""));

      cur = cur->next;
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
}

Controller::~Controller ()
{
  delete scroller;
  delete standard_pointer;

  for (std::map<ButtonName, Button*>::iterator it = buttons.begin(); it != buttons.end(); ++it)
    delete it->second;
}

void
Controller::create_action_buttons (xmlNodePtr cur)
{
  int count = 0;

  while (cur)
    {
      if (xmlIsBlankNode(cur))
        {
          cur = cur->next;
          continue;
        }

      if (XMLhelper::equal_str(cur->name, "action-button"))
        buttons[static_cast<ButtonName>(action_1 + count)] = ButtonFactory::create(XMLhelper::skip_blank(cur->children));
      else
        PingusError::raise(std::string("Wrong Element in Controller Config (action-buttons): ") + reinterpret_cast<const char*>(cur->name));

      cur = cur->next;
      count++;
    }
}

void
Controller::update (float delta)
{
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

} // namespace Input
} // namespace Pingus

/* EOF */
