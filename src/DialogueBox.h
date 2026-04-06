#ifndef DIALOGUEBOX_H
#define DIALOGUEBOX_H

#include <SDL.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <queue>
#include <string>

class DialogueBox {
public:
  SDL_Rect box;
  int padding = 16;

  std::string fullText, displayedText;
  float charsPerSecond = 30.0f;
  float charTimer = 0.0f;

  bool active = false;
  bool finished = false; // true when dialogue completed

  TTF_Font *font = nullptr;

  SDL_Color textColor = {255, 255, 255, 255};
  SDL_Color boxColor = {20, 20, 20, 220};
  SDL_Color borderColor = {200, 200, 200, 255};

  std::queue<std::string>
      dialogue; // using a queue since queues are usually used for dialogue, and
                // yes, i know what queues are

  DialogueBox(TTF_Font *font, int screenW, int screenH);

  void enqueue(const std::string &line);
  void advacne();

  void update(float dt);
  void render(SDL_Renderer *renderer);
};

#endif
