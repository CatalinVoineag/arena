#include "enemy.h"
#include "player.h"
#include "map.h"
#include "pathing.h"
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
  hitboxOffsetH = 50;
  hitboxOffsetW = 55;
  entityBox = {
    .x = rect.x + hitboxOffsetW,
    .y = rect.y + hitboxOffsetH,
    .w = 90, .h = 90
  };
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
  speed = 65;
  sdl_flip = SDL_FLIP_NONE;
  hitDuration = 1000;
  state = TRACKING;
  idleTexture = idle_texture;
  moveTexture = move_texture;
  attackTexture = attack_texture;
  enemyLastFrameTime = 0;
  hitLastFrameTime = 0;
  SDL_GetTextureColorMod(idleTexture, &red, &green, &blue);
  id = reinterpret_cast<uintptr_t>(this);

  obj_coordinates[id] = { entityBox };
}

Enemy::~Enemy() {
  rect = { .x = 0, .y = 0, .w = 0, .h = 0 };  
  entityBox = { .x = 0, .y = 0, .w = 0, .h = 0 };  
};

void Enemy::update(Player *player, Map &gameMap) {
  int xIndex = (entityBox.x + entityBox.w / 2) / 64;
  int yIndex = (entityBox.y + entityBox.h / 2) / 64;
  mapNodeIndex = yIndex * gameMap.mapArray[0].size() + xIndex; 

  Pathing pathing = Pathing();
  vector<MapNode*> nodes = pathing.solveAStar(gameMap, mapNodeIndex, player->mapNodeIndex);

  if (nodes.size() > 1) {
    // state = TRACKING;
    state = IDLE;
  } else {
    state = IDLE;
  }

  if (state == HIT) {
    // set the correct color to the correct texture based on state?
    SDL_SetTextureColorMod(idleTexture, 204, 51, 51);
    SDL_SetTextureColorMod(moveTexture, 204, 51, 51);
    SDL_SetTextureColorMod(attackTexture, 204, 51, 51);
  }

  if(state == IDLE) {
    idle();
  } else {
    // if (SDL_HasRectIntersectionFloat(&hitbox, &player->hitbox)) {
    //   attack(player);
    // } else {
    trackPlayer(player, gameMap, nodes);
    // }
  }

  if (state == HIT) {
    Uint32 now = SDL_GetTicks(); 
    if (hitLastFrameTime == 0) {
      hitLastFrameTime = now; 
    }
    if (now - hitLastFrameTime >= hitDuration) {
      hitLastFrameTime = now;
      SDL_SetTextureColorMod(idleTexture, red, green, blue);
      state = TRACKING;
      hitLastFrameTime = 0;
    }
  }

  if (health <= 0) {
    Enemy::~Enemy();
  }

  SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);

  for (size_t i = 1; i < nodes.size(); ++i) {
      MapNode* from = nodes[i-1];
      MapNode* to   = nodes[i];
      
      SDL_RenderLine(
          renderer,
          from->rect.x + 32,
          from->rect.y + 32,
          to->rect.x + 32,
          to->rect.y + 32
      );
  }

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
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
  entityBox.x = rect.x + hitboxOffsetW;
  entityBox.y = rect.y + hitboxOffsetH;
  obj_coordinates[id] = { entityBox };

  // figure out how to maintain this for repeated attacks
  // how will this work with multiple actions, move/ attack/ block. etc.
  SDL_RenderTextureRotated(renderer, idleTexture, &subRect, &rect, 0.0, NULL, sdl_flip);
  SDL_SetTextureColorMod(idleTexture, 255, 255, 255);
}

void Enemy::trackPlayer(Player *player, Map &gameMap, vector<MapNode*> nodes) {
  Uint32 now = SDL_GetTicks();
  uint64_t nowPerformance = SDL_GetPerformanceCounter();

  if (now - enemyLastFrameTime >= frameDuration) {
    enemyLastFrameTime = now;
    moveAnimationCounter = (moveAnimationCounter + 1) % moveSprites;
  }

  // We can maybe make a function that sets all these to true or false
  // much like SDL functions do
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

  float deltaTime = (nowPerformance - lastCounter) / 1000.0f; 

  if (deltaTime > MAX_DT) {
    deltaTime = MAX_DT;
  }

  if (nodes.size() > 1) {
    int index = nodes.size() - 2;
    int nodeX = nodes[index]->rect.x;
    int nodeY = nodes[index]->rect.y;
    if (nodeX + 5 < entityBox.x) {
      if (!leftColision) { rect.x -= speed * deltaTime; }
      sdl_flip = SDL_FLIP_HORIZONTAL;
    }
    if (nodeX + 5 > entityBox.x) {
      if (!rightColision) { rect.x += speed * deltaTime; }
      sdl_flip = SDL_FLIP_NONE;
    }
    if (nodeY < entityBox.y) {
      if (!upColision) { rect.y -= speed * deltaTime; }
    }
    if (nodeY > entityBox.y) {
      if (!downColision) { rect.y += speed * deltaTime; }
    }
  }

  subRect.x = 192 * moveAnimationCounter;
  subRect.y = 0;
  subRect.w = 192;
  subRect.h = 192;
  entityBox.x = rect.x + hitboxOffsetW;
  entityBox.y = rect.y + hitboxOffsetH;
  obj_coordinates[id] = { entityBox };

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
    entityBox.x = rect.x + hitboxOffsetW;
    entityBox.y = rect.y + hitboxOffsetH;

    // figure out how to maintain this for repeated attacks
    // how will this work with multiple actions, move/ attack/ block. etc.
    SDL_RenderTextureRotated(renderer, attackTexture, &subRect, &rect, 0.0, NULL, sdl_flip);
    SDL_SetTextureColorMod(attackTexture, 255, 255, 255);
  }

void Enemy::hit(int damage) {
  state = HIT;
  health -= damage;
}
