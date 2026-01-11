#include "core/player.h"
#include "core/input.h"
#include "core/enemy.h"
#include "globals.h"
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
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
uint64_t lastTicks;
float MAX_DT = 0.002f;

float distanceToRect(SDL_FRect start, SDL_FRect end) {
  float x = start.x - end.x;
  float y = start.y - end.y;
  return sqrt(pow(x, 2) + pow(y, 2));
};

void solveAStar(Map &gameMap, Player &player, Enemy &enemy) {
  for (int x = 0; x < gameMap.mapArray[0].size(); x++) {
    for (int y = 0; y < gameMap.mapArray.size(); y++) {
      int key = y * gameMap.mapArray[0].size() + x;
      gameMap.mapNodes[key].seen = false;
      gameMap.mapNodes[key].globalGoal = INFINITY;
      gameMap.mapNodes[key].localGoal = INFINITY;
      gameMap.mapNodes[key].parent = nullptr;	// No parents
    }
  }

  MapNode* start = &gameMap.mapNodes[enemy.mapNodeIndex];
  MapNode* end = &gameMap.mapNodes[player.mapNodeIndex]; 

  MapNode *currentNode = start;
  currentNode->localGoal = 0.0f;
  currentNode->globalGoal = distanceToRect(start->rect, end->rect);

  list<MapNode*> nodeList;
  nodeList.push_back(start);

  while (!nodeList.empty() && currentNode != end) {
    nodeList.sort([](const MapNode* first, const MapNode* second) {
      return first->globalGoal < second->globalGoal;
    });

    while (!nodeList.empty() && nodeList.front()->seen) {
      nodeList.pop_front();
    }

    if (nodeList.empty()) {
      break;
    }

    currentNode = nodeList.front();
    currentNode->seen = true;

    for (auto neighbor : currentNode->neighbors) {
      if (!neighbor->seen && !neighbor->obstacle) {
        nodeList.push_back(neighbor);
      }

      float possibleLocalGoal = currentNode->localGoal + distanceToRect(currentNode->rect, neighbor->rect); 

      if (possibleLocalGoal < neighbor->localGoal) {
        neighbor->parent = currentNode;
        neighbor->localGoal = possibleLocalGoal;
        neighbor->globalGoal = neighbor->localGoal + distanceToRect(neighbor->rect, end->rect);
      }
    } 
  }
}

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
  // Enemy second = Enemy(
  //   enemyIdleTexture,
  //   enemyMoveTexture,
  //   enemyAttackTexture,
  //   300,
  //   600
  // );
  // enemies.push_back(second);

  while (state.running) {
    SDL_Event event;
    uint64_t PerfCountFrequency = SDL_GetPerformanceFrequency();
    uint64_t LastCounter = SDL_GetPerformanceCounter();
    lastTicks = SDL_GetTicks(); 

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
    solveAStar(gameMap, player, enemies[0]);

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

    for(auto obj : obj_coordinates) {
      SDL_RenderRect(renderer, &obj.second);
    }

    uint64_t EndCounter = SDL_GetPerformanceCounter();
    uint64_t CounterElapsed = EndCounter - LastCounter;
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

    LastCounter = EndCounter;
  }

  for (int i = 0; i < surfaces.size(); i++){
    SDL_DestroySurface(surfaces[i]);
  }
  delete input;
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
