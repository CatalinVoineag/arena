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
  speed = 200;
  sdl_flip = SDL_FLIP_NONE;
  hitDuration = 1000;
  state = TRACKING;
  idleTexture = idle_texture;
  moveTexture = move_texture;
  attackTexture = attack_texture;
  enemyLastFrameTime = 0;
  hitLastFrameTime = 0;
  SDL_GetTextureColorMod(idleTexture, &red, &green, &blue);
}

Enemy::~Enemy() {
  rect = { .x = 0, .y = 0, .w = 0, .h = 0 };  
  entityBox = { .x = 0, .y = 0, .w = 0, .h = 0 };  
};

void Enemy::update(Player *player, Map &gameMap, float deltaTime) {
  int xIndex = (entityBox.x + entityBox.w / 2) / 64;
  int yIndex = (entityBox.y + entityBox.h / 2) / 64;
  int oldMapIndex = mapNodeIndex;
  mapNodeIndex = yIndex * gameMap.mapArray[0].size() + xIndex; 

  // Collisions will need to be using boundary boxes. For buildings we can use the obstacle but for characters no.
  // We can have characters bigger than a tile.

  if (oldMapIndex != mapNodeIndex) {
    gameMap.mapNodes[oldMapIndex].obstacle = false;
    gameMap.mapNodes[oldMapIndex].objOnTop = nullptr;
  }

  if (gameMap.mapNodes[mapNodeIndex].obstacle == false) {
    gameMap.mapNodes[mapNodeIndex].obstacle = true;
  }
  if(!gameMap.mapNodes[mapNodeIndex].objOnTop) {
    gameMap.mapNodes[mapNodeIndex].objOnTop = this;
  }

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
    trackPlayer(player, gameMap, nodes, deltaTime);
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

  // figure out how to maintain this for repeated attacks
  // how will this work with multiple actions, move/ attack/ block. etc.
  SDL_RenderTextureRotated(renderer, idleTexture, &subRect, &rect, 0.0, NULL, sdl_flip);
  SDL_SetTextureColorMod(idleTexture, 255, 255, 255);
}

void Enemy::trackPlayer(Player *player, Map &gameMap, vector<MapNode*> nodes, float deltaTime) {
  Uint32 now = SDL_GetTicks();

  if (now - enemyLastFrameTime >= frameDuration) {
    enemyLastFrameTime = now;
    moveAnimationCounter = (moveAnimationCounter + 1) % moveSprites;
  }

  if (deltaTime > MAX_DT) {
    deltaTime = MAX_DT;
  }

  if (nodes.size() > 1) {
    int index = nodes.size() - 2;
    int nodeX = nodes[index]->rect.x;
    int nodeY = nodes[index]->rect.y;

    if (nodeX + 5 < entityBox.x) {
      float moveDistance = speed * deltaTime;
      int xIndex = (entityBox.x - moveDistance + entityBox.w / 2) / 64;
      int yIndex = (entityBox.y + entityBox.h / 2) / 64;
      int key = yIndex * gameMap.mapArray[0].size() + xIndex; 

      if (!gameMap.mapNodes[key].obstacle || gameMap.mapNodes[key].objOnTop == this) {
        rect.x -= moveDistance;
      }
        sdl_flip = SDL_FLIP_HORIZONTAL;
    }
    if (nodeX + 5 > entityBox.x) {
      float moveDistance = speed * deltaTime;
      int xIndex = (entityBox.x + moveDistance + entityBox.w / 2) / 64;
      int yIndex = (entityBox.y + entityBox.h / 2) / 64;
      int key = yIndex * gameMap.mapArray[0].size() + xIndex; 

      if (!gameMap.mapNodes[key].obstacle || gameMap.mapNodes[key].objOnTop == this) {
        rect.x += moveDistance;
      }
      sdl_flip = SDL_FLIP_NONE;
    }
    if (nodeY < entityBox.y) {
      float moveDistance = speed * deltaTime;
      int xIndex = (entityBox.x + entityBox.w / 2) / 64;
      int yIndex = (entityBox.y - moveDistance + entityBox.h / 2) / 64;
      int key = yIndex * gameMap.mapArray[0].size() + xIndex; 

      if (!gameMap.mapNodes[key].obstacle || gameMap.mapNodes[key].objOnTop == this) {
        rect.y -= moveDistance;
      }
    }
    if (nodeY > entityBox.y) {
      float moveDistance = speed * deltaTime;
      int xIndex = (entityBox.x + entityBox.w / 2) / 64;
      int yIndex = (entityBox.y + moveDistance + entityBox.h / 2) / 64;
      int key = yIndex * gameMap.mapArray[0].size() + xIndex; 

      if (!gameMap.mapNodes[key].obstacle || gameMap.mapNodes[key].objOnTop == this) {
        rect.y += moveDistance;
      }
    }
  }

  subRect.x = 192 * moveAnimationCounter;
  subRect.y = 0;
  subRect.w = 192;
  subRect.h = 192;
  entityBox.x = rect.x + hitboxOffsetW;
  entityBox.y = rect.y + hitboxOffsetH;

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
