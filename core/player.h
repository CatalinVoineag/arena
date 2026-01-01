#pragma once
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
#include "../globals.h"

namespace Player {
  typedef struct data {
    SDL_FRect rect;
    SDL_FRect subRect;
    int idleAnimationCounter;
    int idleSprites;
    int attackAnimationCounter;
    int attackSprites;
    int defendAnimationCounter;
    int defendSprites;
    int moveAnimationCounter;
    int moveSprites;
    int speed;
    SDL_FlipMode sdl_flip;
  } playerStruct;

  data init();
  void idle();
  void move();
  void attack();
  void defend();
  void update();
}
