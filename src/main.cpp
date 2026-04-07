#include "DialogueBox.h"
#include "NPC.h"
#include "Player.h"
#include "Sentry.h"
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_ttf.h>

struct Statement { //yes i know what structs are, structs are so op i feel like we shouldve learned them
    std::string text;
    bool isTrue;
    std::string person; // whose fact it is
};


void roomSetup(int &level, NPC &npc, DialogueBox &dialogue, std::vector<Statement> &statements, Statement &currentStatement) {
  if (level <= 5) {
    int i = rand() % statements.size();
    currentStatement = statements[i];

    Sentry sentry({"Is this true or false?"}); 

    bool sentryResponse = sentry.getResponse(currentStatement.isTrue);

    dialogue.enqueue(currentStatement.person + " says: \"" + currentStatement.text + "\"");
    if(sentryResponse) dialogue.enqueue("The sentry claims this is TRUE.");
    else dialogue.enqueue("The sentry claims this is FALSE");
    dialogue.enqueue("Head to the right if you think the sentry is right. Head to the left if you think it's wrong.");
    dialogue.start();

    level += 1;
    npc.setColour(level);
  } 
  else {
    // show win screen
  }
}

void checkAns(bool playerGuess, Statement &currentStatement, int &lives, bool &running) {
  if (playerGuess != currentStatement.isTrue) {
    lives -= 1;
    if (lives <= 0) {
      running = false; // trigger game over
    }
  }
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

  std::vector<Statement> statements = {
    {"I served in the Air Force for 6 years", true, "Timmy"},
    {"I was a paramedic", true, "Timmy"},
    {"I watch TV shows in my free time", true, "Mazed"},
    {"I've lost 100+ pounds", true, "Mazed"},
    {"I currently live in Canada", true, "Aayan"},
    {"I've been coding for 7+ years", true, "Aayan"}
    // your groupmates' facts go here too
  };
  Statement currentStatement;

  DialogueBox dialogue(font, 800, 600);

  dialogue.enqueue("Hi!");
  dialogue.enqueue("Welcome to the P vs NP labyrinth!");
  dialogue.enqueue("Your goal is to escape the dungeon alive by going the right way.");
  dialogue.enqueue("There are sentries in every room that tell you if a statement is true or not.");
  dialogue.enqueue("The key to escaping is figuring out if they're telling the turth or not.");
  dialogue.enqueue("If you think the sentry is lying, head to the left.If you think the sentry is telling the truth, then head to the right.");
  dialogue.enqueue("Ready for the first statement? Here it comes:");

  dialogue.start(); // had to implement start method to reset everything
                    // properly
  bool running = true;
  SDL_Event e;

  Player player;
  NPC npc;
  player.x = 300;
  player.y = 200;
  npc.x = 350;
  npc.y = 100;

  Uint32 lastTime = SDL_GetTicks();

  int level = 1;
  int lives = 3;

  roomSetup(level, npc, dialogue, statements,currentStatement);


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
        checkAns(false, currentStatement, lives, running);
        roomSetup(level, npc, dialogue, statements,currentStatement);

      }
      else if ((int)player.x + player.width/2 > 800) {
        player.x = 0;
        checkAns(true, currentStatement, lives, running);
        roomSetup(level, npc, dialogue, statements,currentStatement);

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
    npc.render(renderer);
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
