#include "card.h"

//----------------------------------------------------------------------------
// Public function implementation
//----------------------------------------------------------------------------

bool card_IsRed(Card_t card)
{
  return (card.m_color == CC_HEARTS) || (card.m_color == CC_DIAMONDS);
}

bool card_IsOneLess(Card_t card1, Card_t card2)
{
  return (card1.m_type + 1) == card2.m_type;
}

bool card_IsOppositeColor(Card_t card1, Card_t card2)
{
  // note - negations are necessary to "normalize" the values
  return !card_IsRed(card1) != !card_IsRed(card2);
}

void card_Swap(Card_t* card1Ptr, Card_t* card2Ptr)
{
  if (card1Ptr && card2Ptr) {
    Card_t card = *card1Ptr;
    *card1Ptr = *card2Ptr;
    *card2Ptr = card;
  }
}
