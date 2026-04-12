#include "DialogueBox.h"
#include "NPC.h"
#include "Player.h"
#include "Sentry.h"
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>
#include <string>

struct Statement { //yes i know what structs are
    std::string text;
    bool isTrue;
    std::string person; // whose fact it is
};


void roomSetup(int &level, NPC &npc, DialogueBox &dialogue, std::vector<Statement> &statements, Statement &currentStatement, bool &playing, bool &won) {
  if (level < 5) {
    int i = rand() % statements.size();
    currentStatement = statements[i];

    statements.erase(statements.begin() + i);

    Sentry sentry({"Is this true or false?"}); 

    bool sentryResponse = sentry.getResponse(currentStatement.isTrue);

    dialogue.enqueue("This sentry claims the following: ");

    dialogue.enqueue(currentStatement.person + " says: \"" + currentStatement.text + "\"");
    dialogue.enqueue("Head right if you think the sentry is correct. Head left if you think it's wrong.");
    dialogue.start();

    level += 1;
    npc.setColour(level);
  } 
  else {
    won = true;
    playing = false;
  }
}

void checkAns(bool playerGuess, Statement &currentStatement, int &lives, bool &playing, bool &won) {
  if (playerGuess != currentStatement.isTrue) {
    lives -= 1;
    if (lives <= 0) {
      won = false;
      playing = false; // trigger game over
    }
  }
}

void renderEndScreen(SDL_Renderer* renderer, TTF_Font* font, bool won) {
  // Background color — green tint for win, red tint for lose
  if (won)
    SDL_SetRenderDrawColor(renderer, 20, 60, 20, 255);
  else
    SDL_SetRenderDrawColor(renderer, 60, 20, 20, 255);
  SDL_RenderClear(renderer);

    // Main message
    //
  std::string message;
  if (won) message = "You Win!";
  else message = "You Lose!";

  SDL_Color white = {255, 255, 255, 255};

  SDL_Surface* surface = TTF_RenderText_Blended(font, message.c_str(), white);
  if (surface) {
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    // centers message on screen

    SDL_Rect dst = {
    (800 - surface->w) / 2,
    (600 - surface->h) / 2 - 40,
    surface->w, surface->h
    };

    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
  }

  std::string subtitle;
  if(won) subtitle = "You escaped the labyrinth!";
  else subtitle = "You ran out of lives...";

  SDL_Surface* subSurface = TTF_RenderText_Blended(font, subtitle.c_str(), white);
  if (subSurface) {
    SDL_Texture* subTex = SDL_CreateTextureFromSurface(renderer, subSurface);

    SDL_Rect dst = {
    (800 - subSurface->w) / 2,
    (600 - subSurface->h) / 2 + 20,
    subSurface->w, subSurface->h
    };

    SDL_RenderCopy(renderer, subTex, NULL, &dst);
    SDL_DestroyTexture(subTex);
    SDL_FreeSurface(subSurface);
  }
  // restart text
  SDL_Color grey = {150, 150, 150, 255};
  SDL_Surface* quit = TTF_RenderText_Blended(font, "Press R to restart", grey);
  if (quit) {
    SDL_Texture* qTex = SDL_CreateTextureFromSurface(renderer, quit);
    SDL_Rect dst = {
      (800 - quit->w) / 2,
      500,
      quit->w, quit->h
      };
    SDL_RenderCopy(renderer, qTex, NULL, &dst);
    SDL_DestroyTexture(qTex);
    SDL_FreeSurface(quit);
  }

  SDL_RenderPresent(renderer);
}


void resetGame(Player &player, NPC &npc, DialogueBox &dialogue, std::vector<Statement> &allStatements,
               std::vector<Statement> &activeStatements, Statement &currentStatement, int &level,
               int &lives, bool &playing, bool &introFinished) 
{

    level = 0;
    lives = 3;
    playing = true;
    introFinished = false;

    activeStatements = allStatements;

    player.x = 300;
    player.y = 200;

    dialogue.empty();
    dialogue.enqueue("Hi!");
    dialogue.enqueue("Welcome to the P vs NP labyrinth!");
    dialogue.enqueue("Head right if you think the statement is true. Head left if false.");
    dialogue.start();
}


