#include "renderer.h"
#include "game.h"

#include <iostream>

//#define NDEBUG //uncomment in release to disable assert()

using namespace game;

int main() {
    Game game{};
    Renderer renderer{};
    renderer.InitRenderer();
    renderer.GameLoop(&game);

    return 0;
}
