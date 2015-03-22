#include <boost/format.hpp>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "util/log.hpp"

int main(int argc, char** argv)
{
  for(int i = 1; i < argc; ++i)
  {
    SDL_Surface* surface = IMG_Load(argv[i]);
    if (!surface)
    {
      log_error("failed to load: %1%", argv[i]);
    }
    else
    {
      std::cout << argv[i] << ":";

      if (surface->format->palette)
        std::cout << " palette";

      Uint32 colorkey;
      if (SDL_GetColorKey(surface, &colorkey) == 0)
      {
        std::cout << " colorkey:" << colorkey;
      }

      std::cout << " bitsPerPixel:"  << static_cast<int>(surface->format->BitsPerPixel);
      std::cout << " bytesPerPixel:" << static_cast<int>(surface->format->BytesPerPixel);

      std::cout << " Rmask:" << boost::format("%08x") % surface->format->Rmask;
      std::cout << " Gmask:" << boost::format("%08x") % surface->format->Gmask;
      std::cout << " Bmask:" << boost::format("%08x") % surface->format->Bmask;
      std::cout << " Amask:" << boost::format("%08x") % surface->format->Amask;

      std::cout << " Rshift:" << static_cast<int>(surface->format->Rshift);
      std::cout << " Gshift:" << static_cast<int>(surface->format->Gshift);
      std::cout << " Bshift:" << static_cast<int>(surface->format->Bshift);
      std::cout << " Ashift:" << static_cast<int>(surface->format->Ashift);

      std::cout << " Rloss:" << static_cast<int>(surface->format->Rloss);
      std::cout << " Gloss:" << static_cast<int>(surface->format->Gloss);
      std::cout << " Bloss:" << static_cast<int>(surface->format->Bloss);
      std::cout << " Aloss:" << static_cast<int>(surface->format->Aloss);

      std::cout << std::endl;
    }

    SDL_FreeSurface(surface);
  }
  return 0;
}

/* EOF */

