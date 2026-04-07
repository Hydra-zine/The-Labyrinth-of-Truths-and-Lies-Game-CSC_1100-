#include "NPC.h"

void NPC::setColour(int level){
  if(level == 1){
    r = 255;
    g = 255;
    b = 0;
  }
  else if(level == 2){
    r = 0;
    g = 255;
    b = 0;
  }
  else if(level == 3){
    r = 255;
    g = 0;
    b = 255;
  }
  else if(level == 4){
    r = 0;
    g = 0;
    b = 255;
  }
  else{
    r = 255;
    g = 0;
    b = 0;
  }
}


void NPC::render(SDL_Renderer *renderer) {
  
  SDL_Rect rect = {(int)x, (int)y, width, height};
  SDL_SetRenderDrawColor(renderer, r, g, b, 255);
  SDL_RenderFillRect(renderer, &rect);
}
