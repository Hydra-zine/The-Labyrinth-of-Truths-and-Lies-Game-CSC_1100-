#include <SDL.h>

#include <string>

class Player {
public:
  float x, y;
  float speed = 500.0; // pxl/s

  void
  update(float dt) { // dt is the time between the last frame and the current
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

  void render(SDL_Renderer *renderer) {
    SDL_Rect rect = {(int)x, (int)y, 50, 50};
    SDL_SetRenderDrawColor(renderer, 100, 180, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
  }
};

class TextBox {};

int main() {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, 800, 600, 0);

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  bool running = true;
  SDL_Event e;

  Player player;
  player.x = 300;
  player.y = 200;

  Uint32 lastTime = SDL_GetTicks();

  while (running) { // actual game loop

    Uint32 now = SDL_GetTicks();
    float dt = (now - lastTime) / 1000.0f;
    // calculate deltatime, then convert from ms to s
    lastTime = now;

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        running = false;
    }

    player.update(dt);

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);
    player.render(renderer);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
