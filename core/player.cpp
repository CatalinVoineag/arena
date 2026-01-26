#include "player.h"
#include "../globals.h"
#include "enemy.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <cstdio>

#define global static

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
  speed = 500;
  sdl_flip = SDL_FLIP_NONE;
  midAnimation = false;
  lastFrameTime = 0;
  hitboxOffsetW = 55;
  hitboxOffsetH = 50;
  id = reinterpret_cast<uintptr_t>(this);
}

void Player::idle() {
  uint64_t now = SDL_GetTicks();

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

  SDL_RenderTextureRotated(renderer, idleTexture, &subRect, &rect, 0.0, NULL, sdl_flip);
}
bool pressed(int keycode) {
  return input->keycodes[keycode] == true;
}

typedef struct colisionStruct {
  float overlapX;
  float overlapY;
  float distanceX;
  float distanceY;
} colisionStruct;

colisionStruct colision(SDL_FRect &rect, SDL_FRect &obj) {
  float centerPlayerX = rect.x + rect.w / 2;
  float centerPlayerY = rect.y + rect.h / 2;

  float centerEnemyX = obj.x + obj.w / 2;
  float centerEnemyY = obj.y + obj.h / 2;

  float distanceX = centerPlayerX - centerEnemyX;
  float distanceY = centerPlayerY - centerEnemyY;

  float combinedHalfWidth = rect.w / 2 + obj.w / 2;
  float combinedHalfHeight = rect.h / 2 + obj.h / 2;

  float overlapX = combinedHalfWidth - abs(distanceX);
  float overlapY = combinedHalfHeight - abs(distanceY);

  return { 
    .overlapX = overlapX,
    .overlapY = overlapY,
    .distanceX = distanceX,
    .distanceY = distanceY
  };
}

typedef struct movementCollision {
  bool canMove;
  float pushBackX;
  float pushBackY;
} movementCollision;

movementCollision checkMovementCollision(SDL_FRect testBox, vector<Enemy> &enemies) {
  movementCollision result = { .canMove = true, .pushBackX = 0, .pushBackY = 0 };
  
  for (auto &enemy : enemies) {
    colisionStruct colisionObj = colision(testBox, enemy.entityBox);
    
    if (colisionObj.overlapX > 0 && colisionObj.overlapY > 0) {
      result.canMove = false;
      
      // Calculate pushback for each axis
      if (abs(colisionObj.distanceX) > abs(colisionObj.distanceY)) {
        // Horizontal collision is dominant
        if (colisionObj.distanceX > 0) {
          result.pushBackX = max(result.pushBackX, colisionObj.overlapX);
        } else {
          result.pushBackX = min(result.pushBackX, -colisionObj.overlapX);
        }
      } else {
        // Vertical collision is dominant
        if (colisionObj.distanceY > 0) {
          result.pushBackY = max(result.pushBackY, colisionObj.overlapY);
        } else {
          result.pushBackY = min(result.pushBackY, -colisionObj.overlapY);
        }
      }
    }
  }
  
  return result;
}

void Player::move(Map &gameMap, float deltaTime, vector<Enemy> &enemies) {
  Uint32 now = SDL_GetTicks();

  if (deltaTime > MAX_DT) {
    deltaTime = MAX_DT;
  }

  if (pressed(SDLK_A)) {
    float moveDistance = speed * deltaTime;
    int xIndex = (entityBox.x - moveDistance + entityBox.w / 2) / 64;
    int yIndex = (entityBox.y + entityBox.h / 2) / 64;
    int key = yIndex * gameMap.mapArray[0].size() + xIndex; 

    if (!gameMap.mapNodes[key].obstacle || key == mapNodeIndex) {
      SDL_FRect testBox = {
        .x = entityBox.x - moveDistance,
        .y = entityBox.y,
        .w = entityBox.w,
        .h = entityBox.h
      };
      
      movementCollision collision = checkMovementCollision(testBox, enemies);
      
      if (collision.canMove) {
        rect.x -= moveDistance;
      } else {
        // Apply pushback to move player out of collision
        rect.x += collision.pushBackX;
      }
    }
    sdl_flip = SDL_FLIP_HORIZONTAL;
  }
  if (pressed(SDLK_D)) {
    float moveDistance = speed * deltaTime;
    int xIndex = (entityBox.x + moveDistance + entityBox.w / 2) / 64;
    int yIndex = (entityBox.y + entityBox.h / 2) / 64;
    int key = yIndex * gameMap.mapArray[0].size() + xIndex; 

    if (!gameMap.mapNodes[key].obstacle || key == mapNodeIndex) {
      SDL_FRect testBox = {
        .x = entityBox.x + moveDistance,
        .y = entityBox.y,
        .w = entityBox.w,
        .h = entityBox.h
      };
      
      movementCollision collision = checkMovementCollision(testBox, enemies);
      
      if (collision.canMove) {
        rect.x += moveDistance;
      } else {
        // Apply pushback to move player out of collision
        rect.x += collision.pushBackX;
      }
    }
    sdl_flip = SDL_FLIP_NONE;
  }
  if (pressed(SDLK_W)) {
    float moveDistance = speed * deltaTime;
    int xIndex = (entityBox.x + entityBox.w / 2) / 64;
    int yIndex = (entityBox.y - moveDistance + entityBox.h / 2) / 64;
    int key = yIndex * gameMap.mapArray[0].size() + xIndex; 

    if (!gameMap.mapNodes[key].obstacle || key == mapNodeIndex) {
      SDL_FRect testBox = {
        .x = entityBox.x,
        .y = entityBox.y - moveDistance,
        .w = entityBox.w,
        .h = entityBox.h
      };
      
      movementCollision collision = checkMovementCollision(testBox, enemies);
      
      if (collision.canMove) {
        rect.y -= moveDistance;
      } else {
        // Apply pushback to move player out of collision
        rect.y += collision.pushBackY;
      }
    }
  }

  if (pressed(SDLK_S)) {
    float moveDistance = speed * deltaTime;
    int xIndex = (entityBox.x + entityBox.w / 2) / 64;
    int yIndex = (entityBox.y + moveDistance + entityBox.h / 2) / 64;
    int key = yIndex * gameMap.mapArray[0].size() + xIndex; 

    if (!gameMap.mapNodes[key].obstacle || key == mapNodeIndex) {
      SDL_FRect testBox = {
        .x = entityBox.x,
        .y = entityBox.y + moveDistance,
        .w = entityBox.w,
        .h = entityBox.h
      };
      
      movementCollision collision = checkMovementCollision(testBox, enemies);
      
      if (collision.canMove) {
        rect.y += moveDistance;
      } else {
        // Apply pushback to move player out of collision
        rect.y += collision.pushBackY;
      }
    }
  }

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

void Player::update(vector<Enemy> &enemies, Map &gameMap, float deltaTime) {
  int xIndex = (entityBox.x + entityBox.w / 2) / 64;
  int yIndex = (entityBox.y + entityBox.h / 2) / 64;
  mapNodeIndex = yIndex * gameMap.mapArray[0].size() + xIndex; 

  if (attacking()) {
    attack(enemies);
  }
  else if (defending()) {
    defend();
  } else if (moving()) {
    move(gameMap, deltaTime, enemies);
  } else {
    idle();
  }
}
