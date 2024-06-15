#pragma once

#include "common.h"

namespace game {

class IPlayer {
public:
    virtual void DrawPlayer() const = 0;
    virtual GameState UpdatePlayer(MoveType input) = 0;
    virtual void SetStartLevel(size_t level) = 0;
    virtual void SetFont(const Font &font) = 0;
    virtual void StartGame() = 0;
    virtual void PlayGame() = 0;
    virtual void GameOver() = 0;
    virtual ~IPlayer() = default;
};

}
