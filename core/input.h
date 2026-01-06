#pragma once
#include <SDL3/SDL_events.h>
#include <map>
using namespace std;

typedef struct inputStruct {
  map<int, bool> keycodes;
  // map<int, bool> mousecodes;
  map<int, SDL_MouseButtonEvent> mousecodes;
} inputStruct;
