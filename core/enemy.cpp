#include "enemy.h"
#include "../globals.h"

Uint32 enemyLastFrameTime = 0;

#define global static

namespace Enemy { 
  global data enemy {
    .rect = { .x = 600, .y = 300, .w = 192, .h = 192 },
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
  }; 
  global int hitboxOffsetW = 55;
  global int hitboxOffsetH = 50;

  data& init() {
    return enemy;
  }

  void remove() {
    // this probably needs to be improved
    // the enemy data is still around
    SDL_DestroyTexture(enemyIdleTexture);
  }

  void idle() {
    Uint32 now = SDL_GetTicks();

    if (now - enemyLastFrameTime >= frameDuration) {
      enemyLastFrameTime = now;
      enemy.idleAnimationCounter = (enemy.idleAnimationCounter + 1) % enemy.idleSprites;
    }

    enemy.subRect.x = 192 * enemy.idleAnimationCounter;
    enemy.subRect.y = 0;
    enemy.subRect.w = 192;
    enemy.subRect.h = 192;
    enemy.hitbox.x = enemy.rect.x + hitboxOffsetW;
    enemy.hitbox.y = enemy.rect.y + hitboxOffsetH;

    SDL_RenderTextureRotated(renderer, enemyIdleTexture, &enemy.subRect, &enemy.rect, 0.0, NULL, enemy.sdl_flip);
  }
}
