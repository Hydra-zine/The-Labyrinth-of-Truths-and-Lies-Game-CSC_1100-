
#ifndef NPC_H
#define NPC_H

#include <SDL.h>

class NPC {
public:
  float x, y;
  int r, g, b;
  int width = 50, height = 50;

  void setColour(int level);
  void render(SDL_Renderer *renderer);
};

#endif
