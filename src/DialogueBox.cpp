#include "DialogueBox.h"
#include <string>

DialogueBox::DialogueBox(TTF_Font *font, int screenW, int screenH) {
  this->font = font;
  this->box = {40, screenH - 120 - 40, screenW - 80, 120};
}

void DialogueBox::enqueue(const std::string &line) { dialogue.push(line); }

void DialogueBox::advacne() {
  if (!active)
    active = true; // opens dialogue dialogue box

  if (!finished) { // show full text immediately
    displayedText = fullText;
    finished = true;
    return;
  }

  if (!dialogue.empty()) {
    fullText = dialogue.front();
    dialogue.pop();
    displayedText = "";
    finished = false;
    charTimer = 0.0f;
  } else {
    active = false;
    fullText = "";
    displayedText = "";
  }
}

void DialogueBox::update(float dt) {
  if (!active || finished)
    return; // no need to update

  charTimer += dt; // give accurate seconds on how long its been active for

  float charInterval = 1/this->charsPerSecond; 
  //gives time in seconds till next char to appear
  
  while(charTimer >= charInterval && displayedText.size() < fullText.size()){
    displayedText+=fullText[displayedText.size()]; //gets next char from full text
    charTimer-=charInterval;
  }

  if(displayedText.size() >= fullText.size()){
    finished = true;
  }
}

// I USED CHATGPT FOR THIS FUNCTION TO FORMAT MY TEXTBOX BETTER, THIS ELIMATES
// THE TEDIOUSNESS OF SPENDING HOURS GETTING EVERYTHING IN THE RIGHT PLACE
// MAKING IT LOOK GOOD . THIS ISNT MY CODE. EVRYTHING ELSE IS THO.
void DialogueBox::render(SDL_Renderer *renderer) {
  if (!active)
    return;

  // Draw box background
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, boxColor.r, boxColor.g, boxColor.b,
                         boxColor.a);
  SDL_RenderFillRect(renderer, &box);

  // Draw border
  SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b,
                         borderColor.a);
  SDL_RenderDrawRect(renderer, &box);

  // Render text
  if (!displayedText.empty()) {
    SDL_Surface *surface =
        TTF_RenderText_Blended_Wrapped(font, displayedText.c_str(), textColor,
                                       box.w - padding * 2 // wrap width
        );
    if (surface) {
      SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
      SDL_Rect dst = {box.x + padding, box.y + padding, surface->w, surface->h};
      SDL_RenderCopy(renderer, texture, NULL, &dst);
      SDL_DestroyTexture(texture);
      SDL_FreeSurface(surface);
    }
  }

  // Draw "press space" prompt when text is done
  if (finished && font) {
    SDL_Surface *prompt =
        TTF_RenderText_Blended(font, "[ SPACE ]", {150, 150, 150, 255});
    if (prompt) {
      SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, prompt);
      SDL_Rect dst = {box.x + box.w - prompt->w - padding,
                      box.y + box.h - prompt->h - padding, prompt->w,
                      prompt->h};
      SDL_RenderCopy(renderer, tex, NULL, &dst);
      SDL_DestroyTexture(tex);
      SDL_FreeSurface(prompt);
    }
  }
}
