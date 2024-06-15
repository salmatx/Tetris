#include <cstdio>
#include "game.h"

namespace game {

Game::~Game() {
    UnloadFont(this->font_);
    CloseWindow();
}

void Game::InitRenderer() {
    InitWindow(this->kScreenWidth_, this->kScreenHeight_, this->kTitle_);
    this->font_ = LoadFont(font_type_);
    for (auto &player : players_) {
        player->SetFont(this->font_);
    }
    SetTargetFPS(60);
}

void Game::GameLoop() {
    while (!WindowShouldClose()) {
        PlayerMove input;
        try {
            input = this->GetMoveType().value();
        }
        catch (std::bad_optional_access& e) {
            input = PlayerMove{MoveType::kNone, PlayerType::kPlayerNone};
        }

        if (this->game_phase_ == GameState::kGameStartPhase) {
            this->UpdateGameStart(input.moveType);
            this->DrawStartScreen();
            for (auto& player : players_) {
                 player->UpdatePlayer(input.moveType, this->game_phase_);
            }
        }

        switch (input.player) {
            case PlayerType::kPlayer1:
                this->game_phase_ = this->players_.at(0)->UpdatePlayer(input.moveType, this->game_phase_);
                break;
            case PlayerType::kPlayer2:
                this->game_phase_ = this->players_.at(1)->UpdatePlayer(input.moveType, this->game_phase_);
                break;
            case PlayerType::kPlayerNone:
                this->game_phase_ = this->players_.at(0)->UpdatePlayer(input.moveType, this->game_phase_);
                this->game_phase_ = this->players_.at(1)->UpdatePlayer(input.moveType, this->game_phase_);
                break;
        }

        this->RenderGame();
    }
}

void Game::RenderGame() const {
    BeginDrawing();
    ClearBackground(game::kBackgroundColor);
    for (const auto& player : players_) {
        player->DrawPlayer();
    }
    EndDrawing();
}

std::optional<PlayerMove> Game::GetMoveType() const {
    double wait_for_key_down = 0.05;

    if (IsKeyPressed(KEY_ENTER))
        return PlayerMove{MoveType::kConfirm, PlayerType::kPlayer1};

    if (IsKeyPressed(KEY_A) && IsKeyReleased(KEY_A))
        return PlayerMove{MoveType::kLeft, PlayerType::kPlayer1};
    if (IsKeyPressed(KEY_D) && IsKeyReleased(KEY_D))
        return PlayerMove{MoveType::kRight, PlayerType::kPlayer1};
    if (IsKeyPressed(KEY_W))
        return PlayerMove{MoveType::kUp, PlayerType::kPlayer1};
    if (IsKeyPressed(KEY_S))
        return PlayerMove{MoveType::kDown, PlayerType::kPlayer1};
    if (IsKeyPressed(KEY_LEFT_CONTROL))
        return PlayerMove{MoveType::kDrop, PlayerType::kPlayer1};
    if (IsKeyDown(KEY_A)) {
        WaitTime(wait_for_key_down);
        return PlayerMove{MoveType::kLeft, PlayerType::kPlayer1};
    }
    if (IsKeyDown(KEY_D)) {
        WaitTime(wait_for_key_down);
        return PlayerMove{MoveType::kRight, PlayerType::kPlayer1};
    }

    if (IsKeyPressed(KEY_LEFT) && IsKeyReleased(KEY_LEFT))
        return PlayerMove{MoveType::kLeft, PlayerType::kPlayer2};
    if (IsKeyPressed(KEY_RIGHT) && IsKeyReleased(KEY_RIGHT))
        return PlayerMove{MoveType::kRight, PlayerType::kPlayer2};
    if (IsKeyPressed(KEY_UP))
        return PlayerMove{MoveType::kUp, PlayerType::kPlayer2};
    if (IsKeyPressed(KEY_DOWN))
        return PlayerMove{MoveType::kDown, PlayerType::kPlayer2};
    if (IsKeyPressed(KEY_RIGHT_CONTROL))
        return PlayerMove{MoveType::kDrop, PlayerType::kPlayer2};
    if (IsKeyDown(KEY_LEFT)) {
        WaitTime(wait_for_key_down);
        return PlayerMove{MoveType::kLeft, PlayerType::kPlayer2};
    }
    if (IsKeyDown(KEY_RIGHT)) {
        WaitTime(wait_for_key_down);
        return PlayerMove{MoveType::kRight, PlayerType::kPlayer2};
    }

    return std::nullopt;
}

void Game::UpdateGameStart(const MoveType input) {
    if (input == MoveType::kUp) {
        ++this->start_level_;
    }
    if (input == MoveType::kDown && this->start_level_ > 0) {
        --this->start_level_;
    }
    if (input == MoveType::kConfirm) {
        for (auto& player : players_) {
            player->SetStartLevel(this->start_level_);
        }
        this->game_phase_ = GameState::kGamePlayPhase;
    }
}

void Game::DrawStartScreen() const {
    char buffer[2048];
    std::sprintf(buffer, "START LEVEL: %ld", this->start_level_);
    float x = this->kScreenWidth_ / 2.0f;
    float y = this->kScreenHeight_ / 2.0f - 100.0f;
    float spacing = 30;
    game::DrawString(this->font_, this->font_.baseSize * 1.5, "TETRIS", x, y, TextAlignment::kCenter, WHITE);
    y += spacing;
    game::DrawString(this->font_, this->font_.baseSize * 1.5, buffer, x, y, TextAlignment::kCenter, WHITE);
    y += 3 * spacing;
    game::DrawString(this->font_, this->font_.baseSize * 1.2, "CONTROLS", x, y, TextAlignment::kCenter, WHITE);
    x = this->kScreenWidth_ / 2.0f - 120;
    y += 2 * spacing;
    game::DrawString(this->font_, this->font_.baseSize, "SELECT LEVEL:", x, y, TextAlignment::kLeft, WHITE);
    game::DrawString(this->font_, this->font_.baseSize, "UP KEY / DOWN KEY", x + 120, y, TextAlignment::kLeft, WHITE);
    y += spacing;
    game::DrawString(this->font_, this->font_.baseSize, "MOVE:", x, y, TextAlignment::kLeft, WHITE);
    game::DrawString(this->font_, this->font_.baseSize, "LEFT KEY / RIGHT KEY", x + 120, y, TextAlignment::kLeft, WHITE);
    y += spacing;
    game::DrawString(this->font_, this->font_.baseSize, "ROTATE:", x, y, TextAlignment::kLeft, WHITE);
    game::DrawString(this->font_, this->font_.baseSize, "UP KEY", x + 120, y, TextAlignment::kLeft, WHITE);
    y += spacing;
    game::DrawString(this->font_, this->font_.baseSize, "FAST DROP:", x, y, TextAlignment::kLeft, WHITE);
    game::DrawString(this->font_, this->font_.baseSize, "DOWN KEY", x + 120, y, TextAlignment::kLeft, WHITE);
    y += spacing;
    game::DrawString(this->font_, this->font_.baseSize, "HARD DROP:", x, y, TextAlignment::kLeft, WHITE);
    game::DrawString(this->font_, this->font_.baseSize, "SPACE KEY", x + 120, y, TextAlignment::kLeft, WHITE);
}

void DrawString(Font font, float font_size, const char* msg, size_t x, size_t y,
                TextAlignment alignment, Color color) {
    const int spacing = -2;
    float x_pos;
    float y_pos = (float)y;
    switch (alignment) {
        case TextAlignment::kRight:
            x_pos = (float)x - MeasureTextEx(font,msg, font_size, spacing).x;
            break;
        case TextAlignment::kCenter:
            x_pos = (float)x - MeasureTextEx(font,msg, font_size, spacing).x / 2;
            break;
        case TextAlignment::kLeft:
            x_pos = (float)x;
            break;
    }
    Vector2 position = {x_pos, y_pos};
    DrawTextEx(font, msg, position, font_size, spacing, color);
}
}