/*  $Id$
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#ifndef HEADER_QUAD_TREE_HPP
#define HEADER_QUAD_TREE_HPP

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
    Rect rect;
    C    data;
  };

  Rect bounding_rect;
  Vector2i center;
  typedef std::vector<Object> Items;
  Items items;
  int depth;

  std::auto_ptr<QuadTreeNode<C> > nw;
  std::auto_ptr<QuadTreeNode<C> > ne;
  std::auto_ptr<QuadTreeNode<C> > sw;
  std::auto_ptr<QuadTreeNode<C> > se;

public:
  QuadTreeNode(int depth, const Rect& bounding_rect_)
    : bounding_rect(bounding_rect_),
      center((bounding_rect.left + bounding_rect.right)/2,
             (bounding_rect.top  + bounding_rect.bottom)/2),
      nw(0), ne(0), sw(0), se(0)
  {
  }

  void add(const Rect& rect, const C& c)
  {
    if (depth > 8) // max depth
      {
        Object obj;
        obj.rect = rect;
        obj.data = c;
        items.push_back(obj);
      }
    else
      {
        if (rect.is_inside(center))
          { // Rect overlaps all quadrants
            Object obj;
            obj.rect = rect;
            obj.data = c;
            items.push_back(obj);
          }
        else
          {
            if (rect.right <= center.x)
              { // west
                if(rect.top > center.y)
                  {
                    if (!ne)
                      ne = new QuadTreeNode(depth+1, Rect(bounding_rect.left, bounding_rect.top,
                                                          center.x, center.y));

                    ne->add(rect, c);                   
                  }
                else // (rect.top >= center.y)
                  {
                    if (!ne)
                      ne = new QuadTreeNode(depth+1, Rect(bounding_rect.left, bounding_rect.top,
                                                          center.x, center.y));

                    ne->add(rect, c);
                  }
              }
            else // (rect.right > center.x)
              { // east
                
              }
          }
      }
  }

  void get_items_at(const Rect& rect, std::vector<C>& out_items) const
  {
    for(typename Items::const_iterator i = items.begin(); i != items.end(); ++i)
      {
        if (i->rect.overlaps(rect))
          {
            out_items.push_back(i->data);
          }
      }

    // Check other quadrands for overlapping objects
    if (nw && 
        nw->bounding_rect.right > rect.left &&
        nw->bounding_rect.top   > rect.bottom)
      nw->get_items_at(rect, out_items);
    
    if (ne && 
        ne->bounding_rect.left  < rect.right
        ne->bounding_rect.left  < rect.right) 
      ne->get_items_at(rect, out_items);

    if (sw &&
        sw->bounding_rect.right > rect.left &&
        sw->bounding_rect.right > rect.left)
      sw->get_items_at(rect, out_items);

    if (se &&
        se->bounding_rect.right > rect.left &&
        se->bounding_rect.right > rect.left)
      se->get_items_at(rect, out_items);
  }
};

template<class C>
class QuadTree 
{
private:
  std::auto_ptr<QuadTreeNode<C> > main_node;

public: 
  QuadTree(const Rect& bounding_rect)
    : main_node(new QuadTreeNode<C>(0, bounding_rect))
  {
    
  }

  void add(const Rect& rect, const C& c)
  {
    main_node->add(rect, c);
  }

  std::vector<C> get_items_at(const Rect& rect)  
  {
    std::vector<C> items;
    main_node->get_items_at(const Rect& rect, items);
    return items;
  }
};

#endif

/* EOF */
