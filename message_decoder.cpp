#include "message_decoder.h"

void MessageDecoder::addMessageChunk(const std::vector<char>& buffer) noexcept
{
    rawMessageBuffer_.insert(rawMessageBuffer_.end(), buffer.begin(), buffer.end());

    translateAllRawMessages();
}

std::optional<Message> MessageDecoder::popMessage() noexcept
{
    translateAllRawMessages();

    if(messageBuffer_.empty())
    {
        return std::optional<Message>();
    }

    const auto message = messageBuffer_.front();
    messageBuffer_.pop();
    return message;
}

void MessageDecoder::translateAllRawMessages()
{
    constexpr auto minMessageSize = 4;

    while(rawMessageBuffer_.size() > minMessageSize)
    {
        const uint16_t messageSize = (rawMessageBuffer_[0] << CHAR_BIT) | (rawMessageBuffer_[1]);
        const uint16_t rawMessageId = (rawMessageBuffer_[2] << CHAR_BIT) | (rawMessageBuffer_[3]);
        const auto messageId = static_cast<MessageId>(rawMessageId);
        const auto messageBegin = rawMessageBuffer_.begin();
        const auto messageEnd= rawMessageBuffer_.begin() + messageSize;
        const auto exactMessageBuffer = std::vector<char>(messageBegin, messageEnd);
        auto bytesRead = 0;

        switch(messageId)
        {
            case MessageId::PlayerReady:
            {
                const auto possibleDecodedMessage = PlayerReadyMessage::deserialize(exactMessageBuffer);

                if(possibleDecodedMessage)
                {
                    messageBuffer_.push(possibleDecodedMessage.value());
                }

                bytesRead = PlayerReadyMessage::size;

            } break;
            case MessageId::AcknowledgePlayerReady:
            {
                const auto possibleDecodedMessage = AcknowledgePlayerReadyMessage::deserialize(exactMessageBuffer);

                if(possibleDecodedMessage)
                {
                    messageBuffer_.push(possibleDecodedMessage.value());
                }

                bytesRead = AcknowledgePlayerReadyMessage::size;

            } break;
            case MessageId::PlayedCard:
            {
                const auto possibleDecodedMessage = PlayedCardMessage::deserialize(exactMessageBuffer);

                if(possibleDecodedMessage)
                {
                    messageBuffer_.push(possibleDecodedMessage.value());
                }

                bytesRead = PlayedCardMessage::size;

            } break;
        }

        rawMessageBuffer_.erase(rawMessageBuffer_.begin(), rawMessageBuffer_.begin() + bytesRead);
    }
}
