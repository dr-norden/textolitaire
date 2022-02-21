#include "table.h"
#include "display.h"
#include <unistd.h>

//----------------------------------------------------------------------------
// Private definitions/variables
//----------------------------------------------------------------------------

#define MIN_SCORE -2000000
#define TURBO 1

static SolStack_t PackStack;
static SolStack_t RestStack;
static SolStack_t DeskStacks[NUM_STACKS];
static SolStack_t ColorStacks[NUM_CARD_COLORS];
static int Score = 0;

//----------------------------------------------------------------------------
// Private function prototypes
//----------------------------------------------------------------------------

/* Update score by given numbers - negative score cannot go below MIN_SCORE */
static void UpdateScore(int num);

/* Return true if the card can be moved on top of given desk stack. */
static bool IsCardMovableToDesk(Card_t card, const SolStack_t* stackPtr);

/* Return true if the card can be moved on top of its color stack */
static bool IsCardMovableToColors(Card_t card);

/*
 * Move top of given stack into color stack if possible
 * Return true on success.
 */
static bool MoveToColors(SolStack_t* srcStackPtr);

/*
 * Move top of given stack into desk stack with given number if possible.
 * Return true on success.
 */
static bool MoveToDesk(SolStack_t* srcStackPtr, int stackIdx);

//----------------------------------------------------------------------------
// Public function implementation
//----------------------------------------------------------------------------

void tbl_Init(void)
{
  stk_InitFull(&PackStack);
  stk_InitEmpty(&RestStack);
  stk_InitEmptyArray(DeskStacks, NUM_STACKS);
  stk_InitEmptyArray(ColorStacks, NUM_CARD_COLORS);
  Score = 0;

  // draw cards from the pack into desk
  for (int i = 0; i < NUM_STACKS; i++) {
    for (int j = 0; j <= i; j++) {
      Card_t card;
      stk_Pop(&PackStack, &card);
      stk_PushCopy(&DeskStacks[i], card);
    }
    stk_TopCard(&DeskStacks[i])->m_face = CF_UP;
  }
}

bool tbl_IsVictory(void)
{
  for (ECardColor_t cc = CC_SPADES; cc <= CC_DIAMONDS; cc++) {
    const Card_t* cardPtr = stk_CTopCard(&ColorStacks[cc]);
    if (!cardPtr || cardPtr->m_type != CT_KING) {
      return false;
    }
  }
  return true;
}

void tbl_DrawNextCard(void)
{
  Card_t card;
  if (stk_Pop(&PackStack, &card)) {
    stk_TurnTopCardDown(&RestStack);
    card.m_face = CF_UP;
    stk_PushCopy(&RestStack, card);
  } else {
    while (stk_Pop(&RestStack, &card)) {
      card.m_face = CF_DOWN;
      stk_PushCopy(&PackStack, card);
      UpdateScore(-2);
    }
  }
}

bool tbl_MovePackToColors(void)
{
  if (MoveToColors(&RestStack)) {
    UpdateScore(10);
    return true;
  }
  return false;
}

bool tbl_MovePackToDesk(int stackIdx)
{
  if (MoveToDesk(&RestStack, stackIdx)) {
    UpdateScore(5);
    return true;
  }
  return false;
}

bool tbl_MoveDeskToColors(int stackIdx)
{
  if (MoveToColors(&DeskStacks[stackIdx])) {
    UpdateScore(10);
    return true;
  }
  return false;
}

bool tbl_MoveColorsToDesk(int sourceIdx, int targetIdx)
{
  if (MoveToDesk(&ColorStacks[sourceIdx], targetIdx)) {
    UpdateScore(-15);
    return true;
  }
  return false;
}

bool tbl_IsRevealingDeskMove(int sourceIdx, int targetIdx)
{
  SolStack_t* srcStackPtr = &DeskStacks[sourceIdx];
  SolStack_t* targetPtr = &DeskStacks[targetIdx];
  Card_t* srcCardPtr = NULL;

  // find the first card facing up in the source column
  for (int i = 0; i < srcStackPtr->m_size; i++) {
    if (srcStackPtr->m_cards[i].m_face == CF_UP) {
      srcCardPtr = &srcStackPtr->m_cards[i];
      // don't move king if there's no card under it
      if (i == 0 && srcCardPtr->m_type == CT_KING) {
        srcCardPtr = NULL;
      }
      break;
    }
  }

  // verify whether found card it would fit on target card
  return (srcCardPtr && IsCardMovableToDesk(*srcCardPtr, targetPtr));
}

