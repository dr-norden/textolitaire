#ifndef __CARD_H__
#define __CARD_H__

#include <stdbool.h>

/* Card types - ordered by value */
enum CardType {
    ct_none = 0,
    ct_ace,
    ct_two,
    ct_three,
    ct_four,
    ct_five,
    ct_six,
    ct_seven,
    ct_eight,
    ct_nine,
    ct_ten,
    ct_jack,
    ct_queen,
    ct_king
};


/* Card colors */
enum CardColor {
    cc_spades = 0,  // black
    cc_hearts,      // red
    cc_clubs,       // black
    cc_diamonds     // red
};


/* Card - defined by type color and side (face up or face down) */
struct Card {
    enum CardType  m_type;
    enum CardColor m_color;
    bool           m_down;
};


/* Return true if card is red (hearts or diamonds) */
bool isRed(struct Card card);


/* Return true, if card1 is one less than card2 in type order */
bool oneLess(struct Card card1, struct Card card2);


/* Return true if one card is red while the other card is black */
bool oppositeColor(struct Card card1, struct Card card2);


/* Swap the attributes of given Card structures */
void swapCard(struct Card *pCard1, struct Card *pCard2);


/* Return short text representation of card's type */
const char * typeMark(struct Card card);


/* Return short text representation of card's color */
const char * colorMark(struct Card card);


#endif
