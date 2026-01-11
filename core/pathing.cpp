#include "map.h"
#include "pathing.h"
#include <cmath>
#include <queue>

struct NodeState {
  MapNode* parent = nullptr;
  float globalGoal = INFINITY;
  float localGoal = INFINITY;
  bool seen = false;
};

struct CompareNodes {
    vector<NodeState>* state;

    bool operator()(MapNode* a, MapNode* b) const {
        return (*state)[a->key].globalGoal >
               (*state)[b->key].globalGoal;
    }
};

float distanceToRect(SDL_FRect start, SDL_FRect end) {
  float x = start.x - end.x;
  float y = start.y - end.y;
  return sqrt(pow(x, 2) + pow(y, 2));
};

Pathing::Pathing() {
}

vector<MapNode*> Pathing::solveAStar(Map &gameMap, int startNodeIndex, int endNodeIndex) {
  int width = gameMap.mapArray[0].size();
  int height = gameMap.mapArray.size();
  vector<NodeState> mapNodes(width * height);

  MapNode* start = &gameMap.mapNodes[startNodeIndex];
  MapNode* end = &gameMap.mapNodes[endNodeIndex]; 
  
  MapNode *currentNode = start;
  mapNodes[currentNode->key].localGoal = 0.0f;
  mapNodes[currentNode->key].globalGoal = distanceToRect(start->rect, end->rect);

  priority_queue<MapNode*, vector<MapNode*>, CompareNodes > pq { CompareNodes{ &mapNodes } };
  pq.push(start);

  while (!pq.empty() && currentNode != end) {
    while (!pq.empty() && mapNodes[pq.top()->key].seen) {
      pq.pop();
    }

    if (pq.empty()) {
      break;
    }

    currentNode = pq.top();
    mapNodes[currentNode->key].seen = true;

    for (auto neighbor : currentNode->neighbors) {
      if (!mapNodes[neighbor->key].seen && !neighbor->obstacle) {
        pq.push(neighbor);
      }

      float possibleLocalGoal = mapNodes[currentNode->key].localGoal + distanceToRect(currentNode->rect, neighbor->rect); 

      if (possibleLocalGoal < mapNodes[neighbor->key].localGoal) {
        mapNodes[neighbor->key].parent = currentNode;
        mapNodes[neighbor->key].localGoal = possibleLocalGoal;
        mapNodes[neighbor->key].globalGoal = mapNodes[neighbor->key].localGoal + distanceToRect(neighbor->rect, end->rect);
      }
    } 
  }
  vector<MapNode*> nodes;
  MapNode* node = end;
  while (node != nullptr) {
    nodes.push_back(node);
    node = mapNodes[node->key].parent;
  } 

  return nodes;
}