bool tbl_MoveDeskToDesk(int sourceIdx, int targetIdx)
{
  SolStack_t* srcStackPtr = &DeskStacks[sourceIdx];
  SolStack_t* targetPtr = &DeskStacks[targetIdx];
  SolStack_t helpStack;
  Card_t helpCard;
  Card_t* srcCardPtr = NULL;
  int srcCardIdx;

  // try to find card facing up in the source column that would fit on target
  // card or empty slot
  for (srcCardIdx = 0; srcCardIdx < srcStackPtr->m_size; srcCardIdx++) {
    srcCardPtr = &srcStackPtr->m_cards[srcCardIdx];
    if (srcCardPtr->m_face == CF_UP &&
        IsCardMovableToDesk(*srcCardPtr, targetPtr)) {
      break; // got it
    }
    srcCardPtr = NULL;
  }

  if (!srcCardPtr) {
    return false;
  }

  // move cards from one stack to the other
  stk_InitEmpty(&helpStack);

  while (srcCardIdx < srcStackPtr->m_size) {
    if (stk_Pop(srcStackPtr, &helpCard)) {
      stk_PushCopy(&helpStack, helpCard);
    }
  }

  while (stk_Pop(&helpStack, &helpCard)) {
    stk_PushCopy(targetPtr, helpCard);
  }

  stk_TurnTopCardUp(srcStackPtr);

  return true;
}

const SolStack_t* tbl_GetPack(void)
{
  return &PackStack;
}

const SolStack_t* tbl_GetRest(void)
{
  return &RestStack;
}

const SolStack_t* tbl_GetColorStacks(void)
{
  return ColorStacks;
}

const SolStack_t* tbl_GetDeskStacks(void)
{
  return DeskStacks;
}

int tbl_GetCurrentScore(void)
{
  return Score;
}

//----------------------------------------------------------------------------
// Private function implementation
//----------------------------------------------------------------------------

static void UpdateScore(int num)
{
  Score += num;
  if (Score < MIN_SCORE) {
    Score = MIN_SCORE;
  }
}

static bool IsCardMovableToDesk(Card_t card, const SolStack_t* stackPtr)
{
  if (stackPtr) {
    const Card_t* topCardPtr = stk_CTopCard(stackPtr);
    if (topCardPtr) {
      return (card_IsOneLess(card, *topCardPtr) &&
              card_IsOppositeColor(card, *topCardPtr));
    } else {
      return (card.m_type == CT_KING);
    }
  }
  return false;
}

static bool IsCardMovableToColors(Card_t card)
{
  SolStack_t* targetPtr = &ColorStacks[card.m_color];
  const Card_t* topCardPtr = stk_CTopCard(targetPtr);

  if (topCardPtr) {
    return card_IsOneLess(*topCardPtr, card);
  } else {
    return (card.m_type == CT_ACE);
  }
}

static bool MoveToColors(SolStack_t* srcStackPtr)
{
  const Card_t* srcCardPtr = stk_CTopCard(srcStackPtr);

  if (srcCardPtr && IsCardMovableToColors(*srcCardPtr)) {
    return stk_MoveCardFromTo(srcStackPtr, &ColorStacks[srcCardPtr->m_color]);
  }
  return false;
}

static bool MoveToDesk(SolStack_t* srcStackPtr, int stackIdx)
{
  const Card_t* srcCardPtr = stk_CTopCard(srcStackPtr);
  SolStack_t* targetPtr = &DeskStacks[stackIdx];

  if (srcCardPtr && IsCardMovableToDesk(*srcCardPtr, targetPtr)) {
    return stk_MoveCardFromTo(srcStackPtr, targetPtr);
  }
  return false;
}
