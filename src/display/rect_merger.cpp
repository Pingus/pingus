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

#include <ext/algorithm>
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

// [top, bottom[
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

void print_rows(std::ostream& out, const std::vector<Row>& rows)
{
  for(std::vector<Row>::const_iterator i = rows.begin(); i != rows.end(); ++i)
    {
      out << "  row: " << i->top << " -> " << i->bottom << " - ";
      for(std::vector<Mark>::const_iterator mark_it = i->marks.begin(); mark_it != i->marks.end(); ++mark_it)
        {
          out << ((mark_it->type == Mark::START_MARK) ? "'(" : "')")
                    << mark_it->pos 
                    << ((mark_it->type == Mark::START_MARK) ? "(' " : ")' ");
        }
      out << std::endl;
    }
}

void print_rects(std::ostream& out, const std::vector<Rect>& rects)
{
  for(std::vector<Rect>::const_iterator i = rects.begin(); i != rects.end(); ++i)
    {
      out << *i << std::endl;
    }
}

/** Take a list of rectangles and generate a list of rows written to
    \a rows_out. The rows are returned empty and have to be filled via
    split_rectangles()

    @param[in]  rects     List of rectangles used to generate rows_out
    @param[out] rows_out  Empty vector that get filled with rows
 */
void generate_rows(const std::vector<Rect>& rects, std::vector<Row>& rows_out)
{
  // Figure out the horizontal split lines
  std::vector<Mark> marks;
  for(std::vector<Rect>::const_iterator i = rects.begin(); i != rects.end(); ++i)
    {
      marks.push_back(Mark(Mark::START_MARK, i->top));
      marks.push_back(Mark(Mark::END_MARK,   i->bottom));
    }
  std::sort(marks.begin(), marks.end(), mark_sorter);
  
  assert(!marks.empty());
  assert(marks.front().type == Mark::START_MARK);
  assert(marks.back().type  == Mark::END_MARK);

  // Generate rows from the splitlines
  std::vector<Mark>::const_iterator start = marks.begin();
  for(std::vector<Mark>::const_iterator i = marks.begin()+1; i != marks.end(); ++i)
    { // FIXME: This will generate empty rows (harmless, but not pretty)
      
      if (i->pos != start->pos)
        {
          Row row;
          row.top    = start->pos;
          row.bottom = i->pos;
            
          rows_out.push_back(row);
            
          start = i;
        }
    }

  assert(!rows_out.empty());
}

/** Takes a list of rectangles and adds their left and right borders to rows

    @param[in]     rects List of rectangles that get split up and filled into rows, must be sorted by rect_y_sorter
    @param[in,out] rows  List of rows where the markers are filled in
 */
