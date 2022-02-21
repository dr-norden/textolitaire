#include "solstack.h"
#include <string.h>
#include <time.h>

//----------------------------------------------------------------------------
// Private function prototypes
//----------------------------------------------------------------------------

static void PushNew(SolStack_t* stackPtr, ECardColor_t color, ECardType_t type);
static void Shuffle(SolStack_t* stackPtr);

//----------------------------------------------------------------------------
// Public function implementation
//----------------------------------------------------------------------------

void stk_InitEmpty(SolStack_t* stackPtr)
{
  if (stackPtr) {
    stackPtr->m_size = 0;
    memset(stackPtr->m_cards, 0, sizeof(stackPtr->m_cards));
  }
}

void stk_InitEmptyArray(SolStack_t stacks[], size_t size)
{
  for (int i = 0; i < size; i++) {
    stk_InitEmpty(&stacks[i]);
  }
}

void stk_InitFull(SolStack_t* stackPtr)
{
  if (stackPtr) {
    stk_InitEmpty(stackPtr);
    // fill the stack with all combinations of color and type
    for (ECardColor_t cc = CC_SPADES; cc <= CC_DIAMONDS; cc++) {
      for (ECardType_t ct = CT_ACE; ct <= CT_KING; ct++) {
        PushNew(stackPtr, cc, ct);
      }
    }

    Shuffle(stackPtr);
  }
}

Card_t* stk_TopCard(SolStack_t* stackPtr)
{
  if (stackPtr && stackPtr->m_size > 0) {
    return &stackPtr->m_cards[stackPtr->m_size - 1];
  }
  return NULL;
}

const Card_t* stk_CTopCard(const SolStack_t* stackPtr)
{
  return stk_TopCard((SolStack_t*)stackPtr);
}

bool stk_PushCopy(SolStack_t* stackPtr, Card_t card)
{
  if (stackPtr->m_size < SOL_STACK_MAX) {
    stackPtr->m_size++;
    *stk_TopCard(stackPtr) = card;
    return true;
  }
  return false;
}

bool stk_Pop(SolStack_t* stackPtr, Card_t* cardPtr)
{
  if (cardPtr && stackPtr && stackPtr->m_size > 0) {
    *cardPtr = *stk_TopCard(stackPtr);
    stackPtr->m_size--;
    return true;
  }
  return NULL;
}

bool stk_MoveCardFromTo(SolStack_t* sourcePtr, SolStack_t* targetPtr)
{
  if (sourcePtr && targetPtr) {
    Card_t card;
    if (stk_Pop(sourcePtr, &card)) {
      if (stk_PushCopy(targetPtr, card)) {
        stk_TurnTopCardUp(sourcePtr);
        return true;
      } else {
        // target stack is probably full, push the card back to source
        stk_PushCopy(sourcePtr, card);
        return false;
      }
    }
  }
  return false;
}

void stk_TurnTopCardUp(SolStack_t* stackPtr)
{
  if (stk_TopCard(stackPtr) != NULL) {
    stk_TopCard(stackPtr)->m_face = CF_UP;
  }
}

void stk_TurnTopCardDown(SolStack_t* stackPtr)
{
  if (stk_TopCard(stackPtr) != NULL) {
    stk_TopCard(stackPtr)->m_face = CF_DOWN;
  }
}

//----------------------------------------------------------------------------
// Private function implementation
//----------------------------------------------------------------------------

static void PushNew(SolStack_t* stackPtr, ECardColor_t color, ECardType_t type)
{
  if (stackPtr && stackPtr->m_size < SOL_STACK_MAX) {
    stackPtr->m_size++;
    stk_TopCard(stackPtr)->m_color = color;
    stk_TopCard(stackPtr)->m_type = type;
    stk_TopCard(stackPtr)->m_face = CF_DOWN;
  }
}

void Shuffle(SolStack_t* stackPtr)
{
  if (stackPtr) {
    srand(time(NULL));
    for (int i = stackPtr->m_size - 1; i >= 0; i--) {
      card_Swap(&stackPtr->m_cards[i], &stackPtr->m_cards[rand() % (i + 1)]);
    }
  }
}
