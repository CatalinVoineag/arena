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

typedef enum enemyState {
  NORMAL,
  HIT,
} enemyState;

class Enemy {
  public:
  SDL_FRect rect;
  SDL_FRect subRect;
  SDL_FRect hitbox;
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
  SDL_Texture* texture;

  Enemy(SDL_Texture*, float, float);
  void update();
  void hit();

  private:
  enemyState state;
  int hitboxOffsetW;
  int hitboxOffsetH;
  int hitDuration;
  Uint32 enemyLastFrameTime;
  Uint32 hitLastFrameTime;
  Uint8 red;
  Uint8 green;
  Uint8 blue;

  void idle();
};
