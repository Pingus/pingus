//  Pingus - A free Lemmings clone
//  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmx.de>
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

#include <assert.h>
#include <iostream>
#include "../math/vector2i.hpp"
#include "../math/size.hpp"
#include "rect_merger.hpp"

struct Mark {
  enum Type { START_MARK = 0, END_MARK = 1 } type;
  int pos;

  Mark(Type type_, int pos_) : type(type_), pos(pos_) {}
};

struct Row {
  int top;
  int bottom;
  std::vector<Mark> marks;
};
 
bool rect_y_sorter(const Rect& lhs, const Rect& rhs)
{
  return lhs.top < rhs.top;
}

bool rect_xy_sorter(const Rect& lhs, const Rect& rhs)
{
  if (lhs.left == rhs.left)
    {
      return lhs.top < rhs.top;
    }
  else
    {
      return lhs.left < rhs.left;
    }
}

bool mark_sorter(const Mark& lhs, const Mark& rhs)
{
  if (lhs.pos == rhs.pos)
    {
      return (lhs.type < rhs.type);
    }
  else
    {
      return (lhs.pos < rhs.pos);
    }
}

void generate_rows(const std::vector<Rect> rects, std::vector<Row>& rows)
{
  // Figure out where we need to split the rectangles
  std::vector<int> row_lines;
  for(std::vector<Rect>::const_iterator i = rects.begin(); i != rects.end(); ++i)
    {
      row_lines.push_back(i->top);
      row_lines.push_back(i->bottom);
    }
  std::sort(row_lines.begin(), row_lines.end());

  // Create rows
  int start = row_lines.front();
  for(std::vector<int>::const_iterator i = row_lines.begin()+1; i != row_lines.end(); ++i)
    {
      if (*i != start)
        {
          Row row;
          row.top    = start;
          row.bottom = *i;

          rows.push_back(row);

          start = *i;
        }
    }
}

void split_rectangles(const std::vector<Rect>& rects, std::vector<Row>& rows)
{ // For this to work rects must be sorted by rects[i].top
  std::vector<Rect>::const_iterator rect_it = rects.begin();
  for(std::vector<Row>::iterator row_it = rows.begin(); row_it != rows.end() && rect_it != rects.end(); ++row_it)
    {
      for(; rect_it->top == row_it->top; ++rect_it)
        {
          Mark start(Mark::START_MARK, rect_it->left);
          Mark end  (Mark::END_MARK,   rect_it->right);

          // Add the gives rectangle to all rows it overlaps
          std::vector<Row>::iterator this_row_it = row_it; 
          do 
            {
              this_row_it->marks.push_back(start);
              this_row_it->marks.push_back(end);

              if (this_row_it->bottom < (rect_it->bottom))
                ++this_row_it;
              else
                break;
            }
          while (1);
        }
    }

  // Sort the marker in the rows
  for(std::vector<Row>::iterator i = rows.begin(); i != rows.end(); ++i)
    std::sort(i->marks.begin(), i->marks.end(), mark_sorter);
}

void generate_rectangles(const std::vector<Row>& rows, std::vector<Rect>& rects_out)
{
  for(std::vector<Row>::const_iterator i = rows.begin(); i != rows.end(); ++i)
    {
      const std::vector<Mark>& marks = i->marks;

      assert(marks.front().type == Mark::START_MARK);

      int start = marks.front().pos;
      int parenthesis_count = 1;
      for(std::vector<Mark>::const_iterator mark_it = marks.begin()+1; mark_it != marks.end(); )
        {
          if (mark_it->type == Mark::END_MARK)
            parenthesis_count -= 1;
          else // if (mark_it->type == START_MARK)
            parenthesis_count += 1;

          if (parenthesis_count == 0)
            {
              if (mark_it+1 != marks.end() && 
                  (mark_it+1)->type == Mark::START_MARK &&
                  (mark_it+1)->pos  == mark_it->pos)
                { 
                  parenthesis_count += 1;
                }
              else
                {
                  rects_out.push_back(Rect(start,        i->top, 
                                           mark_it->pos, i->bottom));
                  ++mark_it;
                  if (mark_it != marks.end())
                    start = mark_it->pos;
                }
            }
          else
            {
              ++mark_it;
            }
        }
    }
}

