#include <cstdio>
#include <iostream>
#include <filesystem>
#include <fstream>
#include "game.h"
#include "lib/tinyfiledialogs.h"

namespace game {

namespace fs = std::filesystem;

template class Game<1>;
template class Game<2>;

template <std::uint8_t N>
requires ValidPlayerCount<N>
Game<N>::~Game() {
    UnloadFont(this->font_);
    CloseWindow();
}

template <std::uint8_t N>
requires ValidPlayerCount<N>
void Game<N>::InitRenderer() {
    InitWindow(this->kScreenWidth_, this->kScreenHeight_, this->kTitle_);
    this->font_ = LoadFont(font_type_);
    for (auto &player : players_) {
        player->SetFont(this->font_);
    }
    SetTargetFPS(60);
}

template <std::uint8_t N>
requires ValidPlayerCount<N>
void Game<N>::GameLoop() {
    while (!WindowShouldClose()) {
        PlayerMove input;
        try {
            input = this->GetMoveType().value();
        }
        catch (std::bad_optional_access& e) {
            input = PlayerMove{MoveType::kNone, PlayerType::kPlayerNone};
        }

        switch (this->game_phase_) {
            case GameState::kGameStartPhase:
                this->UpdateGameStart(input.moveType);
                this->DrawStartScreen();
                break;

            case GameState::kGameOverPhase: {
                this->UpdateGameOver(input.moveType);
                size_t x = this->kScreenWidth_ / 2;
                size_t y = this->kScreenHeight_ / 2 - 80;
                game::DrawString(this->font_, this->font_.baseSize * 2.5,
                                 "GAME OVER", x, y, TextAlignment::kCenter,
                                 WHITE);
                break;
            }

            case GameState::kGamePlayPhase:
            case GameState::kGameLinePhase:
                switch (input.player) {
                    case PlayerType::kPlayer1:
                        this->game_phase_ = this->players_.at(0)->UpdatePlayer(input.moveType);
                        break;
                    case PlayerType::kPlayer2:
                        this->game_phase_ = this->players_.at(1)->UpdatePlayer(input.moveType);
                        break;
                    case PlayerType::kPlayerNone:
                        for (const auto &player : this->players_) {
                            this->game_phase_ = player->UpdatePlayer(input.moveType);
                            if (this->game_phase_ == GameState::kGameOverPhase)
                                break;
                        }
                        break;
                }

                if (input.moveType == MoveType::kPause) {
                    this->game_phase_ = GameState::kGamePause;
                }
                break;

            case GameState::kGamePause:
                this->PauseGame();
                break;
        }

        this->RenderGame();
    }
}

template <std::uint8_t N>
requires ValidPlayerCount<N>
void Game<N>::RenderGame() const {
    BeginDrawing();
    ClearBackground(game::kBackgroundColor);
    for (const auto& player : players_) {
        player->DrawPlayer();
    }
    EndDrawing();
}

template <std::uint8_t N>
requires ValidPlayerCount<N>
std::optional<PlayerMove> Game<N>::GetMoveType() const {
    if (IsKeyPressed(KEY_ENTER))
        return PlayerMove{MoveType::kConfirm, PlayerType::kPlayer1};
    if (IsKeyPressed(KEY_P))
        return PlayerMove{MoveType::kPause, PlayerType::kPlayer1};

    if (IsKeyPressed(KEY_A))
        return PlayerMove{MoveType::kLeft, PlayerType::kPlayer1};
    if (IsKeyPressed(KEY_D))
        return PlayerMove{MoveType::kRight, PlayerType::kPlayer1};
    if (IsKeyPressed(KEY_W))
        return PlayerMove{MoveType::kUp, PlayerType::kPlayer1};
    if (IsKeyPressed(KEY_S))
        return PlayerMove{MoveType::kDown, PlayerType::kPlayer1};
    if (IsKeyPressed(KEY_LEFT_CONTROL))
        return PlayerMove{MoveType::kDrop, PlayerType::kPlayer1};

    if (N == 2) {
        if (IsKeyPressed(KEY_LEFT))
            return PlayerMove{MoveType::kLeft, PlayerType::kPlayer2};
        if (IsKeyPressed(KEY_RIGHT))
            return PlayerMove{MoveType::kRight, PlayerType::kPlayer2};
        if (IsKeyPressed(KEY_UP))
            return PlayerMove{MoveType::kUp, PlayerType::kPlayer2};
        if (IsKeyPressed(KEY_DOWN))
            return PlayerMove{MoveType::kDown, PlayerType::kPlayer2};
        if (IsKeyPressed(KEY_RIGHT_CONTROL))
            return PlayerMove{MoveType::kDrop, PlayerType::kPlayer2};
    }

    return std::nullopt;
}

template <std::uint8_t N>
requires ValidPlayerCount<N>
void Game<N>::UpdateGameStart(const MoveType input) {
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
        for (const auto &player : players_) {
            player->StartGame();
            player->UpdatePlayer(MoveType::kNone);
            player->PlayGame();
        }
    }
}

