//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#include "engine/input/control.hpp"
#include "engine/input/controller_description.hpp"

namespace Input {

Controller* Controller::current_ = 0;

Controller* 
Controller::current()
{
  return current_;
}

Controller::Controller(const ControllerDescription& desc) :
  buttons(),
  axes(),
  pointers(),
  scrollers(),
  keyboards(),
  events()
{
  current_ = this; 

  const std::vector<int>& button_lst = desc.get_buttons();
  for(std::vector<int>::const_iterator i = button_lst.begin(); i != button_lst.end(); ++i)
  {
    add_button(*i, new ControllerButton(this, *i));
  }

  const std::vector<int>& axis_lst = desc.get_axes();
  for(std::vector<int>::const_iterator i = axis_lst.begin(); i != axis_lst.end(); ++i)
  {
    add_axis(*i, new ControllerAxis(this, *i));
  }

  const std::vector<int>& pointer_lst = desc.get_pointers();
  for(std::vector<int>::const_iterator i = pointer_lst.begin(); i != pointer_lst.end(); ++i)
  {
    add_pointer(*i, new ControllerPointer(this, *i));
  }

  const std::vector<int>& scroller_lst = desc.get_scrollers();
  for(std::vector<int>::const_iterator i = scroller_lst.begin(); i != scroller_lst.end(); ++i)
  {
    add_scroller(*i, new ControllerScroller(this, *i));
  }

  const std::vector<int>& keyboard_lst = desc.get_keyboards();
  for(std::vector<int>::const_iterator i = keyboard_lst.begin(); i != keyboard_lst.end(); ++i)
  {
    add_keyboard(*i, new ControllerKeyboard(this, *i));
  }
}

Controller::~Controller()
{
  for(std::vector<ControllerButton*>::iterator i = buttons.begin(); i != buttons.end(); ++i)
  {
    delete *i;
  }
  for(std::vector<ControllerAxis*>::iterator i = axes.begin(); i != axes.end(); ++i)
  {
    delete *i;
  }
  for(std::vector<ControllerPointer*>::iterator i = pointers.begin(); i != pointers.end(); ++i)
  {
    delete *i;
  }
  for(std::vector<ControllerScroller*>::iterator i = scrollers.begin(); i != scrollers.end(); ++i)
  {
    delete *i;
  }
  for(std::vector<ControllerKeyboard*>::iterator i = keyboards.begin(); i != keyboards.end(); ++i)
  {
    delete *i;
  }
}

ControllerScroller*
Controller::get_scroller(int id) 
{
  if (id >= 0 && id < int(scrollers.size()))
    return scrollers[id];
  else
    return 0;
}

void
Controller::add_scroller(int id, ControllerScroller* scroller) 
{
  if (int(scrollers.size())-1 < id)
    scrollers.resize(id+1);
   
  assert(scrollers[id] == 0);
  scrollers[id] = scroller;
}
  
ControllerPointer*
Controller::get_pointer(int id) 
{
  if (id >= 0 && id < int(pointers.size()))
    return pointers[id];
  else
    return 0;
}

ControllerKeyboard*
Controller::get_keyboard(int id)
{
  if (id >= 0 && id < int(keyboards.size()))
    return keyboards[id];
  else
    return 0;
}

void
Controller::add_keyboard(int id, ControllerKeyboard* keyboard) 
{
  if (int(keyboards.size())-1 < id)
    keyboards.resize(id+1);
   
  assert(keyboards[id] == 0);
  keyboards[id] = keyboard;
}

void
Controller::add_pointer(int id, ControllerPointer* pointer) 
{
  if (int(pointers.size())-1 < id)
    pointers.resize(id+1);
   
  assert(pointers[id] == 0);
  pointers[id] = pointer;
}
  
ControllerAxis*
Controller::get_axis(int id) 
{
  assert(id >= 0 && id < int(axes.size()));
  return axes[id];
}

void
Controller::add_axis(int id, ControllerAxis* axis) 
{
  if (int(axes.size())-1 < id)
    axes.resize(id+1);
   
  assert(axes[id] == 0);
  axes[id] = axis;
}
  
ControllerButton*
Controller::get_button(int id) 
{
  assert(id >= 0 && id < int(buttons.size()));
  return buttons[id];
}

void
Controller::update(float delta)
{
  for(std::vector<ControllerButton*>::iterator i = buttons.begin();
      i != buttons.end(); ++i)
    if (*i) (*i)->update(delta);

  for(std::vector<ControllerAxis*>::iterator i = axes.begin();
      i != axes.end(); ++i)
    if (*i) (*i)->update(delta);

  for(std::vector<ControllerPointer*>::iterator i = pointers.begin();
      i != pointers.end(); ++i)
    if (*i) (*i)->update(delta);

  for(std::vector<ControllerScroller*>::iterator i = scrollers.begin();
      i != scrollers.end(); ++i)
    if (*i) (*i)->update(delta);
}

void
Controller::add_button(int id, ControllerButton* button) 
{
  if (int(buttons.size())-1 < id)
    buttons.resize(id+1);
   
  assert(buttons[id] == 0);
  buttons[id] = button;
}

void
Controller::add_axis_event(int id, float pos)
{
  // std::cout << "Controller::axis_event: id=" << id << " " << pos << std::endl;
  events.push_back(makeAxisEvent(static_cast<EventName>(id), pos));
}

void
Controller::add_button_event(int id, ButtonState state)
{
  // std::cout << "Controller::button_event: id=" << id << " " << state << std::endl;
  events.push_back(makeButtonEvent(static_cast<EventName>(id), state));
}

void
Controller::add_pointer_event(int id, float x, float y)
{
  // std::cout << "Controller::pointer_event: id=" << id << " " << x << ", " << y << std::endl;
  events.push_back(makePointerEvent(static_cast<EventName>(id), x, y));
}

void
Controller::add_scroller_event(int id, float xrel, float yrel)
{
  // std::cout << "Controller::scroller_event: id=" << id << " " << xrel << ", " << yrel << std::endl;
  events.push_back(makeScrollerEvent(static_cast<EventName>(id), xrel, yrel));
}

void
Controller::add_keyboard_event(unsigned short key)
{
  events.push_back(makeKeyboardEvent(key));
}

void
Controller::clear_events()
{
  events.clear();
}

void
Controller::poll_events(std::vector<Event>& out_events)
{
  for(std::vector<Event>::iterator i = events.begin(); i != events.end(); ++i)  
    out_events.push_back(*i);

  events.clear();
}

} // namespace Input

/* EOF */
