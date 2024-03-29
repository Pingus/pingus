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

#ifndef HEADER_PINGUS_ENGINE_INPUT_CONTROL_HPP
#define HEADER_PINGUS_ENGINE_INPUT_CONTROL_HPP

#include <vector>

#include <logmich/log.hpp>
#include <geom/point.hpp>

#include "engine/input/controller.hpp"
#include "engine/input/event.hpp"

namespace pingus::input {

class Control
{
private:
  Control* parent;

public:
  Control(Control* parent_)
    : parent(parent_)
  {}

  virtual ~Control() {
  }

  virtual void notify_parent()
  {
    if (parent)
    {
      parent->update(this);
    }
    else
    {
      log_error("parent missing!");
    }
  }

  virtual void update(float delta) {
  }

  virtual void update(Control* ctrl) {
    log_warn("Control:update() not handled");
  }

private:
  Control(Control const&);
  Control & operator=(Control const&);
};

class Button : public Control
{
protected:
  ButtonState state;

public:
  Button(Control* parent_) :
    Control(parent_),
    state(BUTTON_RELEASED)
  {}

  bool get_state() const { return state; }

  virtual void set_state(ButtonState new_state)
  {
    if (new_state != state)
    {
      state = new_state;
      notify_parent();
    }
  }
};

class ButtonGroup : public Button
{
private:
  std::vector<std::unique_ptr<Button> > buttons;

public:
  ButtonGroup(Control* parent_) :
    Button(parent_),
    buttons()
  {}

  ~ButtonGroup() override
  {
  }

  void add_button(std::unique_ptr<Button> button_) {
    buttons.push_back(std::move(button_));
  }

  void update(float delta) override {
    for(auto i = buttons.begin(); i != buttons.end(); ++i)
      (*i)->update(delta);
  }

  void update(Control* ctrl) override
  {
    ButtonState new_state = BUTTON_RELEASED;

    for(auto i = buttons.begin();
        i != buttons.end(); ++i)
    {
      if ((*i)->get_state() == BUTTON_PRESSED)
        new_state = BUTTON_PRESSED;
    }

    if (new_state != state)
    {
      state = new_state;
      notify_parent();
    }
  }
};

class ControllerButton : public ButtonGroup
{
private:
  Controller* controller;
  int id;

public:
  ControllerButton(Controller* controller_, int id_)
    : ButtonGroup(nullptr),
      controller(controller_),
      id(id_)
  {}

  void notify_parent() override {
    controller->add_button_event(id, state);
  }

private:
  ControllerButton(ControllerButton const&);
  ControllerButton & operator=(ControllerButton const&);
};

class Axis : public Control
{
protected:
  float pos;
  float dead_zone;
  bool  invert;

public:
  Axis(Control* parent_) :
    Control(parent_),
    pos(0.0f),
    dead_zone(0.2f),
    invert(false)
  {}

  float get_pos() const { return pos; }

  virtual void set_state(float new_pos) {
    if (invert)
      new_pos = -new_pos;

    if (std::abs(new_pos) < dead_zone)
      new_pos = 0.0f;

    if (new_pos != pos)
    {
      pos = new_pos;
      notify_parent();
    }
  }
};

class Pointer : public Control
{
protected:
  geom::fpoint pos;
  geom::frect m_limits;

public:
  Pointer(Control* parent_) :
    Control(parent_),
    pos(),
    m_limits(0, 0, 800, 600) // FIXME: this must be updated
  {}

  geom::fpoint get_pos() const { return pos; }

  void set_limits(geom::frect const& limits) {
    m_limits = limits;
  }

  geom::frect get_limits() const {
    return m_limits;
  }

  void set_pos(geom::fpoint const& new_pos) {
    if (pos != new_pos)
    {
      pos = new_pos;
      notify_parent();
    }
  }
};

class Scroller : public Control
{
protected:
  geom::fpoint delta;

public:
  Scroller(Control* parent_) :
    Control(parent_),
    delta(0.0f, 0.0f)
  {}

  geom::fpoint get_delta() const { return delta; }

  void set_delta(geom::fpoint const& new_delta) {
    if (delta != new_delta)
    {
      delta = new_delta;
      notify_parent();
    }
  }
};

class AxisGroup : public Axis {
private:
  std::vector<std::unique_ptr<Axis> > axes;

public:
  AxisGroup(Control* parent_) :
    Axis(parent_),
    axes()
  {}

  ~AxisGroup() override
  {
  }

  void add_axis(std::unique_ptr<Axis> axis) {
    axes.push_back(std::move(axis));
  }

  void update(float delta) override {
    for(auto i = axes.begin(); i != axes.end(); ++i)
      (*i)->update(delta);
  }

  void update(Control* ctrl) override
  {
    float new_pos = 0;

    for(auto i = axes.begin(); i != axes.end(); ++i)
    {
      new_pos += (*i)->get_pos();
    }

    new_pos = std::clamp(new_pos, -1.0f, 1.0f);

    set_state(new_pos);
  }
};

class ControllerAxis : public AxisGroup
{
private:
  Controller* controller;
  int id;

public:
  ControllerAxis(Controller* controller_, int id_)
    : AxisGroup(nullptr),
      controller(controller_),
      id(id_)
  {}

