#include "player.h"
#include "../globals.h"
#include "enemy.h"
#include <SDL3/SDL_render.h>
#include <cstdio>

#define global static

Player::Player() {
  rect = { .x = 0, .y = 0, .w = 192, .h = 192 };
  subRect = {};
  hitbox = { .w = 90, .h = 90 };
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
  midAnimation = false;
  Uint32 lastFrameTime = 0;
  int hitboxOffsetW = 55;
  int hitboxOffsetH = 50;
}

  void Player::idle() {
    Uint32 now = SDL_GetTicks();

    if (now - lastFrameTime >= frameDuration) {
      lastFrameTime = now;
      idleAnimationCounter = (idleAnimationCounter + 1) % idleSprites;
    }

    subRect.x = 192 * idleAnimationCounter;
    subRect.y = 0;
    subRect.w = 192;
    subRect.h = 192;
    hitbox.x = rect.x + hitboxOffsetW;
    hitbox.y = rect.y + hitboxOffsetH;

    SDL_RenderTextureRotated(renderer, idleTexture, &subRect, &rect, 0.0, NULL, sdl_flip);
  }

  bool pressed(int keycode) {
    return input->keycodes[keycode] == true;
  }

  void Player::move() {
    Uint32 now = SDL_GetTicks();
    if (pressed(SDLK_A)) {
      rect.x -= speed;
      sdl_flip = SDL_FLIP_HORIZONTAL;
    }
    if (pressed(SDLK_D)) {
      rect.x += speed;
      sdl_flip = SDL_FLIP_NONE;
    }
    if (pressed(SDLK_W)) {
      rect.y -= speed;
    }
    if (pressed(SDLK_S)) {
      rect.y += speed;
    }

    if (now - lastFrameTime >= frameDuration) {
      lastFrameTime = now;
      moveAnimationCounter = (moveAnimationCounter + 1) % moveSprites;
    }

    subRect.x = 192 * moveAnimationCounter;
    subRect.y = 0;
    subRect.w = 192;
    subRect.h = 192;
    hitbox.x = rect.x + hitboxOffsetW;
    hitbox.y = rect.y + hitboxOffsetH;

    SDL_RenderTextureRotated(renderer, moveTexture, &subRect, &rect, 0.0, NULL, sdl_flip);
  }

  void Player::defend() {
    Uint32 now = SDL_GetTicks(); 
    if (now - lastFrameTime >= frameDuration) {
      lastFrameTime = now;
      defendAnimationCounter = (defendAnimationCounter + 1) % defendSprites;
    }

    subRect.x = 192 * defendAnimationCounter;
    subRect.y = 0;
    subRect.w = 192;
    subRect.h = 192;
    hitbox.x = rect.x + hitboxOffsetW;
    hitbox.y = rect.y + hitboxOffsetH;

    SDL_RenderTextureRotated(renderer, defendTexture, &subRect, &rect, 0.0, NULL, sdl_flip);
  }

  void Player::attack() {
    Uint32 now = SDL_GetTicks(); 
    midAnimation = true;
    if (now - lastFrameTime >= frameDuration) {
      lastFrameTime = now;
      attackAnimationCounter = (attackAnimationCounter + 1) % attackSprites;
    }

    subRect.x = 192 * attackAnimationCounter;
    subRect.y = 0;
    subRect.w = 192;
    subRect.h = 192;
    hitbox.x = rect.x + hitboxOffsetW;
    hitbox.y = rect.y + hitboxOffsetH;

    SDL_RenderTextureRotated(renderer, attackTexture, &subRect, &rect, 0.0, NULL, sdl_flip);
    if (attackAnimationCounter == attackSprites - 1) {
      attackAnimationCounter = 0;
      midAnimation = false;
    }
  }

  bool moving() {
    return input->keycodes[SDLK_A] ||
      input->keycodes[SDLK_D] ||
      input->keycodes[SDLK_W] ||
      input->keycodes[SDLK_S];
  };

  bool Player::attacking() {
    return input->mousecodes[SDL_BUTTON_LEFT] || midAnimation;
  }

  void Player::hit(vector<Enemy*> enemies) {
    for (int i = 0; i < enemies.size(); i++) {
      bool contact = SDL_HasRectIntersectionFloat(&hitbox, &enemies[i]->hitbox);
      if (contact && !midAnimation) {
        // Enemy::remove();
        enemies[i]->hit();
      } 
    }
  }

  bool defending() {
    return input->mousecodes[SDL_BUTTON_RIGHT];
  }

  void Player::update(vector<Enemy*> enemies) {
    if (attacking()) {
      attack();
      hit(enemies);
    }
    else if (defending()) {
      defend();
    } else if (moving()) {
      move();
    } else {
      idle();
    }
  }
