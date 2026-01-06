#include "player.h"
#include "map_node.h"

class Map {
  public:
  map<string, MapNode> mapNodes;

  Map();
  void update(Player player);

  private:
  Uint8 red;
  Uint8 green;
  Uint8 blue;
};
