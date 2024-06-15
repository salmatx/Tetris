#pragma once

#include <optional>
#include "common.h"

namespace game {

class IGame {
public:
    virtual void InitRenderer() = 0;
    virtual void GameLoop() = 0;
    virtual std::optional<PlayerMove> GetMoveType() const = 0;
    virtual ~IGame() = default;
};

}