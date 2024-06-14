#include "player.h"

//#define NDEBUG //uncomment in release to disable assert()

using namespace game;

int main() {
    Board board{};
    Player renderer{board};
    renderer.InitRenderer();
    renderer.GameLoop();

    return 0;
}
