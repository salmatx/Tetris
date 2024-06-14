#pragma once

#include "common.h"

namespace game {

class IPlayer {
public:
    virtual std::optional<MoveTypes> GetMoveType() const = 0;
};

}
