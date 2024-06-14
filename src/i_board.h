#pragma once

#include "common.h"
#include "piece.h"

#include <cstddef>
#include <vector>

namespace game {

class IBoard {
public:
    virtual std::shared_ptr<tetrino[]> GetPiece(const PieceType type) const = 0;
    virtual int GetShadowPieceRowPosition() = 0;
    virtual int GetPieceRowPosition(const PieceType type) const = 0;
    virtual int GetPieceColumnPosition(const PieceType type) const = 0;
    virtual uint16_t GetPieceSize(const PieceType type) const = 0;
    virtual uint8_t GetBoardHeight() const = 0;
    virtual uint8_t GetBoardWidth() const = 0;
    virtual std::vector<std::vector<uint8_t>> GetBoard() const = 0;
    virtual size_t GetClearedLineCount() const = 0;
    virtual bool IsLineClearing(int index) const= 0;
    virtual void UpdateGame(MoveTypes input) = 0;
    virtual GameState GetActualGamePhase() const = 0;
    virtual size_t GetStartLevel() const = 0;
    virtual size_t GetLevel() const = 0;
    virtual size_t GetPoints() const = 0;
};

}
