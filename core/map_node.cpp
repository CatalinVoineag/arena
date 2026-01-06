#include "map_node.h"
#include <cstdio>

MapNode::MapNode(SDL_FRect rect, SDL_FRect subRect) {
  this->rect = rect;
  this->subRect = subRect;
  this->clicked = false;
  this->obstacle = false;
} 
