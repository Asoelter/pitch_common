#include "messages.h"

#include <covid/socket.h>

char firstByte(StandardChunkType chunk)
{
    return static_cast<char>((chunk >> 8) & 0x0F);
}

char secondByte(StandardChunkType chunk)
{
    return static_cast<char>(chunk & 0x0F);
}

template<StandardChunkType BufferSize>
void serializeHeader(char buffer[BufferSize], StandardChunkType type)
{
    static_assert(BufferSize >= headerSize);

    buffer[0] = firstByte(BufferSize);
    buffer[1] = secondByte(BufferSize);
    buffer[2] = firstByte(type);
    buffer[3] = secondByte(type);
}

std::vector<char> PlayerReadyMessage::serialize() const
{
    char buffer[size];

    serializeHeader<size>(buffer, type);

    return std::vector<char>(std::begin(buffer), std::end(buffer));
}

std::vector<char> PlayedCardMessage::serialize() const
{
    static_assert(sizeof(CardSuit) == 1 , "Card::Suit has gotten too large");
    static_assert(sizeof(CardNumber) == 1 , "Card::Number has gotten too large");

    char buffer[size];

    serializeHeader<size>(buffer, type);
    buffer[4] = static_cast<char>(suit);
    buffer[5] = static_cast<char>(number);

    return std::vector<char>(std::begin(buffer), std::end(buffer));
}
