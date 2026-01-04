#include "enemy.h"
#include "player.h"
#include "../globals.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <cstdio>

Enemy::Enemy(
  SDL_Texture *idle_texture,
  SDL_Texture *move_texture,
  SDL_Texture *attack_texture,
  float x,
  float y
) {
  rect = { .x = x, .y = y, .w = 192, .h = 192 };
  subRect = {};
  hitbox = { .w = 90, .h = 90};
  health = 10;
  damage = 5;
  idleAnimationCounter = 0;
  idleSprites = 8;
  attackAnimationCounter = 0;
  attackSprites = 4;
  defendAnimationCounter = 0;
  defendSprites = 6;
  moveAnimationCounter = 0;
  moveSprites = 6;
  speed = 2;
  sdl_flip = SDL_FLIP_NONE;
  hitboxOffsetW = 55;
  hitboxOffsetH = 50;
  hitDuration = 1000;
  state = NORMAL;
  idleTexture = idle_texture;
  moveTexture = move_texture;
  attackTexture = attack_texture;
  enemyLastFrameTime = 0;
  hitLastFrameTime = 0;
  SDL_GetTextureColorMod(idleTexture, &red, &green, &blue);
}

Enemy::~Enemy() {
  rect = { .x = 0, .y = 0, .w = 0, .h = 0 };  
  hitbox = { .x = 0, .y = 0, .w = 0, .h = 0 };  
};

void Enemy::update(Player *player) {
  if (state == HIT) {
    // set the correct color to the correct texture based on state?
    SDL_SetTextureColorMod(idleTexture, 204, 51, 51);
    SDL_SetTextureColorMod(moveTexture, 204, 51, 51);
    SDL_SetTextureColorMod(attackTexture, 204, 51, 51);
  }

  // if (SDL_HasRectIntersectionFloat(&hitbox, &player->hitbox)) {
  //   attack(player);
  // } else {
    // trackPlayer(player);
  // }

  idle();

  if (state == HIT) {
    Uint32 now = SDL_GetTicks(); 
    if (hitLastFrameTime == 0) {
      hitLastFrameTime = now; 
    }
    if (now - hitLastFrameTime >= hitDuration) {
      hitLastFrameTime = now;
      SDL_SetTextureColorMod(idleTexture, red, green, blue);
      state = NORMAL;
      hitLastFrameTime = 0;
    }
  }

  if (health <= 0) {
    Enemy::~Enemy();
  }
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
  SDL_RenderTextureRotated(renderer, idleTexture, &subRect, &rect, 0.0, NULL, sdl_flip);
  SDL_SetTextureColorMod(idleTexture, 255, 255, 255);
}

void Enemy::trackPlayer(Player *player) {
  Uint32 now = SDL_GetTicks();

  if (now - enemyLastFrameTime >= frameDuration) {
    enemyLastFrameTime = now;
    moveAnimationCounter = (moveAnimationCounter + 1) % moveSprites;
  }

  int playerX = player->rect.x;
  int playerY = player->rect.y;
  if (playerX > rect.x) {
    rect.x += speed;
  }
  if (playerY > rect.y) {
    rect.y += speed;
  }
  if (playerX < rect.x) {
    rect.x -= speed;
  }
  if (playerY < rect.y) {
    rect.y -= speed;
  }

  subRect.x = 192 * moveAnimationCounter;
  subRect.y = 0;
  subRect.w = 192;
  subRect.h = 192;
  hitbox.x = rect.x + hitboxOffsetW;
  hitbox.y = rect.y + hitboxOffsetH;

  // figure out how to maintain this for repeated attacks
  // how will this work with multiple actions, move/ attack/ block. etc.
  SDL_RenderTextureRotated(renderer, moveTexture, &subRect, &rect, 0.0, NULL, sdl_flip);
  SDL_SetTextureColorMod(moveTexture, 255, 255, 255);
}

  void Enemy::attack(Player *player) {
    // bool contact = SDL_HasRectIntersectionFloat(&hitbox, &enemies[i]->hitbox);
    // if (contact && !midAnimation) { 
    //   enemies[i]->hit(damage);
    // }

    Uint32 now = SDL_GetTicks();

    if (now - enemyLastFrameTime >= frameDuration) {
      enemyLastFrameTime = now;
      attackAnimationCounter = (attackAnimationCounter + 1) % attackSprites;
    }

    subRect.x = 192 * attackAnimationCounter;
    subRect.y = 0;
    subRect.w = 192;
    subRect.h = 192;
    hitbox.x = rect.x + hitboxOffsetW;
    hitbox.y = rect.y + hitboxOffsetH;

    // figure out how to maintain this for repeated attacks
    // how will this work with multiple actions, move/ attack/ block. etc.
    SDL_RenderTextureRotated(renderer, attackTexture, &subRect, &rect, 0.0, NULL, sdl_flip);
    SDL_SetTextureColorMod(attackTexture, 255, 255, 255);
  }

void Enemy::hit(int damage) {
  state = HIT;
  health -= damage;
}
