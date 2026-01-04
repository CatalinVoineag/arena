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
#include <vector>
#include "../globals.h"
#include "enemy.h"

class Enemy;

class Player {
  public:
    SDL_FRect rect;
    SDL_FRect subRect;
    SDL_FRect hitbox;
    int health;
    int damage;
    int idleAnimationCounter;
    int idleSprites;
    int attackAnimationCounter;
    int attackSprites;
    int defendAnimationCounter;
    int defendSprites;
    int moveAnimationCounter;
    int moveSprites;
    float speed;
    SDL_FlipMode sdl_flip;
    bool midAnimation;

    Player();
    void update(vector<Enemy> &enemies);

  private:
    Uint32 lastFrameTime = 0;
    int hitboxOffsetW = 55;
    int hitboxOffsetH = 50;
  
    void idle();
    void move(vector<Enemy> &enemies);
    void attack(vector<Enemy> &enemies);
    void hit(vector<Enemy> &enemies);
    bool attacking();
    void defend();
};
