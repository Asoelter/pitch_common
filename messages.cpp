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

template<typename MessageType>
std::vector<char> serializeHeader()
{
    std::vector<char> result;
    result.reserve(MessageType::size);

    result.push_back(firstByte(MessageType::size));
    result.push_back(secondByte(MessageType::size));
    result.push_back(firstByte(MessageType::type));
    result.push_back(secondByte(MessageType::type));

    return result;
}

template<typename MessageType>
std::optional<MessageType> deserializeHeaderOnlyMessage(const std::vector<char>& buffer)
{
    if(buffer.size() != MessageType::size)
    {
        return std::optional<MessageType>();
    }

    const uint16_t size = (buffer[0] << CHAR_BIT) | buffer[1];
    const uint16_t messageType = (buffer[2] << CHAR_BIT) | buffer[3];

    if(size != MessageType::size || messageType != MessageType::type)
    {
        return std::optional<MessageType>();
    }

    return MessageType{};
}

std::vector<char> PlayerReadyMessage::serialize() const
{
    return serializeHeader<PlayerReadyMessage>();
}

std::optional<PlayerReadyMessage> PlayerReadyMessage::deserialize(const std::vector<char>& buffer)
{
    return deserializeHeaderOnlyMessage<PlayerReadyMessage>(buffer);
}

std::vector<char> AcknowledgePlayerReadyMessage::serialize() const
{
    return serializeHeader<AcknowledgePlayerReadyMessage>();
}

std::optional<AcknowledgePlayerReadyMessage> AcknowledgePlayerReadyMessage::deserialize(const std::vector<char>& buffer)
{
    return deserializeHeaderOnlyMessage<AcknowledgePlayerReadyMessage>(buffer);
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

std::vector<char> PromptBidMessage::serialize() const
{
    return serializeHeader<PromptBidMessage>();
}

std::optional<PromptBidMessage> PromptBidMessage::deserialize(const std::vector<char>& buffer)
{
    return deserializeHeaderOnlyMessage<PromptBidMessage>(buffer);
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

std::string MessageToString::operator()(const PromptBidMessage& message)
{
    return "Prompt bid message";
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

MessageId ExtractId::operator()(const PromptBidMessage& message)
{
    return static_cast<MessageId>(message.type);
}
