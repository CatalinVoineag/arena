#pragma once
#include "../globals.h"
#include "map.h"
#include "enemy.h"

class Enemy;

class Player {
  public:
    uintptr_t id;
    SDL_FRect rect;
    SDL_FRect subRect;
    SDL_FRect entityBox;
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
    int mapNodeIndex;
    float speed;
    SDL_FlipMode sdl_flip;
    bool midAnimation;

    Player();
    void update(vector<Enemy> &enemies, Map &gameMap, float deltaTime);

  private:
    uint64_t lastFrameTime = 0;
    int hitboxOffsetW = 55;
    int hitboxOffsetH = 50;
  
    void idle();
    void move(Map &gameMap, float deltaTime);
    void attack(vector<Enemy> &enemies);
    void hit(vector<Enemy> &enemies);
    bool attacking();
    void defend();
};
