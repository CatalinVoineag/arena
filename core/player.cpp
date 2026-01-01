#include "player.h"
#include "../globals.h"

#define global static

namespace Player {
  global data player = {
    .rect = { .x = 200, .y = 300, .w = 192, .h = 192 },
      .subRect = {},
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
  global Uint32 lastFrameTime = 0;

  data init() {
    return player;
  }

  void idle() {
    Uint32 now = SDL_GetTicks();

    if (now - lastFrameTime >= frameDuration) {
      lastFrameTime = now;
      player.idleAnimationCounter = (player.idleAnimationCounter + 1) % player.idleSprites;
    }

    player.subRect.x = 192 * player.idleAnimationCounter;
    player.subRect.y = 0;
    player.subRect.w = 192;
    player.subRect.h = 500;

    SDL_RenderTextureRotated(renderer, idleTexture, &player.subRect, &player.rect, 0.0, NULL, player.sdl_flip);
  }

  bool pressed(int keycode) {
    return input->keycodes[keycode] == true;
  }

  void move() {
    Uint32 now = SDL_GetTicks();
    if (pressed(SDLK_A)) {
      player.rect.x -= player.speed;
      player.sdl_flip = SDL_FLIP_HORIZONTAL;
    }
    if (pressed(SDLK_D)) {
      player.rect.x += player.speed;
      player.sdl_flip = SDL_FLIP_NONE;
    }
    if (pressed(SDLK_W)) {
      player.rect.y -= player.speed;
    }
    if (pressed(SDLK_S)) {
      player.rect.y += player.speed;
    }

    if (now - lastFrameTime >= frameDuration) {
      lastFrameTime = now;
      player.moveAnimationCounter = (player.moveAnimationCounter + 1) % player.moveSprites;
    }

    player.subRect.x = 192 * player.moveAnimationCounter;
    player.subRect.y = 0;
    player.subRect.w = 192;
    player.subRect.h = 500;

    SDL_RenderTextureRotated(renderer, moveTexture, &player.subRect, &player.rect, 0.0, NULL, player.sdl_flip);
  }

  void defend() {
    Uint32 now = SDL_GetTicks(); 
    if (now - lastFrameTime >= frameDuration) {
      lastFrameTime = now;
      player.defendAnimationCounter = (player.defendAnimationCounter + 1) % player.defendSprites;
    }

    player.subRect.x = 192 * player.defendAnimationCounter;
    player.subRect.y = 0;
    player.subRect.w = 192;
    player.subRect.h = 500;

    SDL_RenderTextureRotated(renderer, defendTexture, &player.subRect, &player.rect, 0.0, NULL, player.sdl_flip);
  }

  void attack() {
    Uint32 now = SDL_GetTicks(); 
    if (now - lastFrameTime >= frameDuration) {
      lastFrameTime = now;
      player.attackAnimationCounter = (player.attackAnimationCounter + 1) % player.attackSprites;
    }

    player.subRect.x = 192 * player.attackAnimationCounter;
    player.subRect.y = 0;
    player.subRect.w = 192;
    player.subRect.h = 500;

    SDL_RenderTextureRotated(renderer, attackTexture, &player.subRect, &player.rect, 0.0, NULL, player.sdl_flip);
  }

  bool moving() {
    return input->keycodes[SDLK_A] ||
      input->keycodes[SDLK_D] ||
      input->keycodes[SDLK_W] ||
      input->keycodes[SDLK_S];
  };

  bool attacking() {
    return input->mousecodes[SDL_BUTTON_LEFT] ||
      player.attackAnimationCounter > 0 && player.attackAnimationCounter < player.attackSprites; 
  }

  bool defending() {
    return input->mousecodes[SDL_BUTTON_RIGHT];
  }

  void update() {
    if (attacking()) {
      attack();
    }
    else if (defending()) {
      defend();
    } else if (moving()) {
      move();
    } else {
      idle();
    }
  }
}
