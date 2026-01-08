#pragma once
#include "map_node.h"
#include <map>

class Player;

class Map {
  public:
  map<int, MapNode> mapNodes;
  vector<vector<int>> mapArray;

  Map();
  void update(const Player &player);

  private:
  Uint8 red;
  Uint8 green;
  Uint8 blue;
};
