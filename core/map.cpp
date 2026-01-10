#include "map.h"
#include "player.h"
#include "../globals.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <vector>

auto* debug_get_node(std::map<int, MapNode>& m, int key) {
    auto it = m.find(key);
    return it == m.end() ? nullptr : &it->second;
}


#define global static

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
  mapArray = { 
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

  for (int x = 0; x < mapArray[0].size(); x++) {
    for (int y = 0; y < mapArray.size(); y++) {
      SDL_FRect rect;
      rect.x = 64 * x;
      rect.y = 64 * y;
      rect.w = 64;
      rect.h = 64;

      SDL_FRect subRect;
      subRect.x = mapCoordinates[mapArray[y][x]].first;
      subRect.y = mapCoordinates[mapArray[y][x]].second;
      subRect.w = 64;
      subRect.h = 64;

      int key = y * mapArray[0].size() + x;
      MapNode node = MapNode(rect, subRect);
      mapNodes.emplace(key, MapNode(rect, subRect));
    }
  }

  for (int x = 0; x < mapArray[0].size(); x++) {
    for (int y = 0; y < mapArray.size(); y++) {
      int key = y * mapArray[0].size() + x;

      if (x > 0) {
        int nodeLeftKey = (y + 0) * mapArray[0].size() + (x - 1);
        mapNodes[key].neighbors.push_back(&mapNodes[nodeLeftKey]);
      }
      if (x < mapArray[0].size() - 1) {
        int nodeRightKey = (y + 0) * mapArray[0].size() + (x + 1);
        mapNodes[key].neighbors.push_back(&mapNodes[nodeRightKey]);
      }
      if (y > 0) {
        int nodeAboveKey = (y - 1) * mapArray[0].size() + (x + 0);
        mapNodes[key].neighbors.push_back(&mapNodes[nodeAboveKey]);
      } 
      if (y < mapArray.size() - 1) {
        int nodeBelowKey = (y + 1) * mapArray[0].size() + (x + 0);
        mapNodes[key].neighbors.push_back(&mapNodes[nodeBelowKey]);
      }

      if (y > 0 && x > 0) {
        int nodeLeftKey = (y - 1) * mapArray[0].size() + (x - 1);
        mapNodes[key].neighbors.push_back(&mapNodes[nodeLeftKey]);
      }
      if (y < mapArray.size() - 1 && x > 0) {
        int nodeLeftKey = (y + 1) * mapArray[0].size() + (x - 1);
        mapNodes[key].neighbors.push_back(&mapNodes[nodeLeftKey]);
      }
      if (y > 0 && x < mapArray[0].size() - 1) {
        int nodeLeftKey = (y - 1) * mapArray[0].size() + (x + 1);
        mapNodes[key].neighbors.push_back(&mapNodes[nodeLeftKey]);
      }
      if (y < mapArray.size() - 1 && x < mapArray[0].size() - 1) {
        int nodeLeftKey = (y + 1) * mapArray[0].size() + (x + 1);
        mapNodes[key].neighbors.push_back(&mapNodes[nodeLeftKey]);
      }  
    }
  }
  SDL_GetTextureColorMod(idleTexture, &red, &green, &blue);
}

void Map::update(const Player &player) {
  for (const auto& [key, value] : mapNodes) {
    if (value.clicked || player.mapNodeIndex == key) {
      SDL_SetTextureColorMod(tileMapTexture, 204, 51, 51);
    } else if (value.start) {
      SDL_SetTextureColorMod(tileMapTexture, 0, 255, 0);
    } else if (value.end) {
      SDL_SetTextureColorMod(tileMapTexture, 255, 0, 0);
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

  MapNode* end = &mapNodes[player.mapNodeIndex];

  while (end->parent != nullptr) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderLine(
      renderer,
      end->rect.x + 64 / 2,
      end->rect.y + 64 / 2,
      end->parent->rect.x + 64 / 2,
      end->parent->rect.y + 64 / 2
    );
    end = end->parent;
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
