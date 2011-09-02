//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Jason Green <jave27@gmail.com>,
//                     Ingo Ruhnke <grumbel@gmx.de>
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

#include "editor/viewport.hpp"

#include <iostream>

#include "editor/editor_level.hpp"
#include "engine/display/display.hpp"

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
  selected_objs(),
  highlighted_area(0,0,0,0),
  context_menu(),
  snap_to(false),
  current_action(NOTHING),
  selection_changed()
{
}

// Destructor
Viewport::~Viewport ()
{
  delete drawing_context;
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
  mouse_world_pos  = screen2world(x_, y_);
  mouse_screen_pos = Vector2i(x_, y_);
  
  if (current_action == NOTHING)
  {
    LevelObj* obj = editor->get_level()->object_at(mouse_world_pos.x, mouse_world_pos.y);

    if (obj)
    {
      // If the currently selected object isn't selected, select it and deselect the rest
      if (!obj->is_selected())
      {
        clear_selection();
        obj->select();
        selected_objs.push_back(obj);

        selection_changed(selected_objs);
      }

      for (unsigned i = 0; i < selected_objs.size(); i++)
        selected_objs[i]->set_orig_pos(selected_objs[i]->get_pos());

      // Allow dragging of the currently selected objects
      current_action = DRAGGING;
      drag_world_pos = mouse_world_pos;
    }
    else
    {
      current_action = HIGHLIGHTING;
      highlighted_area.left = highlighted_area.right  = mouse_world_pos.x;
      highlighted_area.top  = highlighted_area.bottom = mouse_world_pos.y;

      clear_selection();

      selection_changed(selected_objs);
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
    for (unsigned i = 0; i < get_objects()->size(); i++)
    {
      if (highlighted_area.contains(Vector2i(int((*get_objects())[i]->get_pos().x),
                                             int((*get_objects())[i]->get_pos().y))))
      {
        selected_objs.push_back((*get_objects())[i]);
        (*get_objects())[i]->select();
      }
      else
        (*get_objects())[i]->unselect();
    }

    selection_changed(selected_objs);
  }
  else if (current_action == DRAGGING)
  {
    // Set the objects' positions for good
    for (unsigned i = 0; i < (*get_objects()).size(); i++)
      (*get_objects())[i]->set_orig_pos((*get_objects())[i]->get_pos());
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

      for (unsigned i = 0; i < selected_objs.size(); i++)
      {
        Vector3f orig_pos(selected_objs[i]->get_orig_pos());
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
        selected_objs[i]->set_pos(Vector3f(new_x, new_y, orig_pos.z));
      }
    }
    break;

    case SCROLLING:
      break;
        
    case NOTHING:
      break;
  }
}

void
Viewport::on_key_pressed(const unsigned short c)
{
  if (c < 256)
  {
    switch(c)
    {
      case 'A':
        clear_selection();
        selection_changed(selected_objs);
        break;

      case 'a':
        if (selected_objs == (*get_objects()))
        {
          clear_selection();
        }
        else 
        {
          clear_selection();
          selected_objs = (*get_objects());
          for (unsigned i = 0; i < selected_objs.size(); i++)
            selected_objs[i]->select();
        }
        selection_changed(selected_objs);
        break;

      case ']':
      case 'w':
        raise_objects();
      break;
            
      case '}':
      case 'W':
        raise_objects_to_top();
      break;

      case '{':
      case 'S':
        lower_objects_to_bottom();
      break;

      case '[':
      case 's':
        lower_objects();
      break;

      case 'r':
        rotate_90_selected_objects();
        break;
            
      case 'R':
        rotate_270_selected_objects();
        break;

      case 8: // backspace
      case 127: // delete
        delete_selected_objects();
        break;
             
      case 'd':
        duplicate_selected_objects();
        break;

      case 'V':
      case 'f':
        hflip_selected_objects();
      break;

      case 'F':
      case 'v':
        vflip_selected_objects();
      break;
            
      case 'c': // dvorak-up
      case 'i': // up
        move_objects(Vector2i(0,-1));
      break;

      case 't': // dvorak-down
      case 'k': // down
        move_objects(Vector2i(0,1));
      break;

      case 'h': // dvorak-left
      case 'j': // left
        move_objects(Vector2i(-1,0));
      break;

      case 'n': // dvorak-right
      case 'l': // right
        move_objects(Vector2i(1,0));
      break;


      case 'C': // dvorak-up
      case 'I': // up
        move_objects(Vector2i(0,-keyboard_movement_distance));
      break;

      case 'T': // dvorak-down
      case 'K': // down
        move_objects(Vector2i(0,keyboard_movement_distance));
      break;

      case 'H': // dvorak-left
      case 'J': // left
        move_objects(Vector2i(-keyboard_movement_distance,0));
      break;

      case 'N': // dvorak-right
      case 'L': // right
        move_objects(Vector2i(keyboard_movement_distance,0));
      break;


      default:
        std::cout << "Viewport::on_key_pressed: " << int(c) << " " << static_cast<char>(c) << std::endl;
        break;
    }
  }
}