void merge_vertical_rectangles(std::vector<Rect>& rects_in, std::vector<Rect>& rects_out)
{
  std::sort(rects_in.begin(), rects_in.end(), rect_xy_sorter);

  assert(!rects_in.empty());
  Rect rect = rects_in.front();
  for(std::vector<Rect>::const_iterator i = rects_in.begin()+1; i != rects_in.end();)
    {
      //std::cout << "pos: " << i - rects_in.begin() << "/" << rects_in.size() << std::endl;
      // Expand the rectangle vertically as much as possible
      while(rect.left   == i->left &&
            rect.right  == i->right &&
            rect.bottom == i->top)
        {
          rect.bottom = i->bottom;
          ++i;
          if (i == rects_in.end())
            break;
        }
      rects_out.push_back(rect);

      if (i != rects_in.end())
        {
          rect = *i;
          ++i;
        }
    }
}

void merge_rectangles(std::vector<Rect> rects_in, std::vector<Rect>& rects_out)
{
  if (rects_in.empty())
    return;

  //std::cout << "--- Merge rectangles: " << rects_in.size() << std::endl;

  if (0)
    for(std::vector<Rect>::const_iterator i = rects_in.begin(); i != rects_in.end(); ++i)
      std::cout << "  rectin: " << *i << std::endl;

  // Prepare rectangles
  std::sort(rects_in.begin(), rects_in.end(), rect_y_sorter);

  // Generate rows
  std::vector<Row> rows;
  generate_rows(rects_in, rows);
  split_rectangles(rects_in, rows);

  // Print rows
  if (0)
    for(std::vector<Row>::iterator i = rows.begin(); i != rows.end(); ++i)
      {
        std::cout << "  row: " << i->top << "-" << i->bottom << " - ";
        for(std::vector<Mark>::iterator mark_it = i->marks.begin(); mark_it != i->marks.end(); ++mark_it)
          {
            std::cout << ((mark_it->type == Mark::START_MARK) ? "'(" : "')")
                      << mark_it->pos 
                      << "' ";
          }
        std::cout << std::endl;
      }

  std::vector<Rect> rects_out_step1;
  generate_rectangles(rows, rects_out_step1);
  merge_vertical_rectangles(rects_out_step1, rects_out);
}

#ifdef __TEST__

// g++ -Wall -O2  -D__TEST__ rect_merger.cpp ../math/rect.cpp ../math/origin.cpp -o rect_merger

#include <stdlib.h>

int main()
{
  srand(time(NULL));

  std::vector<Rect> rects_in;
  std::vector<Rect> rects_out;

  // Generate random rectangles
  for(int i = 0; i < 20; ++i)
    {
      rects_in.push_back(Rect(Vector2i(rand() % 800 - 200,
                                       rand() % 800 - 200),
                              Size(rand() % 400 + 10,
                                   rand() % 400 + 10)));
    }

  merge_rectangles(rects_in, rects_out);

  std::cout << "<?xml version='1.0' standalone='no'?>\n"
            << "<!DOCTYPE svg PUBLIC '-//W3C//DTD SVG 1.1//EN' "
            << "'http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd'>\n"
            << "<svg width='800' height='800' "
            << "xmlns='http://www.w3.org/2000/svg' version='1.1'>" << std::endl;

  std::cout << "<g>" << std::endl;
  for(std::vector<Rect>::iterator i = rects_in.begin(); i != rects_in.end(); ++i)
    std::cout << "  <rect fill='blue' style='fill-opacity:0.5' x='" << i->left << "' y='" << i->top 
              << "' width='" << i->get_width() << "' height='" << i->get_height() << "' />" << std::endl;
  std::cout << "</g>" << std::endl;
  std::cout << std::endl;

  std::cout << "<g>" << std::endl;  
  for(std::vector<Rect>::iterator i = rects_out.begin(); i != rects_out.end(); ++i)
    std::cout << "  <rect fill='yellow' stroke='black' style='fill-opacity:0.5' x='" << i->left << "' y='" << i->top 
              << "' width='" << i->get_width() << "' height='" << i->get_height() << "' />" << std::endl;
  std::cout << "</g>" << std::endl;
  std::cout << "</svg>" << std::endl;

  return 0;
}

#endif

/* EOF */
