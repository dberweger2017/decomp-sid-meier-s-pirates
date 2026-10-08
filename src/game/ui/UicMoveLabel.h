#pragma once

// Partial view based on startMove's two byte stores.
class UicMoveLabel {
public:
    void startMove(bool moving);

private:
    unsigned char m_unknown_00[0x94];
    unsigned char m_moving; // +0x94
    unsigned char m_unknown_95[7];
    unsigned char m_started; // +0x9c
};
