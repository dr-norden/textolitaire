#ifndef __SOLSTACK_H__
#define __SOLSTACK_H__

#include "card.h"
#include <stdbool.h>
#include <stdlib.h>

#define SOL_STACK_MAX 52

/* Stack big enough to hold all playable cards. */
typedef struct
{
  size_t m_size;
  Card_t m_cards[SOL_STACK_MAX];
} SolStack_t;

/* Initialize an empty stack */
void stk_InitEmpty(SolStack_t* stackPtr);

/* Initialize an array of empty stacks */
void stk_InitEmptyArray(SolStack_t stacks[], size_t size);

/* Init stack with all playing cards */
void stk_InitFull(SolStack_t* stackPtr);

/* Return top card from the stack, or NULL if the stack is empty */
Card_t* stk_TopCard(SolStack_t* stackPtr);
const Card_t* stk_CTopCard(const SolStack_t* stackPtr);

/*
 * Add new card at the top of the stack, copy the attributes from given card.
 * Return true if the operation was successful.
 */
bool stk_PushCopy(SolStack_t* stackPtr, Card_t card);

/*
 * Remove the top card of the stack and copy its attributes to given structure.
 * Return true if the operation was successful.
 */
bool stk_Pop(SolStack_t* stackPtr, Card_t* cardPtr);

/*
 * Move top card from source stack to target stack.
 * Turn the new top card on the source stack facing up.
 * Return true if the operation was successful.
 */
bool stk_MoveCardFromTo(SolStack_t* sourcePtr, SolStack_t* targetPtr);

/* Turn the top card of the stack to face up */
void stk_TurnTopCardUp(SolStack_t* stackPtr);

/* Turn the top card of the stack to face down */
void stk_TurnTopCardDown(SolStack_t* stackPtr);

#endif