template <std::uint8_t N>
requires ValidPlayerCount<N>
void Game<N>::DrawStartScreen() const {
    char buffer[2048];
    std::sprintf(buffer, "START LEVEL: %ld", this->start_level_);
    float x = this->kScreenWidth_ / 2.0f;
    float y = this->kScreenHeight_ / 2.0f - 200.0f;
    float spacing = 30;
    game::DrawString(this->font_, this->font_.baseSize * 1.5, "TETRIS", x, y, TextAlignment::kCenter, WHITE);
    y += spacing;
    game::DrawString(this->font_, this->font_.baseSize * 1.5, buffer, x, y, TextAlignment::kCenter, WHITE);
    x = this->kScreenWidth_ / 2.0f;
    y += 3 * spacing;
    game::DrawString(this->font_, this->font_.baseSize, "START GAME:      ENTER", x, y, TextAlignment::kCenter, WHITE);
    y += spacing;
    game::DrawString(this->font_, this->font_.baseSize, "SELECT LEVEL:      UP KEY (W) / DOWN KEY (S)", x, y, TextAlignment::kCenter, WHITE);
    y += 2 * spacing;
    game::DrawString(this->font_, this->font_.baseSize * 1.2, "CONTROLS", x, y, TextAlignment::kCenter, WHITE);
    y += 2 * spacing;
    auto x1 = x - 200;
    auto x2 = x + 140;
    auto y1 = y;
    auto y2 = y;
    game::DrawString(this->font_, this->font_.baseSize, "PLAYER 1", x1, y1, TextAlignment::kLeft, WHITE);
    x1 -= 80;
    y1 += 2 * spacing;
    game::DrawString(this->font_, this->font_.baseSize, "MOVE:", x1, y1, TextAlignment::kLeft, WHITE);
    game::DrawString(this->font_, this->font_.baseSize, "A / D", x1 + 120, y1, TextAlignment::kLeft, WHITE);
    y1 += spacing;
    game::DrawString(this->font_, this->font_.baseSize, "ROTATE:", x1, y1, TextAlignment::kLeft, WHITE);
    game::DrawString(this->font_, this->font_.baseSize, "W", x1 + 120, y1, TextAlignment::kLeft, WHITE);
    y1 += spacing;
    game::DrawString(this->font_, this->font_.baseSize, "FAST DROP:", x1, y1, TextAlignment::kLeft, WHITE);
    game::DrawString(this->font_, this->font_.baseSize, "S", x1 + 120, y1, TextAlignment::kLeft, WHITE);
    y1 += spacing;
    game::DrawString(this->font_, this->font_.baseSize, "HARD DROP:", x1, y1, TextAlignment::kLeft, WHITE);
    game::DrawString(this->font_, this->font_.baseSize, "LEFT CONTROL", x1 + 120, y1, TextAlignment::kLeft, WHITE);

    game::DrawString(this->font_, this->font_.baseSize, "PLAYER 2", x2, y2, TextAlignment::kLeft, WHITE);
    x2 -= 80;
    y2 += 2 * spacing;
    game::DrawString(this->font_, this->font_.baseSize, "MOVE:", x2, y2, TextAlignment::kLeft, WHITE);
    game::DrawString(this->font_, this->font_.baseSize, "LEFT KEY / RIGHT KEY", x2 + 120, y2, TextAlignment::kLeft, WHITE);
    y2 += spacing;
    game::DrawString(this->font_, this->font_.baseSize, "ROTATE:", x2, y2, TextAlignment::kLeft, WHITE);
    game::DrawString(this->font_, this->font_.baseSize, "UP KEY", x2 + 120, y2, TextAlignment::kLeft, WHITE);
    y2 += spacing;
    game::DrawString(this->font_, this->font_.baseSize, "FAST DROP:", x2, y2, TextAlignment::kLeft, WHITE);
    game::DrawString(this->font_, this->font_.baseSize, "DOWN KEY", x2 + 120, y2, TextAlignment::kLeft, WHITE);
    y2 += spacing;
    game::DrawString(this->font_, this->font_.baseSize, "HARD DROP:", x2, y2, TextAlignment::kLeft, WHITE);
    game::DrawString(this->font_, this->font_.baseSize, "RIGHT CONTROL", x2 + 120, y2, TextAlignment::kLeft, WHITE);
}