void split_rectangles(const std::vector<Rect>& rects, std::vector<Row>& rows)
{
  assert(__gnu_cxx::is_sorted(rects.begin(), rects.end(), rect_y_sorter));

  std::vector<Rect>::const_iterator rect = rects.begin();
  for(std::vector<Row>::iterator row = rows.begin(); row != rows.end() && rect != rects.end(); ++row)
    {
      for(; rect->top == row->top; ++rect)
        {
          Mark start(Mark::START_MARK, rect->left);
          Mark end  (Mark::END_MARK,   rect->right);

          // Add the gives rectangle to all rows it overlaps
          std::vector<Row>::iterator this_row = row; 
          do 
            {
              this_row->marks.push_back(start);
              this_row->marks.push_back(end);

              if (this_row->bottom < (rect->bottom))
                ++this_row;
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

/** Takes a list of rows along with markers in them to then generate a
    list of rectangles which are written to \a rects_out

    @param[in]  rows       List of rows used to generate rects_out
    @param[out] rects_out  Empty vector into which the newly generated rects are added 
*/
void generate_rectangles(const std::vector<Row>& rows, std::vector<Rect>& rects_out)
{
  for(std::vector<Row>::const_iterator i = rows.begin(); i != rows.end(); ++i)
    {
      const std::vector<Mark>& marks = i->marks;

      if (!marks.empty())
        {
          assert(marks.front().type == Mark::START_MARK);
          if (0)
            {
              std::cerr << "Size: " << i->marks.size() << std::endl;
              if (marks.front().type != Mark::START_MARK)
                {
                  for(std::vector<Mark>::const_iterator mark_it = marks.begin(); mark_it != marks.end(); )
                    std::cerr << ((mark_it->type == Mark::START_MARK) ? "'(" : "')")
                              << mark_it->pos 
                              << "' ";            
                  assert(!"False");
                }
            }

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
                  if ((mark_it+1) != marks.end() && 
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
}

/** Takes a list of rectangles and merges non overlapping vertically
    adjacent rectangles that have the same left and right borders 

    @param[in]  rects     List of rectangles to be merged, must be sorted with rect_xy_sorter
    @param[out] rects_out Empty vector into which the newly merged rects are added
*/
void merge_vertical_rectangles(const std::vector<Rect>& rects, std::vector<Rect>& rects_out)
{  
  assert(__gnu_cxx::is_sorted(rects.begin(), rects.end(), rect_xy_sorter));
  assert(!rects.empty());
  
  Rect rect = rects.front();
  for(std::vector<Rect>::const_iterator i = rects.begin()+1; i != rects.end(); ++i)
    {
      // Merge rect with i
      if (rect.left   == i->left &&
          rect.right  == i->right &&
          rect.bottom == i->top)
        {
          rect.bottom = i->bottom;
        }
      else
        {
          rects_out.push_back(rect);
          rect = *i;
        }
    }
  rects_out.push_back(rect);

  assert(rects.size() >= rects_out.size());
}

/** Takes a list of overlapping rectangles and generates a list of
    non-overlapping rectangles covering the same area. Rectangles have to be normalized.
 */
void merge_rectangles(const std::vector<Rect>& rects_, std::vector<Rect>& rects_out)
{
  if (rects_.empty())
    return;

  std::vector<Rect> rects = rects_;

  // Prepare rectangles
  std::sort(rects.begin(), rects.end(), rect_y_sorter);

  // Generate rows
  std::vector<Row> rows;
  generate_rows(rects, rows);
  split_rectangles(rects, rows);

  //print_rows(std::cout, rows);

  std::vector<Rect> rects_out_step1;
  generate_rectangles(rows, rects_out_step1);

  //print_rects(std::cout, rects_out_step1);

  std::sort(rects_out_step1.begin(), rects_out_step1.end(), rect_xy_sorter);
  merge_vertical_rectangles(rects_out_step1, rects_out);
  //print_rects(std::cerr, rects_out);
}

#ifdef __TEST__

// g++ -Wall -O2  -D__TEST__ rect_merger.cpp ../math/rect.cpp ../math/origin.cpp -o rect_merger

#include <stdlib.h>

int main(int argc, char** argv)
{
  int t = 1216020230; // 20
  //1216032809 // 20
  //int t = time(NULL);
  //int t = 1216035347; 

  srand(t);

  std::cerr << t << std::endl;

  std::vector<Rect> rects_in;
  std::vector<Rect> rects_out;

  // Generate random rectangles
  for(int i = 0; i < 400; ++i)
    {
      rects_in.push_back(Rect(Vector2i(rand() % 800,
                                       rand() % 800),
                              Size(rand() % 60 + 10,
                                   rand() % 60 + 10)));
    }

  merge_rectangles(rects_in, rects_out);
  
  if (1) // print results as SVG 
    {
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
        std::cout << "  <rect fill='yellow' stroke='black' style='fill-opacity:0.5;stroke-width:1px' x='" << i->left << "' y='" << i->top 
                  << "' width='" << i->get_width() << "' height='" << i->get_height() << "' />" << std::endl;
      std::cout << "</g>" << std::endl;
      std::cout << "</svg>" << std::endl;
    }

  return 0;
}

/*

  if (1)
    {
      std::cout << "--- Merge rectangles: " << rects_in.size() << std::endl;
      for(std::vector<Rect>::const_iterator i = rects_in.begin(); i != rects_in.end(); ++i)
        {
          std::cout << "  rectin: " << *i << std::endl;
        }
    }

  for(std::vector<Row>::const_iterator i = rows.begin(); i != rows.end(); ++i)
    {
      std::cout << "Row: " << i->marks.size() << " ";
      for(std::vector<Mark>::const_iterator j = i->marks.begin(); j != i->marks.end(); ++j)
        {
          std::cout << ((j->type == Mark::START_MARK) ? "'(" : "')")
                      << j->pos 
                      << "' ";          
        }
      std::cout << std::endl;
    }

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

 */

#endif

/* EOF */
