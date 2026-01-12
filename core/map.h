#pragma once
#include "map_node.h"
#include <map>

class Player;

typedef enum textureEnum {
  MONESTARY,
  CASTLE,
} textureEnum;

typedef struct OverlayTexture {
  SDL_FRect rect;
  textureEnum texture;
} OverlayTexture;

class Map {
  public:
  map<int, MapNode> mapNodes;
  vector<vector<int>> mapArray;
  vector<vector<int>> mapOverlay2;
  vector<OverlayTexture> overlayTextures;

  Map();
  void update(const Player &player);

  private:
  Uint8 red;
  Uint8 green;
  Uint8 blue;
};
