#pragma once
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
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
#include <map>
#include "core/input.h"
using namespace std;

extern Uint32 frameDuration;
extern SDL_Texture *idleTexture;
extern SDL_Texture *moveTexture;
extern SDL_Texture *attackTexture;
extern SDL_Texture *defendTexture;
extern SDL_Texture *enemyIdleTexture;
extern SDL_Texture *tileMapTexture;
extern SDL_Texture *monastaryTexture;

extern SDL_Renderer *renderer;
extern SDL_Window *window;
extern inputStruct *input;
