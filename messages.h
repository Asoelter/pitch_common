#ifndef MESSAGES_H
#define MESSAGES_H

#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "card_enums.h"

namespace
{
    using StandardChunkType = uint16_t;
    constexpr size_t headerSize     = 2 * sizeof(StandardChunkType);
}

enum class MessageId : uint16_t
{
    PlayerReady,
    AcknowledgePlayerReady,
    PlayedCard
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

//NOTE: These should eventually be moved to another file (message operations.*?)
struct MessageToString
{
    std::string operator()(const PlayerReadyMessage& message);
    std::string operator()(const AcknowledgePlayerReadyMessage& message);
    std::string operator()(const PlayedCardMessage& message);
};

struct ExtractId
{
    MessageId operator()(const PlayerReadyMessage& message);
    MessageId operator()(const AcknowledgePlayerReadyMessage& message);
    MessageId operator()(const PlayedCardMessage& message);
};

using Message = std::variant<PlayerReadyMessage, AcknowledgePlayerReadyMessage, PlayedCardMessage>;

#endif //MESSAGES_H
