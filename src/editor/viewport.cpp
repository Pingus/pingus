// Pingus - A free Lemmings clone
// Copyright (C) 2007 Jason Green <jave27@gmail.com>,
//                    Ingo Ruhnke <grumbel@gmail.com>
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

#include "editor/viewport.hpp"

#include <boost/format.hpp>

#include "editor/editor_level.hpp"
#include "editor/editor_screen.hpp"
#include "editor/level_obj.hpp"
#include "editor/group_level_obj.hpp"
#include "engine/display/display.hpp"
#include "util/log.hpp"
#include "util/utf8.hpp"

namespace Editor {

static const int keyboard_movement_distance = 32;

// Constructor
Viewport::Viewport(EditorScreen* e, const Rect& rect_)  :
  RectComponent(rect_),
  state(rect.get_width(), rect.get_height()),
  drawing_context(new DrawingContext(rect)),
  editor(e),
  autoscroll(false),
  mouse_world_pos(),
  mouse_screen_pos(),
  drag_world_pos(),
  drag_screen_pos(),
  selection(),
  highlighted_area(0,0,0,0),
  context_menu(),
  snap_to(false),
  current_action(NOTHING),
  m_background_colors(),
  m_background_colors_idx(12),
  selection_changed()
{
  // Black to White
  m_background_colors.push_back(Color(  0,   0,   0));
  m_background_colors.push_back(Color( 64,  64,  64));
  m_background_colors.push_back(Color(128, 128, 128));
  m_background_colors.push_back(Color(255, 255, 255));
  // Rainbow
  m_background_colors.push_back(Color(255,   0,   0));
  m_background_colors.push_back(Color(255, 255,   0));
  m_background_colors.push_back(Color(255,   0, 255));
  m_background_colors.push_back(Color(  0, 255,   0));
  m_background_colors.push_back(Color(  0, 255, 255));
  m_background_colors.push_back(Color(  0,   0, 255));
  // Dimmed Rainbow
  m_background_colors.push_back(Color(128,   0,   0));
  m_background_colors.push_back(Color(128, 128,   0));
  m_background_colors.push_back(Color(128,   0, 128));
  m_background_colors.push_back(Color(  0, 128,   0));
  m_background_colors.push_back(Color(  0, 128, 128));
  m_background_colors.push_back(Color(  0,   0, 128));
}

// Destructor
Viewport::~Viewport ()
{
}

void
Viewport::on_secondary_button_press(int x_, int y_)
{
  mouse_world_pos = screen2world(x_, y_);
  mouse_screen_pos = Vector2i(x_, y_);

  if (current_action == NOTHING)
  {
    drag_screen_pos = mouse_screen_pos;
    current_action = SCROLLING;
  }
}

void
Viewport::on_secondary_button_release(int x_, int y_)
{
  mouse_world_pos = screen2world(x_, y_);
  mouse_screen_pos = Vector2i(x_, y_);

  if (current_action == SCROLLING)
    current_action = NOTHING;
}

// When someone right-clicks inside the viewport
void
Viewport::on_secondary_button_click(int x_, int y_)
{
  mouse_world_pos = screen2world(x_, y_);
  mouse_screen_pos = Vector2i(x_, y_);
}

// Select 1 or more LevelObjs, or drag them.
void
Viewport::on_primary_button_press(int x_, int y_)
{
  SDL_Keymod key_modifier = SDL_GetModState();

  mouse_world_pos  = screen2world(x_, y_);
  mouse_screen_pos = Vector2i(x_, y_);

  if (current_action == NOTHING)
  {
    LevelObjPtr obj = editor->get_level()->object_at(mouse_world_pos.x, mouse_world_pos.y);

    if (obj)
    {
      if (selection.find(obj) == selection.end())
      {
        // if the clicked on object isn't selected, select it and deselect the rest
        if (key_modifier & KMOD_SHIFT)
        {
          selection.insert(obj);
          selection_changed(selection);
        }
        else
        {
          clear_selection();
          selection.insert(obj);
          selection_changed(selection);
        }
      }
      else
      {
        if (key_modifier & KMOD_SHIFT)
        {
          // if object is already selected and shift is pressed remove
          // it from the selection
          selection.erase(obj);
          selection_changed(selection);
        }
      }

      for (auto it = selection.begin(); it != selection.end(); ++it)
      {
        (*it)->set_orig_pos((*it)->get_pos());
      }

      // Allow dragging of the currently selected objects
      current_action = DRAGGING;
      drag_world_pos = mouse_world_pos;
    }
    else
    {
      current_action = HIGHLIGHTING;
      highlighted_area.left = highlighted_area.right  = mouse_world_pos.x;
      highlighted_area.top  = highlighted_area.bottom = mouse_world_pos.y;

      if (!(key_modifier & KMOD_LSHIFT) && !(key_modifier & KMOD_RSHIFT))
      {
        clear_selection();
        selection_changed(selection);
      }
    }
  }
}

void
Viewport::on_primary_button_release(int x_, int y_)
{
  mouse_world_pos = screen2world(x_, y_);
  mouse_screen_pos = Vector2i(x_, y_);

  if (current_action == HIGHLIGHTING)
  {
    highlighted_area.normalize();
    for (auto it = get_objects()->begin(); it != get_objects()->end(); ++it)
    {
      if (highlighted_area.contains((*it)->get_rect()))
      {
        selection.insert(*it);
      }
    }
    selection_changed(selection);
  }
  else if (current_action == DRAGGING)
  {
    // Set the objects' positions for good
    for (auto it = get_objects()->begin(); it != get_objects()->end(); ++it)
    {
      (*it)->set_orig_pos((*it)->get_pos());
    }
    selection_changed(selection);
  }
  current_action = NOTHING;
}

void
Viewport::on_pointer_move(int x_, int y_)
{
  mouse_world_pos = screen2world(x_, y_);
  mouse_screen_pos = Vector2i(x_, y_);

  switch(current_action)
  {
    case HIGHLIGHTING:
      highlighted_area.right  = mouse_world_pos.x;
      highlighted_area.bottom = mouse_world_pos.y;
      break;

    case DRAGGING:
      {
        float new_x, new_y;

        for (auto it = selection.begin(); it != selection.end(); ++it)
        {
          Vector3f orig_pos((*it)->get_orig_pos());
          float x_offset = static_cast<float>(mouse_world_pos.x - drag_world_pos.x);
          float y_offset = static_cast<float>(mouse_world_pos.y - drag_world_pos.y);

          if (snap_to)
          {
            // FIXME: May need to adjust the snap-to offset here.
            new_x = static_cast<float>(static_cast<int>((x_offset + orig_pos.x) / 10) * 10);
            new_y = static_cast<float>(static_cast<int>((y_offset + orig_pos.y) / 10) * 10);
          }
          else
          {
            new_x = x_offset + orig_pos.x;
            new_y = y_offset + orig_pos.y;
          }
          (*it)->set_pos(Vector3f(new_x, new_y, orig_pos.z));
        }
        selection_changed(selection);
      }
      break;

    case SCROLLING:
      break;

    case NOTHING:
      break;
  }
}

void
Viewport::on_key_pressed(const Input::KeyboardEvent& ev)
{
  switch(ev.keysym.sym)
  {
    case SDLK_g:
      if (ev.keysym.mod & KMOD_SHIFT)
      {
        ungroup_selection();
      }
      else
      {
        group_selection();
      }
      break;

    case SDLK_b:
      if (ev.keysym.mod & KMOD_SHIFT)
      {
        if (m_background_colors_idx > 1)
        {
          m_background_colors_idx -= 1;
        }
        else
        {
          m_background_colors_idx = m_background_colors.size() - 1;
        }
      }
      else
      {
        if (m_background_colors_idx < m_background_colors.size()-1)
        {
          m_background_colors_idx += 1;
        }
        else
        {
          m_background_colors_idx = 0;
        }
      }
      break;

    case SDLK_a:
      if (ev.keysym.mod & KMOD_SHIFT)
      {
        clear_selection();
        selection_changed(selection);
      }
      else
      {
        // FIXME: not quite a perfect way to find out if all objects
        // are selected
        if (selection.size() == get_objects()->size())
        {
          clear_selection();
          selection_changed(selection);
        }
        else
        {
          clear_selection();
          selection.insert(get_objects()->begin(), get_objects()->end());
          selection_changed(selection);
        }
      }
      break;

    case SDLK_PAGEUP:
    case SDLK_RIGHTBRACKET:
    case SDLK_w:
      if (ev.keysym.mod & KMOD_CTRL)
      {
        raise_objects_z_pos();
      }

      if (ev.keysym.mod & KMOD_SHIFT)
      {
        raise_objects_to_top();
      }
      else
      {
        raise_objects();
      }
      break;

    case SDLK_PAGEDOWN:
    case SDLK_LEFTBRACKET:
    case SDLK_s:
      if (ev.keysym.mod & KMOD_CTRL)
      {
        lower_objects_z_pos();
      }

      if (ev.keysym.mod & KMOD_SHIFT)
      {
        lower_objects_to_bottom();
      }
      else
      {
        lower_objects();
      }
      break;

    case SDLK_r:
      if (ev.keysym.mod & KMOD_SHIFT)
      {
        rotate_270_selected_objects();
      }
      else
      {
        rotate_90_selected_objects();
      }
      break;

    case SDLK_DELETE:
    case SDLK_BACKSPACE:
      delete_selected_objects();
      break;

    case SDLK_d:
      duplicate_selected_objects();
      break;

    case SDLK_f:
      if (ev.keysym.mod & KMOD_SHIFT)
      {
        vflip_selected_objects();
      }
      else
      {
        hflip_selected_objects();
      }
      break;

    case SDLK_v:
      if (ev.keysym.mod & KMOD_SHIFT)
      {
        hflip_selected_objects();
      }
      else
      {
        vflip_selected_objects();
      }
      break;

    case SDLK_c: // dvorak-up
    case SDLK_i: // up
      if (ev.keysym.mod & KMOD_SHIFT)
      {
        move_objects(Vector2i(0,-keyboard_movement_distance));
      }
      else
      {
        move_objects(Vector2i(0,-1));
      }
      break;

    case SDLK_t: // dvorak-down
    case SDLK_k: // down
      if (ev.keysym.mod & KMOD_SHIFT)
      {
        move_objects(Vector2i(0, keyboard_movement_distance));
      }
      else
      {
        move_objects(Vector2i(0,1));
      }
      break;

    case SDLK_h: // dvorak-left
    case SDLK_j: // left
      if (ev.keysym.mod & KMOD_SHIFT)
      {
        move_objects(Vector2i(-keyboard_movement_distance, 0));
      }
      else
      {
        move_objects(Vector2i(-1,0));
      }
      break;

    case SDLK_n: // dvorak-right
    case SDLK_l: // right
      if (ev.keysym.mod & KMOD_SHIFT)
      {
        move_objects(Vector2i(keyboard_movement_distance, 0));
      }
      else
      {
        move_objects(Vector2i(1,0));
      }
      break;

    case SDLK_EQUALS:
    case SDLK_PLUS:
      for (auto it = selection.begin(); it != selection.end(); ++it)
      {
        if ((*it)->get_attribs() & HAS_REPEAT)
        {
          (*it)->set_repeat((*it)->get_repeat() + 1);
        }
        else if ((*it)->get_attribs() & HAS_HEIGHT)
        {
          (*it)->set_height((*it)->get_height() + 1);
        }
      }
      selection_changed(selection);
      break;

    case SDLK_MINUS:
      for (auto it = selection.begin(); it != selection.end(); ++it)
      {
        if ((*it)->get_attribs() & HAS_REPEAT)
        {
          if ((*it)->get_repeat() > 1)
          {
            (*it)->set_repeat((*it)->get_repeat() - 1);
          }
        }
        else if ((*it)->get_attribs() & HAS_HEIGHT)
        {
          if ((*it)->get_height() > 1)
          {
            (*it)->set_height((*it)->get_height() - 1);
          }
        }
      }
      selection_changed(selection);
      break;

    default:
      log_debug("Viewport::on_key_pressed: %1%", ev.keysym.sym);
      break;
  }
}

// Draws all of the objects in the viewport and the background (if any)
void
Viewport::draw(DrawingContext &gc)
{
  drawing_context->clear();
  drawing_context->fill_screen(m_background_colors[m_background_colors_idx]);
  state.push(*drawing_context);

  // Center Cross
  {
    Size size = editor->get_level()->get_size();
    Vector2i center(size.width / 2, size.height / 2);

    drawing_context->draw_line(center - Vector2i(16, 0), center + Vector2i(16, 0), Color(155,155,155), 5000.0f);
    drawing_context->draw_line(center - Vector2i(0, 16), center + Vector2i(0, 16), Color(155,155,155), 5000.0f);
  }

  // Level border
  drawing_context->draw_rect(Rect(Vector2i(0,0), editor->get_level()->get_size()), Color(255,255,255), 5000.0f);
  drawing_context->draw_rect(Rect(Vector2i(0,0), editor->get_level()->get_size()).grow(1), Color(0,0,0), 5000.0f);

  // Safe area
  drawing_context->draw_rect(Rect(Vector2i(0,0), editor->get_level()->get_size()).grow(-100), Color(155,155,155), 5000.0f);

  // Draw the level objects
  for (auto it = get_objects()->begin(); it != get_objects()->end(); ++it)
  {
    (*it)->draw(*drawing_context);
  }
  for (auto it = get_objects()->begin(); it != get_objects()->end(); ++it)
  {
    if (selection.find(*it) != selection.end())
    {
      (*it)->draw_selection(*drawing_context);
    }
  }

  if (current_action == HIGHLIGHTING)
  {
    drawing_context->draw_fillrect(highlighted_area, Color(155,200,255, 100), 1000.0f);
    drawing_context->draw_rect(highlighted_area, Color(155,200,255), 1000.0f);
  }

  state.pop(*drawing_context);
  gc.draw(*drawing_context, -150);
}

// Returns true if the viewport is at the x,y coordinate
bool
Viewport::is_at(int x, int y)
{
  return drawing_context->get_rect().contains(Vector2i(x,y));
}

void
Viewport::update(float delta)
{
  if (current_action == SCROLLING)
  {
    state.set_pos(Vector2i(state.get_pos().x + static_cast<int>(static_cast<float>(mouse_screen_pos.x - drag_screen_pos.x) * 5.0f * delta),
                           state.get_pos().y + static_cast<int>(static_cast<float>(mouse_screen_pos.y - drag_screen_pos.y) * 5.0f * delta)));
  }

  // Autoscroll if necessary
  if (autoscroll)
  {
    const int autoscroll_border = 10;
    if (autoscroll)
    {
      if (mouse_screen_pos.x < autoscroll_border)
        state.set_pos(state.get_pos() - Vector2i(5, 0));
      else if (Display::get_width() - mouse_screen_pos.x < autoscroll_border)
        state.set_pos(state.get_pos() + Vector2i(5, 0));
      else if (mouse_screen_pos.y < autoscroll_border)
        state.set_pos(state.get_pos() - Vector2i(0, 5));
      else if (Display::get_height() - mouse_screen_pos.y < autoscroll_border)
        state.set_pos(state.get_pos() + Vector2i(0, 5));
    }
  }
}

void
Viewport::refresh()
{
  state.set_limit(Rect(Vector2i(0,0), editor->get_level()->get_size()).grow(512));
}

void
Viewport::duplicate_selected_objects()
{
  Selection new_selection;
  for(auto i = selection.begin(); i != selection.end(); ++i)
  {
    LevelObjPtr clone = (*i)->duplicate(Vector2i(32, 32));
    if (clone)
    {
      new_selection.insert(clone);
      get_objects()->push_back(clone);
    }
  }

  clear_selection();
  selection = new_selection;
  selection_changed(selection);
}

void
Viewport::delete_selected_objects()
{
  for(auto i = selection.begin(); i != selection.end(); ++i)
    (*i)->remove();

  (*get_objects()).erase(std::remove_if((*get_objects()).begin(), (*get_objects()).end(),
                                        std::bind(&LevelObj::is_removed, std::placeholders::_1)),
                         (*get_objects()).end());

  selection.clear();
  selection_changed(selection);
}

void
Viewport::hflip_selected_objects()
{
  for(auto i = selection.begin(); i != selection.end(); ++i)
  {
    (*i)->set_modifier(ResourceModifier::horizontal_flip((*i)->get_modifier()));
  }
}

void
Viewport::vflip_selected_objects()
{
  for(auto i = selection.begin(); i != selection.end(); ++i)
  {
    (*i)->set_modifier(ResourceModifier::vertical_flip((*i)->get_modifier()));
  }
}

void
Viewport::rotate_90_selected_objects()
{
  for(auto i = selection.begin(); i != selection.end(); ++i)
  {
    (*i)->set_modifier(ResourceModifier::rotate_90((*i)->get_modifier()));
  }
}

void
Viewport::rotate_270_selected_objects()
{
  for(auto i = selection.begin(); i != selection.end(); ++i)
  {
    (*i)->set_modifier(ResourceModifier::rotate_270((*i)->get_modifier()));
  }
}

Vector2i
Viewport::screen2world(int x, int y) const
{
  return state.screen2world(drawing_context->screen_to_world(Vector2i(x, y)));
}

void
Viewport::raise_objects()
{
  for(auto i = selection.begin(); i != selection.end(); ++i)
  {
    editor->get_level()->raise_object(*i);
  }
}

void
Viewport::lower_objects()
{
  for(auto i = selection.begin(); i != selection.end(); ++i)
  {
    editor->get_level()->lower_object(*i);
  }
}

void
Viewport::raise_objects_to_top()
{
  for(auto i = selection.begin(); i != selection.end(); ++i)
    editor->get_level()->raise_object_to_top(*i);
}

void
Viewport::lower_objects_to_bottom()
{
  for(auto i = selection.begin(); i != selection.end(); ++i)
    editor->get_level()->lower_object_to_bottom(*i);
}

void
Viewport::raise_objects_z_pos()
{
  for(auto i = selection.begin(); i != selection.end(); ++i)
  {
    (*i)->set_pos_z((*i)->get_pos_z() + 10.0f);
  }
  selection_changed(selection);
}

void
Viewport::lower_objects_z_pos()
{
  for(auto i = selection.begin(); i != selection.end(); ++i)
  {
    (*i)->set_pos_z((*i)->get_pos_z() - 10.0f);
  }
  selection_changed(selection);
}

void
Viewport::group_selection()
{
  // only group two or more objects
  if (selection.size() > 1)
  {
    log_info("grouping selection");

    std::shared_ptr<GroupLevelObj> group(new GroupLevelObj);
    // iterating over all objects instead of the selection, as that
    // way the relative object order is preserved
    for(auto i = get_objects()->begin(); i != get_objects()->end();)
    {
      // object is member of selection, so add it
      if (selection.find(*i) != selection.end())
      {
        group->add_child(*i);

        // FIXME: ugly, refactor EditorLevel to have some more operations
        // for manipulating the level to avoid this hackery
        i = get_objects()->erase(i);
      }
      else
      {
        ++i;
      }
    }

    // FIXME: could make better guesses on where to insert the group
    // instead of just adding it to the top
    editor->get_level()->add_object(group);

    selection.clear();
    selection.insert(group);
    selection_changed(selection);
  }
}

void
Viewport::ungroup_selection()
{
  log_info("ungrouping selection");
  Selection new_selection;

  for(auto i = selection.begin(); i != selection.end(); ++i)
  {
    std::shared_ptr<GroupLevelObj> group = std::dynamic_pointer_cast<GroupLevelObj>(*i);
    if (group)
    {
      auto group_it = std::find(get_objects()->begin(), get_objects()->end(), group);
      assert(group_it != get_objects()->end());

      for(auto j = group->get_objects().begin(); j != group->get_objects().end(); ++j)
      {
        get_objects()->insert(group_it, *j);
        new_selection.insert(*j);
      }

      // remove the old group object
      group->get_objects().clear();
      get_objects()->erase(group_it);
    }
  }

  selection = new_selection;
  selection_changed(selection);
}

void
Viewport::update_layout()
{
  state.set_size(rect.get_width(), rect.get_height());
  drawing_context->set_rect(rect);
}

void
Viewport::clear_selection()
{
  selection.clear();
}

void
Viewport::move_objects(const Vector2i& offset)
{
  for (auto it = selection.begin(); it != selection.end(); ++it)
  {
    Vector3f p = (*it)->get_pos();
    (*it)->set_pos(Vector3f(p.x + static_cast<float>(offset.x),
                            p.y + static_cast<float>(offset.y),
                            p.z));
  }
  selection_changed(selection);
}

Vector2i
Viewport::get_scroll_pos() const
{
  return state.get_pos();
}

void
Viewport::set_scroll_pos(const Vector2i& pos)
{
  state.set_pos(pos);
}

EditorLevel::Objects*
Viewport::get_objects()
{
  return editor->get_level()->get_objects();
}

} // namespace Editor

/* EOF */
