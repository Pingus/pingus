/*
**  Galapix - an image viewer for large image collections
**  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmail.com>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HEADER_PINGUS_MATH_QUAD_TREE_HPP
#define HEADER_PINGUS_MATH_QUAD_TREE_HPP

#include <memory>
#include <vector>

#include "math/rect.hpp"

/**
    +----+----+
    | nw | ne |
    |----+----+
    | sw | se |
    +----+----+
*/
template<class C>
class QuadTreeNode
{
private:
  struct Object {
    Rectf rect;
    C    data;

    Object() :
      rect(),
      data()
    {}
  };

  typedef std::vector<Object> Items;

private:
  Rectf    m_bounding_rect;
  Vector2f m_center;
  Items    m_items;
  int      m_depth;

  std::unique_ptr<QuadTreeNode<C> > m_nw;
  std::unique_ptr<QuadTreeNode<C> > m_ne;
  std::unique_ptr<QuadTreeNode<C> > m_sw;
  std::unique_ptr<QuadTreeNode<C> > m_se;

public:
  QuadTreeNode(int depth, const Rectf& bounding_rect) :
    m_bounding_rect(bounding_rect),
    m_center(bounding_rect.get_center()),
    m_items(),
    m_depth(depth),
    m_nw(),
    m_ne(),
    m_sw(),
    m_se()
  {
  }

  void add(const Rectf& rect, const C& c)
  {
    if (m_depth > 8) // FIXME: max_depth shouldn't be hardcode
    {
      Object obj;
      obj.rect = rect;
      obj.data = c;
      m_items.push_back(obj);
    }
    else
    {
      if (rect.right < m_center.x) // west
      {
        if (rect.bottom < m_center.y) // north
        {
          if (!m_nw.get())
          {
            m_nw.reset(new QuadTreeNode(m_depth+1, Rectf(m_bounding_rect.left, m_bounding_rect.top,
                                                         m_center.x, m_center.y)));
          }
          m_nw->add(rect, c);
        }
        else if(rect.top > m_center.y)  // south
        {
          if (!m_sw.get())
          {
            m_sw.reset(new QuadTreeNode(m_depth+1, Rectf(m_bounding_rect.left, m_center.y,
                                                         m_center.x, m_bounding_rect.bottom)));
          }
          m_sw->add(rect, c);
        }
        else
        {
          Object obj;
          obj.rect = rect;
          obj.data = c;
          m_items.push_back(obj);
        }
      }
      else if (rect.left > m_center.x) // east
      {
        if (rect.bottom < m_center.y) // north
        {
          if (!m_ne.get())
          {
            m_ne.reset(new QuadTreeNode(m_depth+1, Rectf(m_center.x, m_bounding_rect.top,
                                                         m_bounding_rect.right, m_center.y)));
          }
          m_ne->add(rect, c);
        }
        else if(rect.top > m_center.y) // south
        {
          if (!m_se.get())
          {
            m_se.reset(new QuadTreeNode(m_depth+1, Rectf(m_center.x, m_center.y,
                                                         m_bounding_rect.right, m_bounding_rect.bottom)));
          }
          m_se->add(rect, c);
        }
        else
        {
          Object obj;
          obj.rect = rect;
          obj.data = c;
          m_items.push_back(obj);
        }
      }
      else
      {
        Object obj;
        obj.rect = rect;
        obj.data = c;
        m_items.push_back(obj);
      }
    }
  }

  void get_items_at(const Rectf& rect, std::vector<C>& out_items) const
  {
    // If rect overlaps with the given quadrant, recursivly check the quadrant
    if (m_nw.get() &&
        rect.left < m_center.x &&
        rect.top  < m_center.y)
    {
      m_nw->get_items_at(rect, out_items);
    }

    if (m_ne.get() &&
        rect.right > m_center.x &&
        rect.top   < m_center.y)
    {
      m_ne->get_items_at(rect, out_items);
    }

    if (m_sw.get() &&
        rect.left   < m_center.x &&
        rect.bottom > m_center.y)
    {
      m_sw->get_items_at(rect, out_items);
    }

    if (m_se.get() &&
        rect.right  > m_center.x &&
        rect.bottom > m_center.y)
    {
      m_se->get_items_at(rect, out_items);
    }

    // Check all overlapping items
    for(typename Items::const_iterator i = m_items.begin(); i != m_items.end(); ++i)
    {
      if (i->rect.is_overlapped(rect))
      {
        out_items.push_back(i->data);
      }
    }
  }
};

template<class C>
class QuadTree
{
private:
  std::unique_ptr<QuadTreeNode<C> > m_main_node;

public:
  QuadTree(const Rectf& bounding_rect) :
    m_main_node(new QuadTreeNode<C>(0, bounding_rect))
  {
  }

  void add(const Rectf& rect, const C& c)
  {
    m_main_node->add(rect, c);
  }

  std::vector<C> get_items_at(const Rectf& rect)
  {
    std::vector<C> items;
    m_main_node->get_items_at(rect, items);
    return items;
  }
};

#endif

/* EOF */
