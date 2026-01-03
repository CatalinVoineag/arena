#include "enemy.h"
#include "../globals.h"
#include <SDL3/SDL_render.h>
#include <cstdio>

Enemy::Enemy(SDL_Texture *sdl_texture, float x, float y) {
  rect = { .x = x, .y = y, .w = 192, .h = 192 };
  subRect = {};
  hitbox = { .w = 90, .h = 90};
  idleAnimationCounter = 0;
  idleSprites = 8;
  attackAnimationCounter = 0;
  attackSprites = 4;
  defendAnimationCounter = 0;
  defendSprites = 6;
  moveAnimationCounter = 0;
  moveSprites = 6;
  speed = 5;
  sdl_flip = SDL_FLIP_NONE;
  hitboxOffsetW = 55;
  hitboxOffsetH = 50;
  hitDuration = 1000;
  state = NORMAL;
  texture = sdl_texture;
  enemyLastFrameTime = 0;
  hitLastFrameTime = 0;
  SDL_GetTextureColorMod(texture, &red, &green, &blue);
}

void Enemy::update() {
  if (state == HIT) {
    Uint32 now = SDL_GetTicks(); 
    if (now - hitLastFrameTime >= hitDuration) {
      hitLastFrameTime = now;
      SDL_SetTextureColorMod(texture, red, green, blue);
      state = NORMAL;
    }
  }

  idle();
};

void Enemy::idle() {
  Uint32 now = SDL_GetTicks();

  if (now - enemyLastFrameTime >= frameDuration) {
    enemyLastFrameTime = now;
    idleAnimationCounter = (idleAnimationCounter + 1) % idleSprites;
  }

  subRect.x = 192 * idleAnimationCounter;
  subRect.y = 0;
  subRect.w = 192;
  subRect.h = 192;
  hitbox.x = rect.x + hitboxOffsetW;
  hitbox.y = rect.y + hitboxOffsetH;

  // figure out how to maintain this for repeated attacks
  // how will this work with multiple actions, move/ attack/ block. etc.
  if (state == HIT) {
    SDL_SetTextureColorMod(texture, 204, 51, 51);
  }
  SDL_RenderTextureRotated(renderer, texture, &subRect, &rect, 0.0, NULL, sdl_flip);

  SDL_SetTextureColorMod(texture, 255, 255, 255);
}

void Enemy::hit() {
  state = HIT;
}
