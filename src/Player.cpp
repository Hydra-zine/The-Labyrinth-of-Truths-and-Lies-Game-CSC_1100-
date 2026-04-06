#include "Player.h"

void Player::update(
    float dt) { // dt is the time between the last frame and the current
                // frame multiplying dt with speed gives an equal movement
                // speed regardless if the framerate of the computer
  const Uint8 *keys = SDL_GetKeyboardState(NULL);

  if (keys[SDL_SCANCODE_W])
    y -= speed * dt;
  if (keys[SDL_SCANCODE_S])
    y += speed * dt;
  if (keys[SDL_SCANCODE_A])
    x -= speed * dt;
  if (keys[SDL_SCANCODE_D])
    x += speed * dt;
}

void Player::render(SDL_Renderer *renderer) {

  SDL_Rect rect = {(int)x, (int)y, 50, 50};
  SDL_SetRenderDrawColor(renderer, 100, 180, 255, 255);
  SDL_RenderFillRect(renderer, &rect);
}
