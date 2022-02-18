#ifndef __SOLSTACK_H__
#define __SOLSTACK_H__

#include <stdbool.h>
#include <stdlib.h>

#include "card.h"

#define SOL_STACK_MAX 52

/* Stack big enough to hold all playable cards. */
struct SolStack {
    size_t      m_size;
    struct Card m_cards[SOL_STACK_MAX];
};


/* Initialize an empty stack */
void initStack(struct SolStack *pStack);


/* Initialize an array of empty stacks */
void initStacks(struct SolStack stacks[], size_t size);


/* Init stack with all playing cards */
void initFullStack(struct SolStack *pStack);


/* Randomly shuffle cards on given stack */
void shuffleStack(struct SolStack *pStack);


/* Return top card from the stack, or NULL if the stack is empty */
struct Card * topCard(struct SolStack *pStack);


/* Add new card with given attributes at the top of the stack.
 * Returns the new top card or NULL if the stack was full and no card was added.
 */
struct Card * newCard(struct SolStack *pStack, enum CardColor color,
                      enum CardType type);


/* Add new card at the top of the stack, copy the attributes from given card.
 * Returns the new top card or NULL if the stack was full and no card was added.
 */
struct Card * pushStack(struct SolStack *pStack, struct Card card);


/* Remove the top card of the stack and copy its attributes to given Card structure.
 * Returns pointer to given structure or NULL if no card was removed.
 */
struct Card * popStack(struct SolStack *pStack, struct Card *pCard);


/* Move top card from source stack to target stack. Turn the new top card
 * on the source stack facing up. Returns new top card from the target stack
 * or NULL if the operation was not successful.
 */
struct Card * moveCard(struct SolStack *pSource, struct SolStack *pTarget);


/* Turn the top card of the stack facing up */
void topUp(struct SolStack *pStack);


/* Turn the top card of the stack facing down */
void topDown(struct SolStack *pStack);



#endif
