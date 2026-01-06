#include <SDL3/SDL_rect.h>

class MapNode {
  public:
  SDL_FRect rect;
  SDL_FRect subRect;
  bool clicked;
  bool obstacle;

  MapNode(SDL_FRect rect, SDL_FRect subRect);

  private:
};
