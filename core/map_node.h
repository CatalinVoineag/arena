#include <SDL3/SDL_rect.h>
#include <vector>
using namespace std;

class Enemy;

class MapNode {
  public:
  SDL_FRect rect;
  SDL_FRect subRect;
  bool clicked;
  bool obstacle;
  bool start;
  bool end;
  vector<MapNode*> neighbors;
  int key;
  Enemy *objOnTop = nullptr;

  MapNode() // default constructor
    : rect{0, 0, 0, 0}, subRect{0, 0, 0, 0} {}

  MapNode(SDL_FRect rect, SDL_FRect subRect, int key);

  private:
};