  void notify_parent() override {
    controller->add_axis_event(id, pos);
  }

private:
  ControllerAxis(ControllerAxis const&);
  ControllerAxis & operator=(ControllerAxis const&);
};

class PointerGroup : public Pointer
{
private:
  std::vector<std::unique_ptr<Pointer> > pointer;

public:
  PointerGroup(Control* parent_) :
    Pointer(parent_),
    pointer()
  {}

  ~PointerGroup() override
  {
  }

  void update(Control* p) override {
    Pointer* pointer_ = dynamic_cast<Pointer*>(p);
    assert(pointer_);
    geom::fpoint new_pos = pointer_->get_pos();
    if (new_pos != pos)
    {
      pos = new_pos;
      notify_parent();
    }
  }

  void update(float delta) override {
    for(auto i = pointer.begin(); i != pointer.end(); ++i)
      (*i)->update(delta);
  }

  void add_pointer(std::unique_ptr<Pointer> p) {
    pointer.push_back(std::move(p));
  }
};

class ControllerPointer : public PointerGroup
{
private:
  Controller* controller;
  int id;

public:
  ControllerPointer(Controller* controller_, int id_)
    : PointerGroup(nullptr),
      controller(controller_),
      id(id_)
  {}

  void notify_parent() override {
    controller->add_pointer_event(id, pos.x(), pos.y());
  }

private:
  ControllerPointer(ControllerPointer const&);
  ControllerPointer & operator=(ControllerPointer const&);
};

class ScrollerGroup : public Scroller
{
private:
  std::vector<std::unique_ptr<Scroller> > scrollers;

public:
  ScrollerGroup(Control* parent_) :
    Scroller(parent_),
    scrollers()
  {}

  ~ScrollerGroup() override
  {
  }

  void update(float delta_) override {
    for(auto i = scrollers.begin(); i != scrollers.end(); ++i)
      (*i)->update(delta_);
  }

  void update(Control* p) override {
    Scroller* scroller = dynamic_cast<Scroller*>(p);
    assert(scroller);
    delta = scroller->get_delta();
    notify_parent();
  }

  void add_scroller(std::unique_ptr<Scroller> p) {
    scrollers.push_back(std::move(p));
  }

private:
  ScrollerGroup(ScrollerGroup const&);
  ScrollerGroup & operator=(ScrollerGroup const&);
};

class ControllerScroller : public ScrollerGroup
{
private:
  Controller* controller;
  int id;

public:
  ControllerScroller(Controller* controller_, int id_)
    : ScrollerGroup(nullptr),
      controller(controller_),
      id(id_)
  {}

  void notify_parent() override {
    controller->add_scroller_event(id, delta.x(), delta.y());
  }

private:
  ControllerScroller(ControllerScroller const&);
  ControllerScroller & operator=(ControllerScroller const&);
};

class Keyboard : public Control
{
protected:
  SDL_Event m_ev;

public:
  Keyboard(Control* parent_) :
    Control(parent_),
    m_ev()
  {}

  void send_event(SDL_Event const& ev) { m_ev = ev; notify_parent(); }
  SDL_Event get_ev() { return m_ev; }

private:
  Keyboard(Keyboard const&);
  Keyboard & operator=(Keyboard const&);
};

class KeyboardGroup : public Keyboard
{
private:
  std::vector<std::unique_ptr<Keyboard> > keyboards;

public:
  KeyboardGroup(Control* parent_) :
    Keyboard(parent_),
    keyboards()
  {}

  ~KeyboardGroup() override
  {
  }

  void update(float delta) override {
  }

  void update(Control* p) override {
    m_ev = dynamic_cast<Keyboard*>(p)->get_ev();
    notify_parent();
  }

  void add_keyboard(std::unique_ptr<Keyboard> keyboard)
  {
    keyboards.push_back(std::move(keyboard));
  }
};

class ControllerKeyboard : public KeyboardGroup
{
private:
  Controller* controller;
  int id;

public:
  ControllerKeyboard(Controller* controller_, int id_) :
    KeyboardGroup(nullptr),
    controller(controller_),
    id(id_)
  {}

  void notify_parent() override {
    switch(m_ev.type)
    {
      case SDL_KEYUP:
      case SDL_KEYDOWN:
        controller->add_keyboard_event(m_ev.key);
        break;

      case SDL_TEXTINPUT:
        controller->add_text_input_event(m_ev.text);
        break;

      default:
        log_error("unexpected SDL_Event: {}", m_ev.type);
        break;
    }
  }

private:
  ControllerKeyboard(ControllerKeyboard const&);
  ControllerKeyboard & operator=(ControllerKeyboard const&);
};

} // namespace pingus::input

#endif

/* EOF */
