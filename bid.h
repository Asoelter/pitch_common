#ifndef BID_H
#define BID_H

#include <stdexcept>
#include <string>

enum class Bid : uint8_t
{
    Two,
    Three,
    Four,
    five,
    Six,
    Seven,
    Shoot
};

inline std::string toString(Bid bid)
{
    switch(bid)
    {
        case Bid::Two:      return "Two";
        case Bid::Three:    return "Three";
        case Bid::Four:     return "Four";
        case Bid::five:     return "Five";
        case Bid::Six:      return "Six";
        case Bid::Seven:    return "Seven";
        case Bid::Shoot:    return "Shoot";
    }

    throw std::invalid_argument("Unknown bid value: " + std::to_string(static_cast<uint8_t>(bid)));
}

#endif //BID_H
