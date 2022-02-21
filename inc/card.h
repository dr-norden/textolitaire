#ifndef __CARD_H__
#define __CARD_H__

#include <stdbool.h>

/* Card types - ordered by value */
typedef enum
{
  CT_NONE = 0,
  CT_ACE,
  CT_TWO,
  CT_THREE,
  CT_FOUR,
  CT_FIVE,
  CT_SIX,
  CT_SEVEN,
  CT_EIGHT,
  CT_NINE,
  CT_TEN,
  CT_JACK,
  CT_QUEEN,
  CT_KING,
  NUM_CARD_TYPES
} ECardType_t;

/* Card colors */
typedef enum
{
  CC_SPADES = 0, // black
  CC_HEARTS,     // red
  CC_CLUBS,      // black
  CC_DIAMONDS,   // red
  NUM_CARD_COLORS
} ECardColor_t;

/* Directions a card can be facing */
typedef enum
{
  CF_DOWN = 0,
  CF_UP
} ECardFace_t;

/* Card - defined by type color and facing (up or down) */
typedef struct
{
  ECardType_t m_type;
  ECardColor_t m_color;
  ECardFace_t m_face;
} Card_t;

/* Return true if card is red (hearts or diamonds) */
bool card_IsRed(Card_t card);

/* Return true, if card1 is one less than card2 in type order */
bool card_IsOneLess(Card_t card1, Card_t card2);

/* Return true if one card is red while the other card is black */
bool card_IsOppositeColor(Card_t card1, Card_t card2);

/* Swap the attributes of given Card structures */
void card_Swap(Card_t* card1Ptr, Card_t* card2Ptr);

#endif
