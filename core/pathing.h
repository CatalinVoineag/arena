#include <vector>
using namespace std;

class MapNode;
class Map;

class Pathing {
  public:
  Pathing();
  vector<MapNode*> solveAStar(Map &gameMap, int startNodeIndex, int endNodeIndex);
};
