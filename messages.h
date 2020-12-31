#ifndef MESSAGES_H
#define MESSAGES_H

#include <cstdint>
#include <vector>

#include "card_enums.h"

namespace
{
    using StandardChunkType = uint16_t;
    constexpr size_t headerSize     = 2 * sizeof(StandardChunkType);
    constexpr size_t playerReadyId  = 1;
    constexpr size_t playedCardId   = 2;
}

struct PlayerReadyMessage
{
    std::vector<char> serialize() const;

    static constexpr StandardChunkType size = headerSize;
    static constexpr StandardChunkType type = playerReadyId;
};

struct PlayedCardMessage
{
    PlayedCardMessage(CardSuit s, CardNumber n) : suit(s), number(n) {}

    std::vector<char> serialize() const;

    static constexpr StandardChunkType size = headerSize + sizeof(CardSuit) + sizeof(CardNumber);
    static constexpr StandardChunkType type = playedCardId;

    CardSuit   suit;
    CardNumber number;
};

#endif //MESSAGES_H
