#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <iostream>
#include "scene_graph.hpp"
#include "sprite_node.hpp"

SDL_Surface* global_screen;

int main()
{
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Unable to init SDL: " << SDL_GetError() << std::endl;
    exit(1);
  }
  atexit(SDL_Quit);


  global_screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);

  SDL_Event event;

  SDL_Surface* surface = IMG_Load("/home/ingo/projects/pingus/svn/trunk/data/images/pingus/player0/tumble.png");

  SceneGraph graph;
  SpriteNode* sprite = new SpriteNode("/home/ingo/projects/pingus/svn/trunk/data/images/pingus/player0/tumble.png");
  graph.add(sprite);

  sprite->set_pos(Point(100, 100));

  while(1)
    {
      while(SDL_PollEvent(&event)) 
        {
          switch (event.type)
            {
            case SDL_KEYDOWN:
              std::cout << "Keypress" << std::endl;
              break;

            case SDL_QUIT:
              exit(EXIT_SUCCESS);
              break;
            }
        }

      sprite->set_pos(Point(rand()%800, rand()%600));

      graph.render();
      {
        SDL_Rect rect;
        rect.x = 320;
        rect.y = 200;
        SDL_BlitSurface(surface, NULL, global_screen, &rect);
      }

      SDL_Flip(global_screen);

      std::cout << "." << std::endl;
      SDL_Delay(100);
    }
}

/* EOF */

