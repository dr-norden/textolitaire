#include "table.h"

struct SolStack pack;
struct SolStack rest;
struct SolStack stacks[STACKS];
struct SolStack colStacks[cc_diamonds+1];

void initTable() {
    initFullStack(&pack);
    initStack(&rest);
    initStacks(stacks, STACKS);
    initStacks(colStacks, COLORS);

    // draw cards from the pack into desk
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j <= i; j++) {
            struct Card card;
            popStack(&pack, &card);
            pushStack(&stacks[i], card);
        }
        topCard(&stacks[i])->m_down = false;
    }
}


void nextCard() {
    struct Card card;
    if (popStack(&pack, &card) != NULL) {
        card.m_down = false;
        pushStack(&rest, card);
    } else {
        while (popStack(&rest, &card) != NULL) {
            card.m_down = true;
            pushStack(&pack, card);
        }
    }
}


/* Move top of given stack into color stack if possible
 * Return true on success. 
 */
bool moveToColors(struct SolStack *pSource) {
    struct Card *pSrcCard = topCard(pSource);

    if (pSrcCard) {
        struct SolStack *pTarget = &colStacks[pSrcCard->m_color];
        struct Card *pTgtCard = topCard(pTarget);
        if ((pTgtCard && oneLess(*pTgtCard, *pSrcCard)) ||
            (!pTgtCard && pSrcCard->m_type == ct_ace)) {
            return moveCard(pSource, pTarget);
        }
    }

    return false;
}


/* Move top of given stack into desk stack with given number if possible.
 * Return true on success.
 */
bool moveToDesk(struct SolStack *pSource, int stackNum) {
    struct Card *pSrcCard = topCard(pSource);

    if (pSrcCard) {
        struct SolStack *pTarget = &stacks[stackNum];
        struct Card *pTgtCard = topCard(pTarget);
        if ((!pTgtCard && pSrcCard->m_type == ct_king) ||
            (pTgtCard && oneLess(*pSrcCard, *pTgtCard) && oppositeColor(*pSrcCard, *pTgtCard))) {
            return moveCard(pSource, pTarget);
        }
    }

    return false;
}

bool movePackToColors() {
    return moveToColors(&rest);
}

bool movePackToDesk(int stackNum) {
    return moveToDesk(&rest, stackNum);
}

bool moveDeskToColors(int stackNum) {
    return moveToColors(&stacks[stackNum]);
}

bool moveColorsToDesk(int srcNum, int tgtNum) {
    return moveToDesk(&colStacks[srcNum], tgtNum);
}


bool moveDeskToDesk(int srcNum, int tgtNum) {
    struct SolStack *pSource = &stacks[srcNum];
    struct SolStack *pTarget = &stacks[tgtNum];
    struct SolStack helpStack;
    struct Card helpCard;
    struct Card *pTgtCard = topCard(pTarget);
    struct Card *pSrcCard = NULL;
    int i;

    // try to find card facing up in the source column that would fit on target card
    for (i = 0; i < pSource->m_size; i++) {
        pSrcCard = &pSource->m_cards[i];
        if (!pSrcCard->m_down && (
            (pTgtCard && oneLess(*pSrcCard, *pTgtCard) && oppositeColor(*pSrcCard, *pTgtCard)) ||
            (!pTgtCard && pSrcCard->m_type == ct_king))) {
            break;    // got it
        }
        pSrcCard = NULL;
    }

    if (!pSrcCard) {
        return false;
    }

    initStack(&helpStack);

    while (i < pSource->m_size) {
        if (popStack(pSource, &helpCard)) {
            pushStack(&helpStack, helpCard);
        }
    }

    while (popStack(&helpStack, &helpCard) != NULL) {
        pushStack(pTarget, helpCard);
    }

    topUp(pSource);

    return true;
}


bool isVictory() {
    for (enum CardColor cc = cc_spades; cc <= cc_diamonds; cc++) {
        struct Card *pCard = topCard(&colStacks[cc]);
        if (!pCard || pCard->m_type != ct_king) {
            return false;
        }
    }
    return true;
}

struct SolStack * getPack() {
    return &pack;
}

struct SolStack * getRest() {
    return &rest;
}

struct SolStack * getColStacks() {
    return colStacks;
}

struct SolStack * getDeskStacks() {
    return stacks;
}
