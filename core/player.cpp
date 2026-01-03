#include "player.h"
#include "../globals.h"
#include "enemy.h"
#include <cstdio>

#define global static

namespace Player {
  global data player = {
    .rect = { .x = 0, .y = 0, .w = 192, .h = 192 },
      .subRect = {},
      .hitbox = { .w = 90, .h = 90},
      .idleAnimationCounter = 0,
      .idleSprites = 8,
      .attackAnimationCounter = 0,
      .attackSprites = 4,
      .defendAnimationCounter = 0,
      .defendSprites = 6,
      .moveAnimationCounter = 0,
      .moveSprites = 6,
      .speed = 5,
      .sdl_flip = SDL_FLIP_NONE,
      .midAnimation = false,
  };
  global Uint32 lastFrameTime = 0;
  global int hitboxOffsetW = 55;
  global int hitboxOffsetH = 50;
  global Enemy::data& enemy = Enemy::init();

  data& init() {
    return player;
  }

  void idle() {
    Uint32 now = SDL_GetTicks();

    if (now - lastFrameTime >= frameDuration) {
      lastFrameTime = now;
      player.idleAnimationCounter = (player.idleAnimationCounter + 1) % player.idleSprites;
    }

    player.subRect.x = 192 * player.idleAnimationCounter;
    player.subRect.y = 0;
    player.subRect.w = 192;
    player.subRect.h = 192;
    player.hitbox.x = player.rect.x + hitboxOffsetW;
    player.hitbox.y = player.rect.y + hitboxOffsetH;

    SDL_RenderTextureRotated(renderer, idleTexture, &player.subRect, &player.rect, 0.0, NULL, player.sdl_flip);
  }

  bool pressed(int keycode) {
    return input->keycodes[keycode] == true;
  }

  void move() {
    Uint32 now = SDL_GetTicks();
    if (pressed(SDLK_A)) {
      player.rect.x -= player.speed;
      player.sdl_flip = SDL_FLIP_HORIZONTAL;
    }
    if (pressed(SDLK_D)) {
      player.rect.x += player.speed;
      player.sdl_flip = SDL_FLIP_NONE;
    }
    if (pressed(SDLK_W)) {
      player.rect.y -= player.speed;
    }
    if (pressed(SDLK_S)) {
      player.rect.y += player.speed;
    }

    if (now - lastFrameTime >= frameDuration) {
      lastFrameTime = now;
      player.moveAnimationCounter = (player.moveAnimationCounter + 1) % player.moveSprites;
    }

    player.subRect.x = 192 * player.moveAnimationCounter;
    player.subRect.y = 0;
    player.subRect.w = 192;
    player.subRect.h = 192;
    player.hitbox.x = player.rect.x + hitboxOffsetW;
    player.hitbox.y = player.rect.y + hitboxOffsetH;

    SDL_RenderTextureRotated(renderer, moveTexture, &player.subRect, &player.rect, 0.0, NULL, player.sdl_flip);
  }

  void defend() {
    Uint32 now = SDL_GetTicks(); 
    if (now - lastFrameTime >= frameDuration) {
      lastFrameTime = now;
      player.defendAnimationCounter = (player.defendAnimationCounter + 1) % player.defendSprites;
    }

    player.subRect.x = 192 * player.defendAnimationCounter;
    player.subRect.y = 0;
    player.subRect.w = 192;
    player.subRect.h = 192;
    player.hitbox.x = player.rect.x + hitboxOffsetW;
    player.hitbox.y = player.rect.y + hitboxOffsetH;

    SDL_RenderTextureRotated(renderer, defendTexture, &player.subRect, &player.rect, 0.0, NULL, player.sdl_flip);
  }

  void attack() {
    Uint32 now = SDL_GetTicks(); 
    player.midAnimation = true;
    if (now - lastFrameTime >= frameDuration) {
      lastFrameTime = now;
      player.attackAnimationCounter = (player.attackAnimationCounter + 1) % player.attackSprites;
    }

    player.subRect.x = 192 * player.attackAnimationCounter;
    player.subRect.y = 0;
    player.subRect.w = 192;
    player.subRect.h = 192;
    player.hitbox.x = player.rect.x + hitboxOffsetW;
    player.hitbox.y = player.rect.y + hitboxOffsetH;

    SDL_RenderTextureRotated(renderer, attackTexture, &player.subRect, &player.rect, 0.0, NULL, player.sdl_flip);
    if (player.attackAnimationCounter == player.attackSprites - 1) {
      player.attackAnimationCounter = 0;
      player.midAnimation = false;
    }
  }

  bool moving() {
    return input->keycodes[SDLK_A] ||
      input->keycodes[SDLK_D] ||
      input->keycodes[SDLK_W] ||
      input->keycodes[SDLK_S];
  };

  bool attacking() {
    return input->mousecodes[SDL_BUTTON_LEFT] || player.midAnimation;
  }

  void hit() {
    bool contact = SDL_HasRectIntersectionFloat(&player.hitbox, &enemy.hitbox);
    if (contact && !player.midAnimation) {
      Enemy::remove();
    } 
  }

  bool defending() {
    return input->mousecodes[SDL_BUTTON_RIGHT];
  }

  void update() {
    if (attacking()) {
      attack();
      hit();
    }
    else if (defending()) {
      defend();
    } else if (moving()) {
      move();
    } else {
      idle();
    }
  }
}
