//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmail.com>
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

#include "engine/display/delta/draw_op_buffer.hpp"

#include <algorithm>

DrawOpBuffer::DrawOpBuffer() :
  draw_ops(),
  mempool()
{
}

DrawOpBuffer::~DrawOpBuffer()
{
}

MemoryPool<DrawOp>&
DrawOpBuffer::get_mempool()
{
  return mempool;
}

void
DrawOpBuffer::clear()
{
  draw_ops.clear();
  mempool.clear();
}

void
DrawOpBuffer::add(DrawOp* op)
{
  draw_ops.push_back(op);
}

bool
DrawOpBuffer::has_op(DrawOp* op) const
{
  // FIXME: This is a slow brute-force approach, a hashmap or
  // something like that could speed things up quite a bit
  for(DrawOps::const_iterator i = draw_ops.begin(); i != draw_ops.end(); ++i)
  {
    if ((*i)->equal(op))
      return true;
  }
  return false;
}

/** Calculate the regions that are different between \a frontbuffer
    and \a backbuffer, results are written to \a changed_regions  */
void
DrawOpBuffer::buffer_difference_slow(const DrawOpBuffer& frontbuffer, const DrawOpBuffer& backbuffer,
                                     std::vector<Rect>& changed_regions)
{
  // FIXME: This is a very slow brute force approach
  for(DrawOps::const_iterator i = backbuffer.draw_ops.begin(); i != backbuffer.draw_ops.end(); ++i)
    if (!frontbuffer.has_op(*i))
      (*i)->mark_changed_regions(changed_regions);

  for(DrawOps::const_iterator i = frontbuffer.draw_ops.begin(); i != frontbuffer.draw_ops.end(); ++i)
    if (!backbuffer.has_op(*i))
      (*i)->mark_changed_regions(changed_regions);
}

bool
DrawOpBuffer::buffer_equal(const DrawOpBuffer& frontbuffer, const DrawOpBuffer& backbuffer)
{
  if (frontbuffer.draw_ops.size() != backbuffer.draw_ops.size())
  {
    return false;
  }
  else
  {
    for(DrawOps::size_type i = 0; i < frontbuffer.draw_ops.size(); ++i)
    {
      if (!frontbuffer.draw_ops[i]->equal(backbuffer.draw_ops[i]))
        return false;
    }
    return true;
  }
}

/** Calculate the regions that are different between \a frontbuffer
    and \a backbuffer, results are written to \a changed_regions  */
void
DrawOpBuffer::buffer_difference(const DrawOpBuffer& frontbuffer, const DrawOpBuffer& backbuffer,
                                std::vector<Rect>& changed_regions)
{
  std::vector<DrawOp*> ops;
  ops.reserve(backbuffer.draw_ops.size() + frontbuffer.draw_ops.size());

  for(DrawOps::const_iterator i = backbuffer.draw_ops.begin(); i != backbuffer.draw_ops.end(); ++i)
    ops.push_back(*i);

  for(DrawOps::const_iterator i = frontbuffer.draw_ops.begin(); i != frontbuffer.draw_ops.end(); ++i)
    ops.push_back(*i);

  std::sort(ops.begin(), ops.end(), ops_id_sorter);

  for(DrawOps::size_type i = 0; i < ops.size(); ++i)
  {
    bool is_equal = false;
    for(DrawOps::size_type j = i+1; j < ops.size() && ops[i]->id == ops[j]->id; ++j)
    {
      if (ops[i]->equal(ops[j]))
      {
        is_equal = true;
        if (j == i+1) // FIXME: This is a bit fishy, since ops_id_sorter() doesn't give a perfect sorting
          i = j;
        break;
      }
    }

    if (!is_equal)
      ops[i]->mark_changed_regions(changed_regions);
  }
}

void
DrawOpBuffer::render(SDLFramebuffer& fb, DrawOpBuffer& frontbuffer)
{
  if (!buffer_equal(frontbuffer, *this))
  {
    std::vector<Rect> changed_regions;

    buffer_difference(frontbuffer, *this, changed_regions);

    // Clip things to the screen
    Size screen_size = fb.get_size();
    for(std::vector<Rect>::iterator i = changed_regions.begin(); i != changed_regions.end(); ++i)
    {
      // FIXME: It might be a good idea to remove empty rectangles here, so that merge_rectangles() can work smoother
      i->left = Math::clamp(0, int(i->left), screen_size.width);
      i->top  = Math::clamp(0, int(i->top),  screen_size.height);

      i->right  = Math::clamp(0, int(i->right),  screen_size.width);
      i->bottom = Math::clamp(0, int(i->bottom), screen_size.height);
    }

    if (!changed_regions.empty())
    {
      // Merge rectangles
      std::vector<Rect> update_rects;
      merge_rectangles(changed_regions, update_rects);

      int area = calculate_area(update_rects);

      if (area < fb.get_size().get_area()*75/100) // FIXME: Random Magic ratio, need benchmarking to find proper value
      { // Update all regions that need update

        for(std::vector<Rect>::iterator i = update_rects.begin(); i != update_rects.end(); ++i)
        {
          // draw things
          fb.push_cliprect(*i);
          for(DrawOps::iterator j = draw_ops.begin(); j != draw_ops.end(); ++j)
          {
            (*j)->render(fb);
          }
          fb.pop_cliprect();
        }

        fb.update_rects(update_rects);
      }
      else
      { // Update the whole screen at once, since we have to many rects
        for(DrawOps::iterator j = draw_ops.begin(); j != draw_ops.end(); ++j)
          (*j)->render(fb);
        fb.flip();
      }
    }
  }
}

/* EOF */
