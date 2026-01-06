#include "map.h"
#include "../globals.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <vector>

#define global static

global vector<vector<int>> mapArray = { 
    { 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2 },
    { 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
    { 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
    { 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
    { 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
    { 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
    { 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
    { 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
    { 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
    { 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
    { 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
    { 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
    { 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
    { 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
    { 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
    { 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
    { 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4 },
  };

global map<int, pair<int, int>> mapCoordinates {
    { 0, {64, 64}},
    { 1, {64, 0}},
    { 2, {128, 0}},
    { 3, {128, 64}},
    { 4, {128, 128}},
    { 5, {64, 128}},
    { 6, {0, 128}},
    { 7, {0, 64}},
    { 8, {0, 0}}
  };

Map::Map() {
  for (int w = 0; w < mapArray[0].size(); w++) {
    for (int h = 0; h < mapArray.size(); h++) {
      SDL_FRect rect;
      rect.x = 64 * w;
      rect.y = 64 * h;
      rect.w = 64;
      rect.h = 64;

      SDL_FRect subRect;
      subRect.x = mapCoordinates[mapArray[h][w]].first;
      subRect.y = mapCoordinates[mapArray[h][w]].second;
      subRect.w = 64;
      subRect.h = 64;

      string stringw = to_string(w); 
      string stringy = to_string(h); 
      string key = stringw + "_" + stringy;
      MapNode node = MapNode(rect, subRect);
      mapNodes.emplace(key, MapNode(rect, subRect));
    }
  }
  SDL_GetTextureColorMod(idleTexture, &red, &green, &blue);
}

void Map::update(Player player) {
  for (const auto& [key, value] : mapNodes) {
    if (value.clicked) {
      SDL_SetTextureColorMod(tileMapTexture, 204, 51, 51);
    } else {
      SDL_SetTextureColorMod(tileMapTexture, red, green, blue);
    }

    SDL_RenderTexture(
      renderer,
      tileMapTexture,
      &value.subRect,
      &value.rect
    );

    SDL_RenderRect(renderer, &value.rect);
  }

  SDL_FRect rect;
  rect.x = 800;
  rect.y = 100;
  rect.w = 192;
  rect.h = 320;

  uintptr_t id = reinterpret_cast<uintptr_t>(&rect);
  obj_coordinates[id] = { rect };

  SDL_RenderTexture(renderer, monastaryTexture, NULL, &rect);
}