template <std::uint8_t N>
requires ValidPlayerCount<N>
void Game<N>::UpdateGameOver(const MoveType input) {
    if (input == MoveType::kConfirm) {
        for (const auto& player: players_) {
            player->GameOver();
            player->UpdatePlayer(MoveType::kNone);
        }
        this->game_phase_ = GameState::kGameStartPhase;
    }
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

std::string OpenFileExplorer(bool option_file) {
    if (option_file) {
        const char* filePath = tinyfd_openFileDialog("Select a file", "", 0,
                                                     nullptr, nullptr, 0);
        if (filePath) {
            return {filePath};
        }
    }
    else {
        const char* folderPath = tinyfd_selectFolderDialog("Select a folder", "");
        if (folderPath) {
            return {folderPath};
        }
    }

    return "";
}

std::string GenerateSaveFileName(const std::string& directoryPath) {
    int fileNumber = 1;
    std::string baseName = "tetris_save_";
    std::string extension = ".json";
    std::ostringstream fileName;

    do {
        fileName.str("");
        fileName << baseName << std::setw(2) << std::setfill('0') << fileNumber << extension;
        ++fileNumber;
    } while (fs::exists(fs::path(directoryPath) / fileName.str()));

    return (fs::path(directoryPath) / fileName.str()).string();
}

bool CreateSaveFile(const std::string& directoryPath, const json &data) {
    std::string saveFileName = GenerateSaveFileName(directoryPath);

    std::ofstream ofs(saveFileName);
    if (!ofs) {
        std::cerr << "Failed to create file: " << saveFileName << std::endl;
        return false;
    }

    ofs << data;
    ofs.close();

    std::cout << "File created: " << saveFileName << std::endl;
    return true;
}

void DrawMessageBox(const char* message, int x_pos, int y_pos) {
    bool showModal = true;
    int width = 400;
    int height = 200;
    int x = x_pos - width / 2;
    int y = y_pos - height / 2;

    while (showModal && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangle(x, y, width, height, DARKGRAY);
        DrawText(message, x + 50, y + 50, 20, WHITE);
        DrawText("Press Enter to continue...", x + 50, y + 110, 20, LIGHTGRAY);

        if (IsKeyPressed(KEY_ENTER)) {
            showModal = false;
        }

        EndDrawing();
    }
}

template<std::uint8_t N>
requires ValidPlayerCount<N>json Game<N>::SaveToJson() {
    json doc;
    for (int i = 0; i < N; ++i) {
        auto tmp = dynamic_cast<ISaveService*>(this->players_.at(i));
        std::string key = "Player" + std::to_string(i);
        doc[key] = tmp->SaveToJson();
    }
    std::string path = OpenFileExplorer(false);
    if (!path.empty()) {
        if (CreateSaveFile(path, doc)) {
            DrawMessageBox("File saved successfully!", this->kScreenWidth_ / 2,
                           this->kScreenHeight_ / 2);
        } else {
            DrawMessageBox("Saving error!", this->kScreenWidth_ / 2,
                           this->kScreenHeight_ / 2);
        }
    }
    return doc;
}

template<std::uint8_t N>
requires ValidPlayerCount<N>void Game<N>::PauseGame() {
    size_t x = this->kScreenWidth_ / 2;
    size_t y = this->kScreenHeight_ / 2 - 80;
    game::DrawString(this->font_, this->font_.baseSize * 2.5,
                     "GAME PAUSED", x, y, TextAlignment::kCenter,
                     WHITE);
    y += 60;
    game::DrawString(this->font_, this->font_.baseSize * 2.5,
                     "DO YOU WANT TO SAVE GAME?     Y/N", x, y, TextAlignment::kCenter,
                     WHITE);
    if (IsKeyPressed(KEY_Y)) {
        this->SaveToJson();
        this->game_phase_ = GameState::kGamePlayPhase;
    }
    if (IsKeyPressed(KEY_N)) {
        this->game_phase_ = GameState::kGamePlayPhase;
    }
}

}