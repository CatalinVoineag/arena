#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <bits/stdc++.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iterator>
#include <string.h>
#include <vector>
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

typedef enum playerState {
  IDLE,
  MOVING
} playerState;

typedef struct playerStruct {
  SDL_FRect rect;
  SDL_FRect subRect;
  int animationCounter;
  playerState state;
  int speed;
} playerStruct;

typedef struct keyboardStruct {
  map<int, bool> keycodes;
} keyboardStruct;

global SDL_Window *window = NULL;
global SDL_Renderer *renderer = NULL;
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
global playerStruct player {
  .rect = { .x = 200, .y = 300, .w = 192, .h = 192 },
  .subRect = {},
  .animationCounter = 0,
  .state = IDLE,
  .speed = 5
}; 
global keyboardStruct keyboard {
  .keycodes = {}
};
global Uint32 lastFrameTime = 0;
global Uint32 frameDuration = 80;
global vector<SDL_Surface*> surfaces;
global SDL_Texture *idleTexture;
global SDL_Texture *moveTexture;

void idle() {
  Uint32 now = SDL_GetTicks();

  if (now - lastFrameTime >= frameDuration) {
    lastFrameTime = now;
    player.animationCounter = (player.animationCounter + 1) % 8;
  }

  player.subRect.x = 192 * player.animationCounter;
  player.subRect.y = 0;
  player.subRect.w = 192;
  player.subRect.h = 500;

  SDL_RenderTexture(renderer, idleTexture, &player.subRect, &player.rect);
}

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

bool pressed(int keycode) {
  return keyboard.keycodes[keycode] == true;
}

void move() {
  Uint32 now = SDL_GetTicks();
  if (pressed(SDLK_A)) {
    player.rect.x -= player.speed;
  }
  if (pressed(SDLK_D)) {
    player.rect.x += player.speed;
  }
  if (pressed(SDLK_W)) {
    player.rect.y -= player.speed;
  }
  if (pressed(SDLK_S)) {
    player.rect.y += player.speed;
  }

  if (now - lastFrameTime >= frameDuration) {
    lastFrameTime = now;
    player.animationCounter = (player.animationCounter + 1) % 6;
  }

  player.subRect.x = 192 * player.animationCounter;
  player.subRect.y = 0;
  player.subRect.w = 192;
  player.subRect.h = 500;

  SDL_RenderTexture(renderer, moveTexture, &player.subRect, &player.rect);
}

bool moving() {
  return keyboard.keycodes[SDLK_A] ||
    keyboard.keycodes[SDLK_D] ||
    keyboard.keycodes[SDLK_W] ||
    keyboard.keycodes[SDLK_S];
};

int main() {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  if (!SDL_CreateWindowAndRenderer("Word", 1920, 1080, SDL_WINDOW_EXTERNAL, &window, &renderer)) {
    SDL_Log("Cannot create window %s\n", SDL_GetError());
  } 

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_Keycode keycode;

  idleTexture = loadTexture("Units/Warrior/Warrior_Idle.png");
  moveTexture = loadTexture("Units/Warrior/Warrior_Run.png");

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
          SDL_FPoint point = { event.button.x, event.button.y };
          break;
        }
        case SDL_EVENT_KEY_DOWN: {
          keyboard.keycodes[event.key.key] = event.key.down;
          switch(event.key.key) {
            case SDLK_A: {
              player.state = MOVING;
              break;
            }
            case SDLK_D: {
              player.state = MOVING;
              break;
            }
            case SDLK_S: {
              player.state = MOVING;
              break;
            }
            case SDLK_W: {
              player.state = MOVING;
              break;
            }
          }
          break;
        }
        case SDL_EVENT_KEY_UP:
          keyboard.keycodes[event.key.key] = event.key.down;
          break;
      }
    }

    for (auto keycode : keyboard.keycodes) {
      if (keycode.second == 1) {
        printf("keycode pressed %i %b \n", keycode.first, keycode.second);
      }
    }

    if (moving()) {
      move();
    } else {
      idle();
    }

    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Delay(16);
  }

  for (int i = 0; i < surfaces.size(); i++){
    SDL_DestroySurface(surfaces[i]);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
