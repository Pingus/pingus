//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
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

#include <iostream>
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "font.hpp"
#include "line_iterator.hpp"
#include "font_description.hpp"
#include "gui/display.hpp"

static bool vline_empty(SDL_Surface* surface, int x, Uint8 threshold)
{
  if (x >= surface->w)
    return true;

  Uint8* pixels = (Uint8*)surface->pixels;

  for(int y = 0; y < surface->h; ++y)
    {
      const Uint8& p = pixels[surface->pitch*y + x*surface->format->BytesPerPixel + 3];
      if (p > threshold)
        {
          return false;
        }
    }
  return true;
}

class FontImpl
{
public:
  SDL_Surface* surface;
  SDL_Rect chrs[256];
  int space_length;
  float char_spacing;
  float vertical_spacing;
  
  FontImpl(const FontDescription& desc)
    : surface(0),
      space_length(desc.space_length),
      char_spacing(desc.char_spacing)
  {
    //std::cout << "desc.image: " << desc.image << std::endl;
    //std::cout << "desc.space: " << desc.space_length << std::endl;
    //std::cout << "Characters: " << desc.characters << std::endl;

    for(int i = 0; i < 256; ++i)
      chrs[i].x = chrs[i].y = chrs[i].w = chrs[i].h = 0;

    surface = IMG_Load(desc.image.c_str());
    //std::cout << "IMG: " << desc.image << std::endl;
    assert(surface);

    vertical_spacing = (desc.vertical_spacing == -1) ? surface->h : desc.vertical_spacing;

    if (surface->format->BitsPerPixel != 32)
      {
        std::cout << "Error: '" << desc.filename << "' invalid, fonts need to be RGBA, but is "
                  << surface->format->BitsPerPixel << "bpp" << std::endl;
        assert(0);
      }
        
    SDL_LockSurface(surface);
    
    if (!desc.monospace)
      {
        int first = -1; // -1 signals no character start found yet
        int idx = 0;
        for(int x = 0; x <= surface->w; ++x) // '<=' so we scan one past
          // the last line, to catch
          // the last character
          {
            if (!vline_empty(surface, x, desc.alpha_threshold))
              { // line contains a character
                if (first == -1) 
                  { // found the start of a character
                    first = x;
                  } 
                else 
                  {
                    // do nothing and continue to search for an end
                  }
              }
            else
              { // line doesn't contain a character
                if (first != -1) 
                  { // we have a start and a end, so lets construct a char

                    if (idx < int(desc.characters.size()))
                      {
                        //std::cout << idx << " '" << desc.characters[idx] << "' " 
                        //          <<  " glyph: " << first << " - " << x << std::endl;

                        SDL_Rect& rect = chrs[static_cast<unsigned char>(desc.characters[idx])];
                        rect.x = first;
                        rect.y = 0;
                        rect.w = x - first;
                        rect.h = surface->h;
                      }
                    else
                      {
                        std::cout << "Error: Found more desc.characters then are mapped" << std::endl;
                      }

                    idx += 1;
                    first = -1;
                  }
              }
          }

        if (idx != int(desc.characters.size())) 
          {
            std::cout << "Font: " << desc.image << "\n"
                      << "  Error: glyphs found: " << idx << ", expected "  << desc.characters.size() << "\n"
                      << "  Format: bpp: " << int(surface->format->BitsPerPixel) << "\n"
                      << "  Size: " << surface->w << "x" << surface->h
              //      << "  RMask: " << hex << surface->format->Rmask << "\n"
              //      << "  GMask: " << hex << surface->format->Gmask << "\n"
              //      << "  BMask: " << hex << surface->format->Bmask << "\n"
              //      << "  AMask: " << hex << surface->format->Amask << "\n"
                      << std::endl;
          }
      }
    else // monospace
      {
        assert(surface->w % desc.characters.size() == 0);

        space_length = surface->w / desc.characters.size();
        
        for(int i = 0; i < int(desc.characters.size()); ++i)
          {
            SDL_Rect& rect = chrs[static_cast<unsigned char>(desc.characters[i])];
            
            rect.x = i * space_length;
            rect.y = 0;
            rect.w = space_length;
            rect.h = surface->h;
          }
      }

    SDL_UnlockSurface(surface);
  }

