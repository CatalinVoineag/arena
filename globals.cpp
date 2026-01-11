#include "globals.h"

Uint32 frameDuration;
SDL_Texture *idleTexture;
SDL_Texture *moveTexture;
SDL_Texture *attackTexture;
SDL_Texture *defendTexture;
map<uintptr_t, SDL_FRect> obj_coordinates;
uint64_t lastTicks;
float MAX_DT;
