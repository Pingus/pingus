#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <format>

#include <logmich/log.hpp>

int main(int argc, char** argv)
{
  for(int i = 1; i < argc; ++i)
  {
    SDL_Surface* surface = IMG_Load(argv[i]);
    if (!surface)
    {
      log_error("failed to load: {}: {}", argv[i], SDL_GetError());
      continue;
    }
    else
    {
      std::cout << argv[i] << ":";

      SDL_PixelFormatDetails const* fmt = SDL_GetPixelFormatDetails(surface->format);

      std::cout << " format:" << SDL_GetPixelFormatName(surface->format);

      if (SDL_GetSurfacePalette(surface))
        std::cout << " palette";

      Uint32 colorkey;
      if (SDL_GetSurfaceColorKey(surface, &colorkey))
      {
        std::cout << " colorkey:" << colorkey;
      }

      std::cout << " bitsPerPixel:"  << static_cast<int>(fmt->bits_per_pixel);
      std::cout << " bytesPerPixel:" << static_cast<int>(fmt->bytes_per_pixel);

      std::cout << " Rmask:" << std::format("{:08x}", fmt->Rmask);
      std::cout << " Gmask:" << std::format("{:08x}", fmt->Gmask);
      std::cout << " Bmask:" << std::format("{:08x}", fmt->Bmask);
      std::cout << " Amask:" << std::format("{:08x}", fmt->Amask);

      std::cout << " Rshift:" << static_cast<int>(fmt->Rshift);
      std::cout << " Gshift:" << static_cast<int>(fmt->Gshift);
      std::cout << " Bshift:" << static_cast<int>(fmt->Bshift);
      std::cout << " Ashift:" << static_cast<int>(fmt->Ashift);

      std::cout << " Rbits:" << static_cast<int>(fmt->Rbits);
      std::cout << " Gbits:" << static_cast<int>(fmt->Gbits);
      std::cout << " Bbits:" << static_cast<int>(fmt->Bbits);
      std::cout << " Abits:" << static_cast<int>(fmt->Abits);

      std::cout << std::endl;
    }

    SDL_DestroySurface(surface);
  }
  return 0;
}

/* EOF */

