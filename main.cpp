#include "core/player.h"
#include "core/input.h"
#include "core/enemy.h"
#include "core/map.h"
#include "globals.h"
#include <SDL3/SDL_render.h>
#include <bits/stdc++.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iterator>
#include <string.h>
#include <vector>
#include <map>
using namespace std;

#define global static

typedef struct button_struct {
  bool pressed; 
  SDL_FRect rect; 
  const char *name;
} button_struct;

typedef struct gameState {
  bool running;
  int currentRound;
  int totalRounds;
  bool roundWon;
  int scoreToWin;
  int newLetters;
  int score;
  bool gameOver;
} gameState;

SDL_Window *window;
SDL_Renderer *renderer;
global int letters_size = 16;
global vector<button_struct> button_structs;
global gameState state {
  .running = true,
  .currentRound = 0,
  .totalRounds = 2,
  .roundWon = false,
  .scoreToWin = 10,
  .newLetters = 3,
  .score = 0,
  .gameOver = false,
};
Player::data player;
inputStruct *input = new inputStruct { .keycodes = {}, .mousecodes = {} };
global vector<SDL_Surface*> surfaces;
Uint32 frameDuration;

SDL_Texture *loadTexture(string path) {
  SDL_Surface *surface;
  char *png_path = NULL;
  SDL_asprintf(&png_path, "%s../assets/%s", SDL_GetBasePath(), path.c_str()); 
  surface = SDL_LoadPNG(png_path);

  if (!surface) {
    SDL_Log("Surface Error %s", SDL_GetError());
  }
  surfaces.push_back({surface});

  SDL_free(png_path);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  return texture;
}

SDL_Texture *idleTexture;
SDL_Texture *moveTexture;
SDL_Texture *attackTexture;
SDL_Texture *defendTexture;
SDL_Texture *enemyIdleTexture;
SDL_Texture *tileMapTexture;
SDL_Texture *monastaryTexture;

int main() {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  if (!SDL_CreateWindowAndRenderer("Word", 1920, 1080, SDL_WINDOW_EXTERNAL, &window, &renderer)) {
    SDL_Log("Cannot create window %s\n", SDL_GetError());
  } 

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);

  player = Player::init();
  frameDuration = 80;
  idleTexture = loadTexture("Units/Warrior/Warrior_Idle.png");
  moveTexture = loadTexture("Units/Warrior/Warrior_Run.png");
  attackTexture = loadTexture("Units/Warrior/Warrior_Attack1.png");
  defendTexture = loadTexture("Units/Warrior/Warrior_Guard.png");
  enemyIdleTexture = loadTexture("Units/Red/Warrior/Warrior_Idle.png");
  tileMapTexture = loadTexture("Terrain/Tileset/Tilemap_color2.png");
  monastaryTexture = loadTexture("Buildings/Blue Buildings/Monastery.png");

  while (state.running) {
    SDL_Event event;

    if (state.gameOver) {
    } else {
    }

    while (SDL_PollEvent(&event)) {
      switch(event.type) {
        case SDL_EVENT_QUIT:
          state.running = false;
          break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
          input->mousecodes[event.button.button] = event.button.down;
          break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
          input->mousecodes[event.button.button] = event.button.down;
          break;
        }
        case SDL_EVENT_KEY_DOWN: {
          input->keycodes[event.key.key] = event.key.down;
          break;
        }
        case SDL_EVENT_KEY_UP:
          input->keycodes[event.key.key] = event.key.down;
          break;
      }
    }

    // SDL_FPoint point = { player.rect.x, player.rect.y };
    // if (SDL_PointInRectFloat(&point, &enemy.rect)) {
    //   printf("HIT\n");
    // } else {
    //   printf("NOT HIT\n");
    // }

    Map::update();
    Player::update();
    Enemy::idle();

    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Delay(16);
  }

  for (int i = 0; i < surfaces.size(); i++){
    SDL_DestroySurface(surfaces[i]);
  }
  delete input;
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
