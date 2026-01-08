#include "map_node.h"
#include <cmath>

MapNode::MapNode(SDL_FRect rect, SDL_FRect subRect) {
  this->rect = rect;
  this->subRect = subRect;
  this->clicked = false;
  this->obstacle = false;
  this->start = false;
  this->end = false;
} 
