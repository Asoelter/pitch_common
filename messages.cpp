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

template<typename MessageType>
bool verifyHeader(const std::vector<char>& buffer)
{
    if(buffer.size() != MessageType::size)
    {
        return false;
    }

    const uint16_t size = (buffer[0] << CHAR_BIT) | buffer[1];
    const uint16_t messageType = (buffer[2] << CHAR_BIT) | buffer[3];

    if(size != MessageType::size || messageType != MessageType::type)
    {
        return false;
    }

    return true;
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

std::vector<char> PromptPlayCardMessage::serialize() const
{
    return serializeHeader<PromptPlayCardMessage>();
}

std::optional<PromptPlayCardMessage> PromptPlayCardMessage::deserialize(const std::vector<char>& buffer)
{
    return deserializeHeaderOnlyMessage<PromptPlayCardMessage>(buffer);
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

std::vector<char> BidMessage::serialize() const
{
    static_assert(sizeof(Bid) == 1, "Bid enum has gotten too large");

    auto buffer = serializeHeader<BidMessage>();
    buffer.push_back(static_cast<char>(bid));

    return buffer;
}

std::optional<BidMessage> BidMessage::deserialize(const std::vector<char>& buffer)
{
    if(!verifyHeader<BidMessage>(buffer))
    {
        return std::optional<BidMessage>();
    }

    const auto possibleBid = static_cast<uint8_t>(buffer.back());

    if(possibleBid < static_cast<uint8_t>(Bid::Two) || possibleBid > static_cast<uint8_t>(Bid::Shoot))
    {
        return std::optional<BidMessage>();
    }

    const auto bid = static_cast<Bid>(buffer.back());

    return BidMessage(bid);
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

std::string MessageToString::operator()(const PromptPlayCardMessage& message)
{
    return "Prompt play card";
}

std::string MessageToString::operator()(const PlayedCardMessage& message)
{
    return "Played Card: " + toString(message.number) + " of " + toString(message.suit);
}

std::string MessageToString::operator()(const PromptBidMessage& message)
{
    return "Prompt bid message";
}

std::string MessageToString::operator()(const BidMessage& message)
{
    return "Bid message";
}

#define IMPLEMENT_EXTRACT_ID(msgType)                                   \
    MessageId ExtractId::operator()(const msgType##Message& message)    \
    {                                                                   \
        return static_cast<MessageId>(message.type);                    \
    }                                                                   \

#define X(msgType) IMPLEMENT_EXTRACT_ID(msgType)
#   include "messages.inc"
#undef IMPLEMENT_EXTRACT_ID
#undef X
    
