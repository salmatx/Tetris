#include "renderer.h"

//#define NDEBUG //uncomment in release to disable assert()

using namespace game;

int main() {
    Renderer renderer{};
    renderer.InitRenderer();
    renderer.GameLoop();

    return 0;
}
