#pragma once

#include "common.h"

namespace game {

class IPlayer {
public:
    virtual MoveTypes GetMoveType() const = 0;
};

}
