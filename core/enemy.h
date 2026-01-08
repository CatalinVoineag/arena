#pragma once
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
class Player;

typedef enum enemyState {
  NORMAL,
  HIT,
} enemyState;

class Map;

class Enemy {
  public:
  uintptr_t id;
  SDL_FRect rect;
  SDL_FRect subRect;
  SDL_FRect entityBox;
  enemyState state;
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
  SDL_Texture* idleTexture;
  SDL_Texture* moveTexture;
  SDL_Texture* attackTexture;

  Enemy(
    SDL_Texture* idle_texture,
    SDL_Texture* move_texture,
    SDL_Texture* attac_texture,
    float x,
    float y
  );
  ~Enemy();
  void update(Player* player, Map &gameMap);
  void hit(int);
  void unhit();

  private:
  int hitboxOffsetW;
  int hitboxOffsetH;
  int hitDuration;
  Uint32 enemyLastFrameTime;
  Uint32 hitLastFrameTime;
  Uint8 red;
  Uint8 green;
  Uint8 blue;

  void idle();
  void trackPlayer(Player* player, Map &gameMap);
  void attack(Player* player);
};