  ~FontImpl()
  {
    SDL_FreeSurface(surface);
  }

  void draw(Origin origin, int x, int y_, const std::string& text, SDL_Surface* target)
  {
    float y = float(y_);
    // FIXME: only origins top_left, top_right and top_center to work right now
    LineIterator it(text);
    while(it.next()) {
      draw_line(origin, x, int(y), it.get(), target);
      y += vertical_spacing;
    }
  }

  void draw_line(Origin origin, int x, int y, const std::string& text, SDL_Surface* target)
  {
    Vector2i offset = calc_origin(origin, get_size(text));

    float dstx = float(x - offset.x);
    float dsty = float(y - offset.y);

    if (!target) target = Display::get_screen();

    for(std::string::size_type i = 0; i < text.size(); ++i)
      {
        if (text[i] == ' ')
          {
            dstx += space_length + char_spacing;
          }
        else
          {
            SDL_Rect& srcrect = chrs[static_cast<unsigned char>(text[i])];
            if (srcrect.w != 0 && srcrect.h != 0)
              {
		SDL_Rect dstrect = { int(dstx), int(dsty), 0, 0 };
                SDL_BlitSurface(surface, &srcrect, target, &dstrect);
                dstx += srcrect.w + char_spacing;
              }
            else
              {
                //std::cout << "Font: character " << static_cast<unsigned char>(text[i]) << " missing in font" << std::endl;
              }
          }
      }
  }

  int get_height() const
  {
    return surface->h;
  }

  int get_width(char idx) const
  {
    return chrs[static_cast<unsigned char>(idx)].w;
  }

  int  get_width(const std::string& text) const
  {
    float width = 0.0f;
    float last_width = 0;
    for(std::string::size_type i = 0; i < text.size(); ++i)
      {
        if (text[i] == ' ')
          {
            width += space_length + char_spacing;
          }
        else if (text[i] == '\n')
          {
            last_width = std::max(last_width, width);
            width = 0;
          }
        else
          {
            width += chrs[static_cast<unsigned char>(text[i])].w + char_spacing;
          }
      }
    return int(std::max(width, last_width));
  }

  Size get_size(const std::string& text) const
  {
    return Size(get_width(text), surface->h);
  }

  Rect bounding_rect(int x, int y, const std::string& str) const
  {
    return Rect(Vector2i(x, y), get_size(str));
  }
};

Font::Font()
{
}

Font::Font(const FontDescription& desc)
  : impl(new FontImpl(desc))
{
}

void
Font::draw(int x, int y, const std::string& text, SDL_Surface* target)
{
  if (impl)
    impl->draw(origin_top_left, x,y,text, target);
}

void
Font::draw(Origin origin, int x, int y, const std::string& text, SDL_Surface* target)
{
  if (impl)
    impl->draw(origin, x,y,text, target); 
}

int
Font::get_height() const
{
  if (impl)
    return impl->get_height();
  else
    return 0;
}

int
Font::get_width(char c) const
{
  if (impl)
    return impl->get_width(c);
  else
    return 0; 
}

int
Font::get_width(const std::string& text) const
{
  if (impl)
    return impl->get_width(text);
  else
    return 0;  
}

Size
Font::get_size(const std::string& str) const
{
  if (impl)
    return impl->get_size(str);
  else
    return Size(); 
}

Rect
Font::bounding_rect(int x, int y, const std::string& str) const
{
  if (impl)
    return impl->bounding_rect(x, y, str);
  else
    return Rect();
}

/* EOF */
