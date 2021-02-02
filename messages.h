#ifndef MESSAGES_H
#define MESSAGES_H

#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "bid.h"
#include "card_enums.h"

namespace
{
    using StandardChunkType = uint16_t;
    constexpr size_t headerSize     = 2 * sizeof(StandardChunkType);
}

enum class MessageId : uint16_t
{
#define DECLARE_MESSAGE_ENUM(msgType) msgType##,
#define X(msgType) DECLARE_MESSAGE_ENUM(msgType)
    #include "messages.inc"
#undef X
#undef DECLARE_MESSAGE_ENUM
};

struct PlayerReadyMessage
{
    std::vector<char> serialize() const;
    static std::optional<PlayerReadyMessage> deserialize(const std::vector<char>& buffer);

    static constexpr StandardChunkType size = headerSize;
    static constexpr StandardChunkType type = static_cast<uint16_t>(MessageId::PlayerReady);
};

struct AcknowledgePlayerReadyMessage
{
    std::vector<char> serialize() const;
    static std::optional<AcknowledgePlayerReadyMessage> deserialize(const std::vector<char>& buffer);

    static constexpr StandardChunkType size = headerSize;
    static constexpr StandardChunkType type = static_cast<uint16_t>(MessageId::AcknowledgePlayerReady);
};

struct PromptPlayCardMessage
{
    std::vector<char> serialize() const;
    static std::optional<PromptPlayCardMessage> deserialize(const std::vector<char>& buffer);

    static constexpr StandardChunkType size = headerSize;
    static constexpr StandardChunkType type = static_cast<uint16_t>(MessageId::PromptPlayCard);
};

struct PlayedCardMessage
{
    PlayedCardMessage(CardSuit s, CardNumber n) : suit(s), number(n) {}

    std::vector<char> serialize() const;
    static std::optional<PlayedCardMessage> deserialize(const std::vector<char>& buffer);

    static constexpr StandardChunkType size = headerSize + sizeof(CardSuit) + sizeof(CardNumber);
    static constexpr StandardChunkType type = static_cast<uint16_t>(MessageId::PlayedCard);

    CardSuit   suit;
    CardNumber number;
};

struct PromptBidMessage
{
    std::vector<char> serialize() const;
    static std::optional<PromptBidMessage> deserialize(const std::vector<char>& buffer);

    static constexpr StandardChunkType size = headerSize;
    static constexpr StandardChunkType type = static_cast<uint16_t>(MessageId::PromptBid);
};

struct BidMessage
{
    BidMessage(Bid b) : bid(b) {}

    std::vector<char> serialize() const;
    static std::optional<BidMessage> deserialize(const std::vector<char>& buffer);

    static constexpr StandardChunkType size = headerSize + sizeof(Bid);
    static constexpr StandardChunkType type = static_cast<uint16_t>(MessageId::PromptBid);

    Bid bid;
};

//NOTE: These should eventually be moved to another file (message operations.*?)
struct MessageToString
{
#define DECLARED_MESSAGE_TO_STRING(msgType) std::string operator()(const msgType##Message& message);
#define X(msgType) DECLARED_MESSAGE_TO_STRING(msgType)
    #include "messages.inc"
#undef DECLARED_MESSAGE_TO_STRING
#undef X
};

struct ExtractId
{
#define DECLARED_EXTRACT_ID(msgType) MessageId operator()(const msgType##Message& message);
#define X(msgType) DECLARED_EXTRACT_ID(msgType)
    #include "messages.inc"
#undef DECLARED_EXTRACT_ID
#undef X
};

using Message = std::variant<
#define EXCLUDE_LAST
#define ADD_TO_MESSAGE_VARIANT(msgType) msgType##Message,
#define X(msgType) ADD_TO_MESSAGE_VARIANT(msgType)
    #include "messages.inc"
#undef EXCLUDE_LAST
#undef ADD_TO_MESSAGE_VARIANT
#undef X

#define EXCLUDE_ALL_BUT_LAST
#define ADD_TO_MESSAGE_VARIANT(msgType) msgType##Message
#define X(msgType) ADD_TO_MESSAGE_VARIANT(msgType)
    #include "messages.inc"
#undef EXCLUDE_ALL_BUT_LAST
#undef ADD_TO_MESSAGE_VARIANT
#undef X
>;

#if 0
using Message = std::variant<
    PlayerReadyMessage,
    AcknowledgePlayerReadyMessage,
    PlayedCardMessage,
    PromptBidMessage,
    BidMessage
>;
#endif

#endif //MESSAGES_H
