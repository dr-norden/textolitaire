#include "solstack.h"
#include <string.h>
#include <time.h>


void initStack(struct SolStack *pStack) {
    if (pStack) {
        pStack->m_size = 0;
        memset(pStack->m_cards, 0, sizeof(pStack->m_cards));
    }
}


void initStacks(struct SolStack stacks[], size_t size) {
    for (int i = 0; i < size; i++) {
        initStack(&stacks[i]);
    }
}


struct Card * topCard(struct SolStack *pStack) {
    if (pStack && pStack->m_size > 0) {
        return &pStack->m_cards[pStack->m_size-1];
    }
    return NULL;
}


struct Card * newCard(struct SolStack *pStack, enum CardColor color,
                      enum CardType type) {

    if (pStack && pStack->m_size < SOL_STACK_MAX) {
        pStack->m_size++;
        topCard(pStack)->m_color = color;
        topCard(pStack)->m_type  = type;
        topCard(pStack)->m_down  = true;
        return topCard(pStack);
    }

    return NULL;
}


struct Card * pushStack(struct SolStack *pStack, struct Card card) {
    if (pStack->m_size < SOL_STACK_MAX) {
        pStack->m_size++;
        *topCard(pStack) = card;
        return topCard(pStack);
    }
    return NULL;
}


struct Card * popStack(struct SolStack *pStack, struct Card *pCard) {
    if (pCard && pStack && pStack->m_size > 0) {
        *pCard = *topCard(pStack);
        pStack->m_size--;
        return pCard;
    }
    return NULL;
}


void topUp(struct SolStack *pStack) {
    if (topCard(pStack) != NULL) {
        topCard(pStack)->m_down = false;
    }
}


void shuffleStack(struct SolStack *pStack) {
    if (pStack) {
        srand(time(NULL));
        for (int i = pStack->m_size-1; i >= 0; i--) {
            swapCard(&pStack->m_cards[i], &pStack->m_cards[rand()%(i+1)]);
        }
    }
}


void initFullStack(struct SolStack *pStack) {
    if (pStack) {
        initStack(pStack);
        // fill the stack with all combinations of color and type
        for (enum CardColor cc = cc_spades; cc <= cc_diamonds; cc++) {
            for (enum CardType ct = ct_ace; ct <= ct_king; ct++) {
                newCard(pStack, cc, ct);
            }
        }

        shuffleStack(pStack);
    }
}



struct Card * moveCard(struct SolStack *pSource, struct SolStack *pTarget) {
    if (pSource && pTarget) {
        struct Card card;
        if (popStack(pSource, &card) != NULL) {
            struct Card *pCard = pushStack(pTarget, card);
            if (pCard) {
                topUp(pSource);
                return pCard;
            } else {
                // target stack is probably full, push the card back to source
                pushStack(pSource, card);
                return NULL;
            }
        }
    }
    return NULL;
}


