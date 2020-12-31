#ifndef CARD_ENUMS_H
#define CARD_ENUMS_H

#define DECLARE_CARD_ENUMS          \
enum class Suit : unsigned char     \
{                                   \
    Heart,                          \
    Club,                           \
    Diamond,                        \
    Spade                           \
};                                  \
                                    \
enum class Number : unsigned char   \
{                                   \
    Two, Three, Four,  Five,        \
    Six, Seven, Eight, Nine,        \
    Ten, Jack,  Queen, King,        \
    Ace                             \
};                                  \

#endif //CARD_ENUMS_H