// Draws all of the objects in the viewport and the background (if any)
void
Viewport::draw(DrawingContext &gc)
{
  drawing_context->clear();
  drawing_context->fill_screen(Color(155,0,155));
  state.push(*drawing_context);
  
  // Level border
  drawing_context->draw_rect(Rect(Vector2i(0,0), editor->get_level()->get_size()), Color(255,255,255), 5000.0f);
  drawing_context->draw_rect(Rect(Vector2i(0,0), editor->get_level()->get_size()).grow(1), Color(0,0,0), 5000.0f);

  // Safe area
  drawing_context->draw_rect(Rect(Vector2i(0,0), editor->get_level()->get_size()).grow(-100), Color(155,155,155), 5000.0f);
        
  // Draw the level objects
  for (unsigned i = 0; i < (*get_objects()).size(); i++)
    (*get_objects())[i]->draw(*drawing_context);

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
  state.set_limit(Rect(Vector2i(0,0), editor->get_level()->get_size()).grow(256));
}

void
Viewport::duplicate_selected_objects()
{
  std::vector<LevelObj*> new_objs;
  for(std::vector<LevelObj*>::iterator i = selected_objs.begin(); i != selected_objs.end(); ++i)
  {
    LevelObj* clone = (*i)->duplicate(Vector2i(32, 32));
    if (clone)
    {
      new_objs.push_back(clone);
      (*get_objects()).push_back(clone);
      clone->select();
    }
  }
  
  clear_selection();
  selected_objs = new_objs;
  selection_changed(selected_objs);  
}

void
Viewport::delete_selected_objects()
{
  for(std::vector<LevelObj*>::iterator i = selected_objs.begin(); i != selected_objs.end(); ++i)
    (*i)->remove();
  
  (*get_objects()).erase(std::remove_if((*get_objects()).begin(), (*get_objects()).end(), boost::mem_fn(&LevelObj::is_removed)),
                         (*get_objects()).end());

  for(std::vector<LevelObj*>::iterator i = selected_objs.begin(); i != selected_objs.end(); ++i)
    delete (*i);

  selected_objs.clear();
  selection_changed(selected_objs);
}

void
Viewport::hflip_selected_objects()
{
  for(std::vector<LevelObj*>::iterator i = selected_objs.begin(); i != selected_objs.end(); ++i)
  {
    (*i)->set_modifier(ResourceModifier::horizontal_flip((*i)->get_modifier()));
  }  
}

void
Viewport::vflip_selected_objects()
{
  for(std::vector<LevelObj*>::iterator i = selected_objs.begin(); i != selected_objs.end(); ++i)
  {
    (*i)->set_modifier(ResourceModifier::vertical_flip((*i)->get_modifier()));
  }
}

void
Viewport::rotate_90_selected_objects()
{
  for(std::vector<LevelObj*>::iterator i = selected_objs.begin(); i != selected_objs.end(); ++i)
  {
    (*i)->set_modifier(ResourceModifier::rotate_90((*i)->get_modifier()));
  }
}

void
Viewport::rotate_270_selected_objects()
{
  for(std::vector<LevelObj*>::iterator i = selected_objs.begin(); i != selected_objs.end(); ++i)
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
  for(std::vector<LevelObj*>::iterator i = selected_objs.begin(); i != selected_objs.end(); ++i)
  {
    editor->get_level()->raise_object(*i);
  }
}

void
Viewport::lower_objects()
{
  for(std::vector<LevelObj*>::iterator i = selected_objs.begin(); i != selected_objs.end(); ++i)
  {
    editor->get_level()->lower_object(*i);
  }
}

void
Viewport::raise_objects_to_top()
{
  for(std::vector<LevelObj*>::iterator i = selected_objs.begin(); i != selected_objs.end(); ++i)
    editor->get_level()->raise_object_to_top(*i);
}

void
Viewport::lower_objects_to_bottom()
{
  for(std::vector<LevelObj*>::iterator i = selected_objs.begin(); i != selected_objs.end(); ++i)
    editor->get_level()->lower_object_to_bottom(*i); 
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
  for (unsigned i = 0; i < selected_objs.size(); i++)
    selected_objs[i]->unselect();
                
  selected_objs.clear(); 
}

void
Viewport::move_objects(const Vector2i& offset)
{
  for (unsigned i = 0; i < selected_objs.size(); i++)
  {
    Vector3f p = selected_objs[i]->get_pos(); 
    selected_objs[i]->set_pos(Vector3f(p.x + static_cast<float>(offset.x),
                                       p.y + static_cast<float>(offset.y),
                                       p.z));
  }
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

std::vector<LevelObj*>*
Viewport::get_objects()
{
  return editor->get_level()->get_objects();
}

} // namespace Editor

/* EOF */
