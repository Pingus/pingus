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
      log_error("failed to load: " << argv[i]);
    }
    else
    {
      std::cout << argv[i] << ":";

      if (surface->format->palette)
        std::cout << " palette";

      if (surface->flags & SDL_SRCCOLORKEY)
        std::cout << " colorkey";

      std::cout << " bitsPerPixel:" << static_cast<int>(surface->format->BitsPerPixel);
      std::cout << " bytesPerPixel:" << static_cast<int>(surface->format->BytesPerPixel);

      std::cout << std::endl;
    }

    SDL_FreeSurface(surface);
  }
  return 0;
}

/* EOF */

