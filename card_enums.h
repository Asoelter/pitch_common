#ifndef CARD_ENUMS_H
#define CARD_ENUMS_H

enum class CardSuit : unsigned char
{
    Heart,
    Club,
    Diamond,
    Spade
};

enum class CardNumber : unsigned char
{
    Two, Three, Four,  Five,
    Six, Seven, Eight, Nine,
    Ten, Jack,  Queen, King,
    Ace
};

#endif //CARD_ENUMS_H
