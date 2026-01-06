#include "player.h"
#include "../globals.h"
#include "enemy.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <cstdio>

#define global static
map<uintptr_t, SDL_FRect> obj_coordinates;

Player::Player() {
  rect = { .x = 0, .y = 0, .w = 192, .h = 192 };
  subRect = {};
  entityBox = {
    .x = rect.x + hitboxOffsetW,
    .y = rect.y + hitboxOffsetH,
    .w = 90, .h = 90
  };
  health = 20;
  damage = 2;
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
  lastFrameTime = 0;
  hitboxOffsetW = 55;
  hitboxOffsetH = 50;
  id = reinterpret_cast<uintptr_t>(this);

  obj_coordinates[id] = { entityBox };
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
    entityBox.x = rect.x + hitboxOffsetW;
    entityBox.y = rect.y + hitboxOffsetH;
    obj_coordinates[id] = { entityBox };

    SDL_RenderTextureRotated(renderer, idleTexture, &subRect, &rect, 0.0, NULL, sdl_flip);
  }
  bool pressed(int keycode) {
    return input->keycodes[keycode] == true;
  }

  void Player::move() {
    Uint32 now = SDL_GetTicks();
    bool rightColision = false;
    bool leftColision = false;
    bool upColision = false;
    bool downColision = false;

    for (auto objRect : obj_coordinates) {
      if(objRect.first == id) { continue; }
      bool colision = SDL_HasRectIntersectionFloat(&entityBox, &objRect.second); 
      if (colision) {
        float entityBoxLeft = entityBox.x;
        float entityBoxRight = entityBox.x + entityBox.w;
        float entityBoxTop = entityBox.y;
        float entityBoxBottom = entityBox.y + entityBox.h;

        float objLeft = objRect.second.x;
        float objRight = objRect.second.x + objRect.second.w;
        float objTop = objRect.second.y;
        float objBottom = objRect.second.y + objRect.second.h;

        float overlapX = min(entityBoxRight, objRight) - max(entityBoxLeft, objLeft);
        float overlapY = min(entityBoxBottom, objBottom) - max(entityBoxTop, objTop);

        if (overlapX < overlapY) {
          if (entityBox.x < objRect.second.x) {
            rightColision = true;
          } else {
            leftColision = true;
          }
        } else {
          if (entityBox.y < objRect.second.y) {
            downColision = true;
          } else {
            upColision = true;
          }
        }
      }
    }

    if (pressed(SDLK_A)) {
      if (!leftColision) { rect.x -= speed; }
      sdl_flip = SDL_FLIP_HORIZONTAL;
    }
    if (pressed(SDLK_D)) {
      if (!rightColision) { rect.x += speed; }
      sdl_flip = SDL_FLIP_NONE;
    }
    if (pressed(SDLK_W) && !upColision) { rect.y -= speed; }
    if (pressed(SDLK_S) && !downColision) { rect.y += speed; }

    if (now - lastFrameTime >= frameDuration) {
      lastFrameTime = now;
      moveAnimationCounter = (moveAnimationCounter + 1) % moveSprites;
    }

    subRect.x = 192 * moveAnimationCounter;
    subRect.y = 0;
    subRect.w = 192;
    subRect.h = 192;
    entityBox.x = rect.x + hitboxOffsetW;
    entityBox.y = rect.y + hitboxOffsetH;
    obj_coordinates[id] = { entityBox };

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
    entityBox.x = rect.x + hitboxOffsetW;
    entityBox.y = rect.y + hitboxOffsetH;
    obj_coordinates[id] = { entityBox };

    SDL_RenderTextureRotated(renderer, defendTexture, &subRect, &rect, 0.0, NULL, sdl_flip);
  }

  void Player::attack(vector<Enemy> &enemies) {
    for (int i = 0; i < enemies.size(); i++) {
      bool contact = SDL_HasRectIntersectionFloat(&entityBox, &enemies[i].entityBox);
      if (contact && !midAnimation) { 
        enemies[i].hit(damage);
      }
    }

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
    entityBox.x = rect.x + hitboxOffsetW;
    entityBox.y = rect.y + hitboxOffsetH;
    obj_coordinates[id] = { entityBox };

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
    // return input->mousecodes[SDL_BUTTON_LEFT] || midAnimation;
    return false;
  }

  bool defending() {
    return input->mousecodes[SDL_BUTTON_RIGHT].down;
  }

  void Player::update(vector<Enemy> &enemies) {
    if (attacking()) {
      attack(enemies);
    }
    else if (defending()) {
      defend();
    } else if (moving()) {
      move();
    } else {
      idle();
    }
  }
