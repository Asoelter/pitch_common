#include "message_decoder.h"

#include <cassert>
#include <stdexcept>

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

    while(rawMessageBuffer_.size() >= minMessageSize)
    {
        const uint16_t messageSize = (rawMessageBuffer_[0] << CHAR_BIT) | (rawMessageBuffer_[1]);
        const uint16_t rawMessageId = (rawMessageBuffer_[2] << CHAR_BIT) | (rawMessageBuffer_[3]);
        const auto messageId = static_cast<MessageId>(rawMessageId);
        const auto messageBegin = rawMessageBuffer_.begin();
        const auto messageEnd= rawMessageBuffer_.begin() + messageSize;
        const auto exactMessageBuffer = std::vector<char>(messageBegin, messageEnd);
        auto bytesRead = 0;

        #define DESERIALIZE_MESSAGE(msgName)                                                            \
            case MessageId::msgName:                                                                    \
            {                                                                                           \
                const auto possibleDecodedMessage = msgName##Message::deserialize(exactMessageBuffer);  \
                if(possibleDecodedMessage)                                                              \
                {                                                                                       \
                    messageBuffer_.push(possibleDecodedMessage.value());                                \
                }                                                                                       \
                else                                                                                    \
                {                                                                                       \
                    throw std::runtime_error("unable to deserialize buffer");                           \
                }                                                                                       \
                                                                                                        \
                bytesRead = msgName##Message::size;                                                     \
            }break;                                                                                     \

        switch(messageId)
        {
            DESERIALIZE_MESSAGE(PlayerReady);
            DESERIALIZE_MESSAGE(AcknowledgePlayerReady);
            DESERIALIZE_MESSAGE(PlayedCard);
            DESERIALIZE_MESSAGE(PromptBid);
            DESERIALIZE_MESSAGE(Bid);
            default: throw std::runtime_error("unknown message type");
        }

        rawMessageBuffer_.erase(rawMessageBuffer_.begin(), rawMessageBuffer_.begin() + bytesRead);
    }
}

#undef DESERIALIZE_MESSAGE
