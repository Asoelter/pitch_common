#include "messages.h"

#include <cassert>
#include <covid/socket.h>

#define UNIMPLEMENTED_RETURN(val) assert(false); return val;

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

std::optional<PlayerReadyMessage> PlayerReadyMessage::deserialize(const std::vector<char>& buffer)
{
    if(buffer.size() != PlayerReadyMessage::size)
    {
        return std::optional<PlayerReadyMessage>();
    }

    const uint16_t size = (buffer[0] << CHAR_BIT) & buffer[1];
    const uint16_t messageType = (buffer[2] << CHAR_BIT) & buffer[3];

    if(size != PlayerReadyMessage::size || messageType != PlayerReadyMessage::type)
    {
        return std::optional<PlayerReadyMessage>();
    }

    return PlayerReadyMessage{};
}

std::vector<char> AcknowledgePlayerReadyMessage::serialize() const
{
    char buffer[size];

    serializeHeader<size>(buffer, type);

    return std::vector<char>(std::begin(buffer), std::end(buffer));
}

std::optional<AcknowledgePlayerReadyMessage> AcknowledgePlayerReadyMessage::deserialize(const std::vector<char>& buffer)
{
    if(buffer.size() != PlayerReadyMessage::size)
    {
        return std::optional<AcknowledgePlayerReadyMessage>();
    }

    const uint16_t messageSize = (buffer[0] << CHAR_BIT) | buffer[1];
    const uint16_t messageType = (buffer[2] << CHAR_BIT) | buffer[3];

    if(messageSize != AcknowledgePlayerReadyMessage::size || messageType != AcknowledgePlayerReadyMessage::type)
    {
        return std::optional<AcknowledgePlayerReadyMessage>();
    }

    return AcknowledgePlayerReadyMessage{};
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

std::optional<PlayedCardMessage> PlayedCardMessage::deserialize(const std::vector<char>& buffer)
{
    if(buffer.size() != PlayedCardMessage::size)
    {
        return std::optional<PlayedCardMessage>();
    }

    const uint16_t MessageSize = (buffer[0] << CHAR_BIT) | buffer[1];
    const uint16_t messageType = (buffer[2] << CHAR_BIT) | buffer[3];

    if(MessageSize != PlayedCardMessage::size || messageType != PlayedCardMessage::type)
    {
        return std::optional<PlayedCardMessage>();
    }

    const auto suit = static_cast<CardSuit>(buffer[4]);
    const auto number = static_cast<CardNumber>(buffer[5]);

    return PlayedCardMessage(suit, number);
}

//NOTE: These should eventually be moved to another file (message operations.*?)
std::string MessageToString::operator()(const PlayerReadyMessage& message)
{
    return "Player reader";
}

std::string MessageToString::operator()(const AcknowledgePlayerReadyMessage& message)
{
    return "Acknowledge player ready";
}

std::string MessageToString::operator()(const PlayedCardMessage& message)
{
    return "Played Card: " + toString(message.number) + " of " + toString(message.suit);
}

MessageId ExtractId::operator()(const PlayerReadyMessage& message)
{
    return static_cast<MessageId>(message.type);
}

MessageId ExtractId::operator()(const AcknowledgePlayerReadyMessage& message)
{
    return static_cast<MessageId>(message.type);
}

MessageId ExtractId::operator()(const PlayedCardMessage& message)
{
    return static_cast<MessageId>(message.type);
}
