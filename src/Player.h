#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

class Player {
public:
  float x, y;
  float speed = 500.0; // pxl/s
  int width = 50, height = 50;

  void update(float dt);
  void render(SDL_Renderer *renderer);
};

#endif
