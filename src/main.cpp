#include "player.h"
#include "game.h"

//#define NDEBUG //uncomment in release to disable assert()

using namespace game;

int main() {
    const int window_height = 720;
    const int window_width = 880;
    Board board_1{};
    Board board_2{};
    Player player_1{board_1, 0};
    Player player_2{board_2, window_width / 2};
    Game<2> game{window_height, window_width, game::font_type, player_1, player_2};
    game.InitRenderer();
    game.GameLoop();

    return 0;
}
