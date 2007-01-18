/*  $Id$
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
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

#include <iostream>
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "font.hpp"

static bool vline_empty(SDL_Surface* surface, int x, Uint8 threshold)
{
  Uint8* pixels = (Uint8*)surface->pixels;

  for(int y = 0; y < surface->h; ++y)
    {
      const Uint8& p = pixels[surface->pitch*y + x*surface->format->BytesPerPixel];
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
  int spacing;
  
  FontImpl(const std::string& name)
    : surface(0),
      spacing(20)
  {
    for(int i = 0; i < 256; ++i)
      chrs[i].x = chrs[i].y = chrs[i].w = chrs[i].h = 0;

    std::cout << "Font: Trying to load: " << name << std::endl;
    surface = IMG_Load("data/images/fonts/chalk_large-iso-8859-1.png"); //name.c_str());
    assert(surface);

    SDL_LockSurface(surface);
    
    std::string characters = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞßàáâãäåæçèéêëìíîïğñòóôõö÷øùúûüışÿ";
    std::cout << "Surface: " << surface->w << std::endl;
    int first = -1; // -1 signals no character start found yet
    int chr = 0;
    for(int x = 0; x < surface->w; ++x)
      {
        ///std::cout << x << " " << surface->w << std::endl;
        if (vline_empty(surface, x, 0))
          {
            if (first != -1) // skipping empty space
              {
                if (chr < int(characters.size()))
                  {
                    std::cout << chr << " " << characters[chr]
                              << " Empty: " << first << " - " << x << std::endl;

                    SDL_Rect& rect = chrs[static_cast<unsigned char>(characters[chr])];
                    rect.x = first;
                    rect.y = 0;
                    rect.w = x - first;
                    rect.h = surface->h;
                  }
                else
                  std::cout << "Error: Found more characters then are mapped" << std::endl;

                chr += 1;
                
                first = -1;
              }
          }
        else
          {
            if (first == -1) // found the start of a character
              first = x;
          }
      }
    std::cout << "Font: Found " << chr << " expected "  << characters.size() << std::endl;

    SDL_UnlockSurface(surface);
    std::cout << "Font created successfully" << std::endl;
  }

  ~FontImpl()
  {
    SDL_FreeSurface(surface);
  }

  void draw(int x, int y, const std::string& text, SDL_Surface* target)
  {
    SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;
    for(std::string::size_type i = 0; i < text.size(); ++i)
      {
        if (text[i] == ' ')
          {
            dstrect.x += spacing;
          }
        else
          {
            SDL_Rect& srcrect = chrs[static_cast<unsigned char>(text[i])];
            if (srcrect.w != 0 && srcrect.h != 0)
              {
                SDL_BlitSurface(surface, &srcrect, target, &dstrect);
                dstrect.x += srcrect.w + 1;
              }
            else
              {
                std::cout << "Font: character " << static_cast<unsigned char>(text[i]) << " missing in font" << std::endl;
              }
          }
      }
  }

  void set_alignment(Origin origin)
  {
    
  }

  int get_height()
  {
    return 0;
  }

  int get_width(char)
  {
    return 0;
  }

  Size get_size(const std::string& str)
  {
    return Size();
  }

  Rect bounding_rect(int , int, const std::string& str) const
  {
    return Rect();
  }
};

Font::Font()
  : impl(0)
{
}

Font::Font(const std::string& name)
  : impl(new FontImpl(name))
{
}

void
Font::draw(int x, int y, const std::string& text, SDL_Surface* target)
{
  if (impl)
    impl->draw(x,y,text, target);
}

void
Font::set_alignment(Origin origin)
{
  if (impl)
    impl->set_alignment(origin);
}

int
Font::get_height()
{
  if (impl)
    return impl->get_height();
  else
    return 0;
}

int
Font::get_width(char c)
{
  if (impl)
    return impl->get_width(c);
  else
    return 0; 
}

Size
Font::get_size(const std::string& str)
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
