#include "message_decoder.h"

void MessageDecoder::addMessageChunk(const std::vector<char>& buffer) noexcept
{
    rawMessageBuffer_.insert(rawMessageBuffer_.end(), buffer.begin(), buffer.end());

    constexpr auto minMessageSize = 4;

    if(rawMessageBuffer_.size() < minMessageSize)
    {
        return;
    }

    const uint16_t rawMessageId = (rawMessageBuffer_[0] << CHAR_BIT) | (rawMessageBuffer_[1]);
    const uint16_t messageSize = (rawMessageBuffer_[2] << CHAR_BIT) | (rawMessageBuffer_[3]);
    const auto messageId = static_cast<MessageId>(rawMessageId);
    const auto messageBegin = rawMessageBuffer_.begin();
    const auto messageEnd= rawMessageBuffer_.begin() + messageSize;
    const auto exactMessageBuffer = std::vector<char>(messageBegin, messageEnd);

    switch(messageId)
    {
        case MessageId::PlayerReady:
        {
            const auto possibleDecodedMessage = PlayerReadyMessage::deserialize(exactMessageBuffer);

            if(possibleDecodedMessage)
            {
                messageBuffer_.push(possibleDecodedMessage.value());
            }
        } break;
        case MessageId::PlayedCard:
        {
            const auto possibleDecodedMessage = PlayedCardMessage::deserialize(exactMessageBuffer);

            if(possibleDecodedMessage)
            {
                messageBuffer_.push(possibleDecodedMessage.value());
            }
        } break;
    }
}

std::optional<Message> MessageDecoder::popMessage() noexcept
{
    if(messageBuffer_.empty())
    {
        return std::optional<Message>();
    }

    const auto message = messageBuffer_.front();
    messageBuffer_.pop();
    return message;
}