void renderHUD(SDL_Renderer *renderer, TTF_Font* font, int lives, int level){


  //rendering lives on top right

  SDL_Color white = {255, 255, 255, 255};
  SDL_Color red = {220, 30, 30, 255};

  std::string livesText = "Lives: " + std::to_string(lives);
  SDL_Surface *livesSurface = TTF_RenderText_Blended(font, livesText.c_str(), red);

  if(livesSurface){
    SDL_Texture *livesTexture = SDL_CreateTextureFromSurface(renderer, livesSurface);
    SDL_Rect dst = {
      800 - livesSurface -> w - 16, //padding
      16,
      livesSurface -> w, livesSurface -> h
    };
    SDL_RenderCopy(renderer, livesTexture, NULL, &dst);
    SDL_DestroyTexture(livesTexture);
    SDL_FreeSurface(livesSurface);
  }

  //rendering level on top left

  std::string levelText = "Room: " + std::to_string(level);
  SDL_Surface* levelSurface = TTF_RenderText_Blended(font, levelText.c_str(), white);

  if(levelSurface){
    SDL_Texture *levelTexture = SDL_CreateTextureFromSurface(renderer, levelSurface);
    SDL_Rect dst = {
      16, 
      16, 
      levelSurface -> w, levelSurface -> h
    };
    SDL_RenderCopy(renderer, levelTexture, NULL, &dst);
    SDL_DestroyTexture(levelTexture);
    SDL_FreeSurface(levelSurface);
  }
}

int main(int argv, char** args) {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
  SDL_Window *window = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, 800, 600, 0);

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  TTF_Font *font =
      TTF_OpenFont("assets/fonts/BigBlueTerm437NerdFont-Regular.ttf", 18);

  std::vector<Statement> allStatements = {
    //Timmy - true
    {"I served in the Air Force for 6 years", true, "Timmy"},
    {"I was a paramedic", true, "Timmy"},
    // Timmy — false
    {"I have a pilot's license", false, "Timmy"},
    {"I have never broken a bone", false, "Timmy"},

    // Mazed — true
    {"I watch TV shows in my free time", true, "Mazed"},
    {"I've lost 100+ pounds", true, "Mazed"},
    // Mazed — false
    {"I've been to every continent", false, "Mazed"},
    {"I once met a celebrity", false, "Mazed"},

    // Aayan — true
    {"I currently live in Canada", true, "Aayan"},
    {"I've been coding for 7+ years", true, "Aayan"},
    // Aayan — false
    {"I can speak five languages fluently", false, "Aayan"},
    {"I have never watched anime", false, "Aayan"},

    // William - true;
    {"I like fashion", true, "William"},
    {"I've played video games since I was 5", true, "William"},

    // Isaac - true
    {"I've been to Japan", true, "Isaac"},
    {"I was in a drumline", true, "Isaac"}

  };

  std::vector<Statement> statements = allStatements;

  Statement currentStatement;

  DialogueBox dialogue(font, 800, 600);

  dialogue.enqueue("Hi!");
  dialogue.enqueue("Welcome to the P vs NP labyrinth!");
  dialogue.enqueue("Your goal is to escape the dungeon alive by going the right way.");
  dialogue.enqueue("There are sentries in every room that tell you if a statement is true or not.");
  dialogue.enqueue("The key to escaping is figuring out if they're telling the truth or not.");
  dialogue.enqueue("If you think the sentry is lying, head to the left.If you think the sentry is telling the truth, then head to the right.");
  dialogue.enqueue("Ready for the first statement? Here it comes:");

  dialogue.start(); // had to implement start method to reset everything
                    // properly
  bool running = true;
  bool playing = true;
  bool introFinished = false;
  bool won = false;
  SDL_Event e;

  Player player;
  NPC npc;
  player.x = 300;
  player.y = 200;
  npc.x = 350;
  npc.y = 100;

  Uint32 lastTime = SDL_GetTicks();

  int level = 0;
  int lives = 3;



  while (running) { // actual game loop

    Uint32 now = SDL_GetTicks();
    float dt = (now - lastTime) / 1000.0f;
    // calculate deltatime, then convert from ms to s
    lastTime = now;

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        running = false;
      if(e.type == SDL_KEYDOWN){
        SDL_Keycode key = e.key.keysym.sym;
        if (key == SDLK_SPACE) dialogue.advance();
        else if (!playing && key == SDLK_r) resetGame(player, npc, dialogue, allStatements,
            statements, currentStatement, level, lives,playing, introFinished);

      }
    }

    player.update(dt);
    dialogue.update(dt);


    //boundaries
    if (!dialogue.active) {
      if(!introFinished){
        introFinished = true;
        roomSetup(level, npc, dialogue, statements,currentStatement, playing, won);
      }
      if ((int)player.x + player.width/2 < 0) {
        player.x = 760;
        checkAns(false, currentStatement, lives, playing, won);
        roomSetup(level, npc, dialogue, statements,currentStatement, playing, won);

      }
      else if ((int)player.x + player.width/2 > 800) {
        player.x = 0;
        checkAns(true, currentStatement, lives, playing, won);
        roomSetup(level, npc, dialogue, statements,currentStatement, playing, won);

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



    if (playing) {
      SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
      SDL_RenderClear(renderer);
      npc.render(renderer);
      player.render(renderer);
      dialogue.render(renderer);
      renderHUD(renderer, font, lives, level);
      SDL_RenderPresent(renderer);
    } 
    else {
      renderEndScreen(renderer, font, won);
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();

  return 0;
}
