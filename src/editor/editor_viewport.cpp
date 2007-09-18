//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Jason Green <jave27@gmail.com>,
//                     Ingo Ruhnke <grumbel@gmx.de>
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

#include <vector>
#include <string>
#include <iostream>
#include "../gui/gui_manager.hpp"
#include "../gui/display.hpp"
#include "../display/drawing_context.hpp"
#include "../math/vector3f.hpp"
#include "../graphic_context_state.hpp"
#include "editor_level.hpp"
#include "editor_screen.hpp"
#include "editor_viewport.hpp"
#include "level_objs.hpp"

namespace Editor {

// Constructor
EditorViewport::EditorViewport(EditorScreen* e, const Rect& rect_) 
  : RectComponent(rect_),
    state(rect.get_width(), rect.get_height()),
    drawing_context(new DrawingContext(rect)),
    editor(e),
    autoscroll(false),
    highlighted_area(0,0,0,0),
    snap_to(false),
    current_action(NOTHING)
{
}

// Destructor
EditorViewport::~EditorViewport ()
{
  delete drawing_context;
}

void
EditorViewport::on_secondary_button_press(int x_, int y_)
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
EditorViewport::on_secondary_button_release(int x_, int y_)
{
  mouse_world_pos = screen2world(x_, y_);
  mouse_screen_pos = Vector2i(x_, y_);

  if (current_action == SCROLLING)
    current_action = NOTHING;
}

// When someone right-clicks inside the viewport
void
EditorViewport::on_secondary_button_click(int x_, int y_)
{
  mouse_world_pos = screen2world(x_, y_);
  mouse_screen_pos = Vector2i(x_, y_);
}

// Select 1 or more LevelObjs, or drag them.
void 
EditorViewport::on_primary_button_press(int x_, int y_)
{
  mouse_world_pos  = screen2world(x_, y_);
  mouse_screen_pos = Vector2i(x_, y_);
  
  if (current_action == NOTHING)
    {
      LevelObj* obj = object_at(mouse_world_pos.x, mouse_world_pos.y);

      if (obj)
        {
          // If the currently selected object isn't selected, select it and deselect the rest
          if (!obj->is_selected())
            {
              for (unsigned i = 0; i < selected_objs.size(); i++)
                selected_objs[i]->unselect();
                
              selected_objs.clear();
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

          selected_objs.clear();
          selection_changed(selected_objs);
        }
    }
}

void 
EditorViewport::on_primary_button_release(int x_, int y_)
{
  mouse_world_pos = screen2world(x_, y_);
  mouse_screen_pos = Vector2i(x_, y_);

  if (current_action == HIGHLIGHTING)
    {
      highlighted_area.normalize();
      for (unsigned i = 0; i < objs.size(); i++)
        {
          if (highlighted_area.is_inside(Vector2i(int(objs[i]->get_pos().x),
                                                  int(objs[i]->get_pos().y))))
            {
              selected_objs.push_back(objs[i]);
              objs[i]->select();
            }
          else
            objs[i]->unselect();
        }

      selection_changed(selected_objs);
    }
  else if (current_action == DRAGGING)
    {
      // Set the objects' positions for good
      for (unsigned i = 0; i < objs.size(); i++)
        objs[i]->set_orig_pos(objs[i]->get_pos());
    }
  current_action = NOTHING;
}

void
EditorViewport::on_pointer_move(int x_, int y_)
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
              float x_offset = (float)(mouse_world_pos.x - drag_world_pos.x);
              float y_offset = (float)(mouse_world_pos.y - drag_world_pos.y);

              if (snap_to)
                {
                  // FIXME: May need to adjust the snap-to offset here.
                  new_x = (float)((int)((x_offset + orig_pos.x) / 10) * 10);
                  new_y = (float)((int)((y_offset + orig_pos.y) / 10) * 10);
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
EditorViewport::on_key_pressed(const unsigned short c)
{
  std::cout << "EditorViewport::on_key_pressed: " << int(c) << " " << c << std::endl;
}

// Draws all of the objects in the viewport and the background (if any)
void
EditorViewport::draw(DrawingContext &gc)
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
  for (unsigned i = 0; i < objs.size(); i++)
    objs[i]->draw(*drawing_context);

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
EditorViewport::is_at(int x, int y)
{
  return drawing_context->get_rect().is_inside(Vector2i(x,y));
}

void
EditorViewport::update(float delta)
{
  UNUSED_ARG(delta);

  if (current_action == SCROLLING)
    state.set_pos(state.get_pos() + Vector2f(mouse_screen_pos - drag_screen_pos) * delta * 5.0f);

  // Autoscroll if necessary
  if (autoscroll)
    {
      const int autoscroll_border = 10;
      if (autoscroll)
        {
          if (mouse_screen_pos.x < autoscroll_border)
            state.set_pos(state.get_pos() - Vector2f(5, 0));
          else if (Display::get_width() - mouse_screen_pos.x < autoscroll_border)
            state.set_pos(state.get_pos() + Vector2f(5, 0));
          else if (mouse_screen_pos.y < autoscroll_border)
            state.set_pos(state.get_pos() - Vector2f(0, 5));
          else if (Display::get_height() - mouse_screen_pos.y < autoscroll_border)
            state.set_pos(state.get_pos() + Vector2f(0, 5));
        }
    }
}

LevelObj*
EditorViewport::object_at (int x, int y)
{
  // we travel reversly through the object list, so that we get the
  // top-most object
  for (std::vector<LevelObj*>::reverse_iterator i = objs.rbegin ();
       i != objs.rend (); ++i)
    {
      if ((*i)->is_at(x, y))
        return *i;
    }
  return 0;
}

void
EditorViewport::refresh()
{
  state.set_limit(Rect(Vector2i(0,0), editor->get_level()->get_size()).grow(256));
  std::cout << editor->get_level()->get_size().width << ", "
            << editor->get_level()->get_size().height 
            << std::endl;
}

void 
EditorViewport::add_object(LevelObj* obj)
{
  objs.push_back(obj);
}

void
EditorViewport::delete_selected_objects()
{
  for(std::vector<LevelObj*>::iterator i = selected_objs.begin(); i != selected_objs.end(); ++i)
    (*i)->remove();
  
  selected_objs.clear();
  selection_changed(selected_objs);
}

void
EditorViewport::hflip_selected_objects()
{
  for(std::vector<LevelObj*>::iterator i = selected_objs.begin(); i != selected_objs.end(); ++i)
    {
      (*i)->set_modifier(ResourceModifierNS::horizontal_flip((*i)->get_modifier()));
    }  
}

void
EditorViewport::vflip_selected_objects()
{
  for(std::vector<LevelObj*>::iterator i = selected_objs.begin(); i != selected_objs.end(); ++i)
    {
      (*i)->set_modifier(ResourceModifierNS::vertical_flip((*i)->get_modifier()));
    }
}

void
EditorViewport::rotate_90_selected_objects()
{
  for(std::vector<LevelObj*>::iterator i = selected_objs.begin(); i != selected_objs.end(); ++i)
    {
      (*i)->set_modifier(ResourceModifierNS::rotate_90((*i)->get_modifier()));
    }
}

void
EditorViewport::rotate_270_selected_objects()
{
  for(std::vector<LevelObj*>::iterator i = selected_objs.begin(); i != selected_objs.end(); ++i)
    {
      (*i)->set_modifier(ResourceModifierNS::rotate_270((*i)->get_modifier()));
    }
}

Vector2i
EditorViewport::screen2world(int x, int y) const
{
  return Vector2i(state.screen2world(drawing_context->screen_to_world(Vector2i(x, y))));
}

void
EditorViewport::raise_object(LevelObj* obj)
{
  for(std::vector<LevelObj*>::size_type i = 0; i < objs.size(); ++i)
    {
      if (objs[i] == obj)
        {
          if (i != objs.size()-1)
            std::swap(objs[i], objs[i+1]);
          break;
        }
    }
}

void
EditorViewport::lower_object(LevelObj* obj)
{
  for(std::vector<LevelObj*>::size_type i = 0; i < objs.size(); ++i)
    {
      if (objs[i] == obj)
        {
          if (i != 0)
            std::swap(objs[i], objs[i-1]);
          break;
        }
    }
}

void
EditorViewport::raise_object_to_top(LevelObj* obj)
{
  for(std::vector<LevelObj*>::size_type i = 0; i < objs.size(); ++i)
    {
      if (objs[i] == obj)
        {
          for(int j = i; j < int(objs.size()-1); ++j)
            std::swap(objs[j], objs[j+1]);

          break;
        }      
    }
}

void
EditorViewport::lower_object_to_bottom(LevelObj* obj)
{
  for(std::vector<LevelObj*>::size_type i = 0; i < objs.size(); ++i)
    {
      if (objs[i] == obj)
        {
          for(int j = i; j >= 1; --j)
            std::swap(objs[j], objs[j-1]);
          
          break;
        }      
    }
}

void
EditorViewport::raise_objects()
{
  for(std::vector<LevelObj*>::iterator i = selected_objs.begin(); i != selected_objs.end(); ++i)
    {
      raise_object(*i);
    }
}

void
EditorViewport::lower_objects()
{
  for(std::vector<LevelObj*>::iterator i = selected_objs.begin(); i != selected_objs.end(); ++i)
    {
      lower_object(*i);
    }
}

void
EditorViewport::raise_objects_to_top()
{
  for(std::vector<LevelObj*>::iterator i = selected_objs.begin(); i != selected_objs.end(); ++i)
    raise_object_to_top(*i);
}

void
EditorViewport::lower_objects_to_bottom()
{
  for(std::vector<LevelObj*>::iterator i = selected_objs.begin(); i != selected_objs.end(); ++i)
    lower_object_to_bottom(*i); 
}

void
EditorViewport::update_layout()
{
  state.set_size(rect.get_width(), rect.get_height());
  drawing_context->set_rect(rect);
}

void
EditorViewport::clear()
{
  selected_objs.clear();
  for(std::vector<LevelObj*>::iterator i = objs.begin(); i != objs.end(); ++i)
    delete *i;
  objs.clear();
}

} // namespace Editor

/* EOF */
