#include "core/player.h"
#include "core/input.h"
#include "core/enemy.h"
#include "core/pathing.h"
#include "globals.h"
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <bits/stdc++.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
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
SDL_Texture *enemyMoveTexture;
SDL_Texture *enemyAttackTexture;
SDL_Texture *tileMapTexture;
SDL_Texture *monastaryTexture;
SDL_Texture *castleTexture;
uint64_t lastCounter;
float MAX_DT = 0.002f;

int main() {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  if (!SDL_CreateWindowAndRenderer("Word", 1920, 1080, SDL_WINDOW_EXTERNAL, &window, &renderer)) {
    SDL_Log("Cannot create window %s\n", SDL_GetError());
  } 

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);

  frameDuration = 80;
  idleTexture = loadTexture("Units/Warrior/Warrior_Idle.png");
  moveTexture = loadTexture("Units/Warrior/Warrior_Run.png");
  attackTexture = loadTexture("Units/Warrior/Warrior_Attack1.png");
  defendTexture = loadTexture("Units/Warrior/Warrior_Guard.png");
  enemyIdleTexture = loadTexture("Units/Red/Warrior/Warrior_Idle.png");
  enemyMoveTexture = loadTexture("Units/Red/Warrior/Warrior_Run.png");
  enemyAttackTexture = loadTexture("Units/Red/Warrior/Warrior_Attack1.png");
  tileMapTexture = loadTexture("Terrain/Tileset/Tilemap_color2.png");
  monastaryTexture = loadTexture("Buildings/Blue Buildings/Monastery.png");
  castleTexture = loadTexture("Buildings/Blue Buildings/Castle.png");
  Map gameMap = Map();
  Player player = Player();

  vector<Enemy> enemies;
  Enemy enemy = Enemy(
    enemyIdleTexture,
    enemyMoveTexture,
    enemyAttackTexture,
    900,
    600
  );
  enemies.push_back(enemy);
  Enemy second = Enemy(
    enemyIdleTexture,
    enemyMoveTexture,
    enemyAttackTexture,
    300,
    600
  );
  enemies.push_back(second);

  while (state.running) {
    SDL_Event event;
    uint64_t PerfCountFrequency = SDL_GetPerformanceFrequency();
    uint64_t lastCounter = SDL_GetPerformanceCounter();

    if (state.gameOver) {
    } else {
    }

    while (SDL_PollEvent(&event)) {
      switch(event.type) {
        case SDL_EVENT_QUIT:
          state.running = false;
          break;
        case SDL_EVENT_MOUSE_MOTION: {
          break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
          input->mousecodes[event.button.button] = event.button;

          int xIndex = event.button.x / 64;
          int yIndex = event.button.y / 64;
          int key = yIndex * gameMap.mapArray[0].size() + xIndex; 

          gameMap.mapNodes[key].clicked = !gameMap.mapNodes[key].clicked;
          gameMap.mapNodes[key].obstacle = !gameMap.mapNodes[key].obstacle;
          break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
          input->mousecodes[event.button.button] = event.button;
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

    gameMap.update(player);
    player.update(enemies, gameMap);
    for (int i = 0; i < enemies.size(); i++) {
      enemies[i].update(&player, gameMap);
    }

    // clear mouse or keycodes
    // input->mousecodes.erase(SDL_BUTTON_LEFT);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderRect(renderer, &player.rect);
    SDL_RenderRect(renderer, &player.entityBox);

    for(auto enemy : enemies) {
      SDL_RenderRect(renderer, &enemy.rect);
    }

    uint64_t EndCounter = SDL_GetPerformanceCounter();
    uint64_t CounterElapsed = EndCounter - lastCounter;
    double MSPerFrame(((1000.0f * (double)CounterElapsed) / (double)PerfCountFrequency));
    double FPS = (double)PerfCountFrequency / (double)CounterElapsed;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* white, full alpha */
    string fps = string("FPS ") + to_string(FPS);
    SDL_SetRenderScale(renderer, 2.0f, 2.0f);
    SDL_RenderDebugText(renderer, 825, 25, fps.c_str());
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);

    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    lastCounter = EndCounter;
  }

  for (int i = 0; i < surfaces.size(); i++){
    SDL_DestroySurface(surfaces[i]);
  }
  delete input;
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
