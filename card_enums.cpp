#include "card_enums.h"

#include <stdexcept>

std::string toString(CardSuit suit)
{
    switch(suit)
    {
        case CardSuit::Heart:     return "hearts";
        case CardSuit::Club:      return "clubs";
        case CardSuit::Diamond:   return "diamonds";
        case CardSuit::Spade:     return "spades";
    }
}

std::string toString(CardNumber number)
{
    switch(number)
    {
        case CardNumber::Two:     return "2";
        case CardNumber::Three:   return "3";
        case CardNumber::Four:    return "4";
        case CardNumber::Five:    return "5";
        case CardNumber::Six:     return "6";
        case CardNumber::Seven:   return "7";
        case CardNumber::Eight:   return "8";
        case CardNumber::Nine:    return "9";
        case CardNumber::Ten:     return "10";
        case CardNumber::Jack:    return "jack";
        case CardNumber::Queen:   return "queen";
        case CardNumber::King:    return "king";
        case CardNumber::Ace:     return "ace";
    }
}
