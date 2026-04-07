#include "DialogueBox.h"
#include "Player.h"
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_ttf.h>



void roomSetup(int &level){
  if(level <= 5){
    //queue up dialgue + questions
    //change npc colour
    //change floor
    level+=1;
  }
  else{
    //show win screen
  }
}

void checkAns(bool ans, int &lives){
  //if(ans != question.ans){
  //  lives-=1
  //}
  //
  //if(lives <=0){
  //  gameover()
  //}
}


int main() {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
  SDL_Window *window = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, 800, 600, 0);

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  TTF_Font *font =
      TTF_OpenFont("assets/fonts/BigBlueTerm437NerdFont-Regular.ttf", 18);
  if (!font) {
    SDL_Log("Failed to load font: %s", TTF_GetError());
    return 1; // exit early with an error
  }

  DialogueBox dialogue(font, 800, 600);

  dialogue.enqueue("Testing testing");
  dialogue.enqueue("No way this actually works");
  dialogue.enqueue("Hi GitHub!!");
  dialogue.start(); // had to implement start method to resst everything
                    // properly
  bool running = true;
  SDL_Event e;

  Player player;
  player.x = 300;
  player.y = 200;

  Uint32 lastTime = SDL_GetTicks();

  int level = 1;
  int lives = 3;

  while (running) { // actual game loop

    Uint32 now = SDL_GetTicks();
    float dt = (now - lastTime) / 1000.0f;
    // calculate deltatime, then convert from ms to s
    lastTime = now;

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        running = false;
      if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
        dialogue.advacne();
    }

    player.update(dt);
    dialogue.update(dt);


    //boundaries
    if (!dialogue.active) {
      if ((int)player.x + player.width/2 < 0) {
        player.x = 760;
        roomSetup(level);
      }
      else if ((int)player.x + player.width/2 > 800) {
        player.x = 0;
        roomSetup(level);
      }
    }
    else{
      if ((int)player.x < 0) {
        player.x = 1;
      }
      else if ((int)player.x + player.width > 800) {
        player.x = 800 - player.width - 1;
      }
    }
    if((int)player.y < 0){
      player.y = 1;
    }
    else if((int)player.y + player.height > 600) {
      player.y = 600 - player.height - 1;
    }





    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);
    player.render(renderer);
    dialogue.render(renderer);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();

  return 0;
}
