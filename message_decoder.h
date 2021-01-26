#ifndef MESSAGE_DECODER_H
#define MESSAGE_DECODER_H

#include <optional>
#include <queue>
#include <vector>

#include "messages.h"

class MessageDecoder
{
public:
    MessageDecoder() = default;
    MessageDecoder(const MessageDecoder&) = default;
    MessageDecoder(MessageDecoder&&) = default;
    ~MessageDecoder() = default;

    MessageDecoder& operator=(const MessageDecoder&) = default;
    MessageDecoder& operator=(MessageDecoder&&) = default;

    void addMessageChunk(const std::vector<char>& buffer) noexcept;
    std::optional<Message> popMessage() noexcept;

private:
    std::vector<char>   rawMessageBuffer_;
    std::queue<Message> messageBuffer_;
};

#endif //MESSAGE_DECODER_H
