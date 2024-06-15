#pragma once

#include <cstddef>
#include <raylib.h>
#include <array>
#include <type_traits>
#include <concepts>
#include "i_game.h"
#include "i_player.h"

namespace game {

template <typename T>
concept IsPlayer = std::is_base_of_v<IPlayer, std::remove_reference_t<T>>;

class Game : public IGame{
public:
    template<typename... T>
    requires (IsPlayer<T>&&...) && (sizeof...(T) == 2)
    Game(const size_t screen_height, const size_t screen_width, const char* font, T&&... players)
        : kScreenHeight_(screen_height),
         kScreenWidth_(screen_width),
         players_{&players...},
         font_type_(font) {};
    ~Game() override;
    void InitRenderer() override;
    void GameLoop() override;
    std::optional<PlayerMove> GetMoveType() const override;

private:
    const size_t kScreenHeight_;
    const size_t kScreenWidth_;
    const char* kTitle_ = "Tetris";
    const std::array<IPlayer*, 2> players_;
    const char* font_type_;
    Font font_;
    size_t start_level_ = 0;
    GameState game_phase_ = GameState::kGameStartPhase;

    void RenderGame() const;
    void UpdateGameStart(const MoveType input);
    void UpdateGameOver(const MoveType input);
    void DrawStartScreen() const;
};

void DrawString(Font font, float font_size, const char* msg, size_t x, size_t y, TextAlignment alignment, Color color);

}
