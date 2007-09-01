#include <stdexcept>
#include "SDL.h"
#include "manager.hpp"

int main()
{
  try {
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Unable to init SDL: " << SDL_GetError() << std::endl;
    exit(1);
  }
  atexit(SDL_Quit);

  SDL_Surface* screen = SDL_SetVideoMode(640, 480, 0, 0);

  Input::Manager manager;

  manager.load("../../data/controller/input2.scm");

  while(true)
    {
      manager.update(0.033);
      
      SDL_Delay(33);
      SDL_Flip(screen);
    }
  } catch (std::exception& err) {
    std::cout << "Exception: " << err.what() << std::endl;
  }

  return 0;
}

/* EOF */
