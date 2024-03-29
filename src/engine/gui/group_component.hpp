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

#ifndef HEADER_PINGUS_ENGINE_GUI_GROUP_COMPONENT_HPP
#define HEADER_PINGUS_ENGINE_GUI_GROUP_COMPONENT_HPP

#include "engine/display/drawing_context.hpp"
#include "engine/gui/rect_component.hpp"

#include <memory>

namespace pingus::gui {

class GroupComponent : public RectComponent
{
private:
  typedef std::vector<std::unique_ptr<Component> > Components;
  Components children;
  DrawingContext drawing_context;

  /** Used to detect enter/leave events */
  Component* mouse_over_comp;

  /** Used to decide where keyboard events should go */
  Component* focused_comp;

  Component* grabbed_comp;

  /** Used to do a mouse grab, as long as a button is pressed, the
      component that got the press gets moves and release events */
  Component* primary_pressed_comp;
  Component* secondary_pressed_comp;

public:
  GroupComponent(Rect const& rect, bool clip = true);
  ~GroupComponent() override;

  void draw(DrawingContext& gc) override;
  virtual void draw_background(DrawingContext& gc) {}

  void update(float delta) override;

  void on_primary_button_press(int x, int y) override;
  void on_primary_button_release(int x, int y) override;

  void on_secondary_button_press(int x, int y) override;
  void on_secondary_button_release(int x, int y) override;

  void on_primary_button_click(int x, int y) override {}
  void on_secondary_button_click(int x, int y) override {}

  void on_key_pressed(pingus::input::KeyboardEvent const& ev) override;

  void on_text_input(pingus::input::TextInputEvent const& ev) override;

  void on_pointer_enter() override;
  void on_pointer_leave() override;

  void on_pointer_move(int x, int y) override;

  void add(std::unique_ptr<Component> comp);

  template<typename C, typename ...Args>
  C* create(Args&&... args)
  {
    auto c = std::make_unique<C>(std::forward<Args>(args)...);
    C* result = c.get();
    add(std::move(c));
    return result;
  }

  void update_layout() override;

  bool is_at(int x, int y) override;

  // Causes all input directed to comp
  void grab(Component* comp);
  void ungrab(Component* comp);

  Component* component_at (Vector2i const& pos);
private:
  GroupComponent(GroupComponent const&);
  GroupComponent& operator=(GroupComponent const&);
};

} // namespace pingus::gui

#endif

/* EOF */
