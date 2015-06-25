// Pingus - A free Lemmings clone
// Copyright (C) 1998-2009 Ingo Ruhnke <grumbel@gmail.com>
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

#include <iostream>
#include <stdlib.h>

#include "engine/display/delta/rect_merger.hpp"

int main(int argc, char** argv)
{
  unsigned int t = 1216020230; // 20
  //1216032809 // 20
  //int t = time(NULL);
  //int t = 1216035347;

  srand(t);

  std::cerr << t << std::endl;

  std::vector<Rect> rects_in;
  std::vector<Rect> rects_out;

  // Generate random rectangles
  if (0)
    for(int i = 0; i < 400; ++i)
      {
        rects_in.push_back(Rect(Vector2i(rand() % 800,
                                         rand() % 800),
                                Size(rand() % 60 + 10,
                                     rand() % 60 + 10)));
      }

  rects_in.push_back(Rect(380, 279, 412, 311));
  rects_in.push_back(Rect(307, 280, 339, 312));
  rects_in.push_back(Rect(397, 279, 429, 311));
  rects_in.push_back(Rect(354, 280, 386, 312));
  rects_in.push_back(Rect(282, 277, 314, 309));
  rects_in.push_back(Rect(441, 282, 473, 314));
  rects_in.push_back(Rect(531, 281, 563, 313));
  rects_in.push_back(Rect(249, 280, 281, 312));
  rects_in.push_back(Rect(4, 112, 36, 144));
  rects_in.push_back(Rect(381, 279, 413, 311));
  rects_in.push_back(Rect(306, 280, 338, 312));
  rects_in.push_back(Rect(396, 279, 428, 311));
  rects_in.push_back(Rect(355, 280, 387, 312));
  rects_in.push_back(Rect(283, 277, 315, 309));
  rects_in.push_back(Rect(440, 282, 472, 314));
  rects_in.push_back(Rect(530, 281, 562, 313));
  rects_in.push_back(Rect(249, 280, 281, 312));
  rects_in.push_back(Rect(4, 112, 36, 144));

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

/* EOF */
