#include "table.h"
#include "display.h"
#include <unistd.h>

#define MIN_SCORE -2000000

static struct SolStack pack;
static struct SolStack rest;
static struct SolStack stacks[STACKS];
static struct SolStack colStacks[cc_diamonds+1];
static enum Command activeCmd = cmd_none;
static int score = 0;

void initTable() {
    initFullStack(&pack);
    initStack(&rest);
    initStacks(stacks, STACKS);
    initStacks(colStacks, COLORS);
    activeCmd = cmd_none;
    score = 0;

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


enum Command getActiveCmd() {
    return activeCmd;
}


int getScore() {
    return score;
}

static void updateScore(int num) {
    score += num;
    if (score < MIN_SCORE) {
        score = MIN_SCORE;
    }
}


void nextCard() {
    struct Card card;
    if (popStack(&pack, &card) != NULL) {
        topDown(&rest);
        card.m_down = false;
        pushStack(&rest, card);
    } else {
        while (popStack(&rest, &card) != NULL) {
            card.m_down = true;
            pushStack(&pack, card);
            updateScore(-2);
        }
    }
}



/* Return true if the card can be moved on top of given desk stack. */
static bool isCardMovableToDesk(struct Card card, struct SolStack *pStack) {
    if (pStack) {
        struct Card *pTopCard = topCard(pStack);
        if (pTopCard) {
            return (oneLess(card, *pTopCard) && oppositeColor(card, *pTopCard));
        } else {
            return (card.m_type == ct_king);
        }
    }
    return false;
}

/* Return true if the card can be moved on top of its color stack */
static bool isCardMovableToColors(struct Card card) {
    struct SolStack *pTarget = &colStacks[card.m_color];
    struct Card *pTopCard = topCard(pTarget);

    if (pTopCard) {
        return oneLess(*pTopCard, card);
    } else {
        return (card.m_type == ct_ace);
    }
}

/* Move top of given stack into color stack if possible
 * Return true on success.
 */
static bool moveToColors(struct SolStack *pSource) {
    struct Card *pSrcCard = topCard(pSource);

    if (pSrcCard && isCardMovableToColors(*pSrcCard)) {
        return moveCard(pSource, &colStacks[pSrcCard->m_color]);
    }

    return false;
}


/* Move top of given stack into desk stack with given number if possible.
 * Return true on success.
 */
static bool moveToDesk(struct SolStack *pSource, int stackNum) {
    struct Card *pSrcCard = topCard(pSource);
    struct SolStack *pTarget = &stacks[stackNum];

    if (pSrcCard && isCardMovableToDesk(*pSrcCard, pTarget)) {
        return moveCard(pSource, pTarget);
    }

    return false;
}

bool movePackToColors() {
    if (moveToColors(&rest)) {
        updateScore(10);
        return true;
    }
    return false;
}

bool movePackToDesk(int stackNum) {
    if (moveToDesk(&rest, stackNum)) {
        updateScore(5);
        return true;
    }
    return false;
}

bool moveDeskToColors(int stackNum) {
    if (moveToColors(&stacks[stackNum])) {
        updateScore(10);
        return true;
    }
    return false;
}

bool moveColorsToDesk(int srcNum, int tgtNum) {
    if (moveToDesk(&colStacks[srcNum], tgtNum)) {
        updateScore(-15);
        return true;
    }
    return false;
}

/* Move chain of cards from one desk stack to another if possible.
 * If revealOnly is set to true, it only allows moves that reveal
 * a new card or vacate an empty space.
 * Return true on success.
 */
static bool moveDeskToDesk3(int srcNum, int tgtNum, bool revealOnly) {
    struct SolStack *pSource = &stacks[srcNum];
    struct SolStack *pTarget = &stacks[tgtNum];
    struct SolStack helpStack;
    struct Card helpCard;
    struct Card *pSrcCard = NULL;
    int i;

    if (revealOnly) {
        // find the first card facing up in the source column
        for (i = 0; i < pSource->m_size; i++) {
            if (!pSource->m_cards[i].m_down) {
                pSrcCard = &pSource->m_cards[i];
                // don't move king if there's no card under it
                if (i == 0 && pSrcCard->m_type == ct_king) {
                    pSrcCard = NULL;
                }
                break;
            }
        }

        // verify whether it would fit on target card
        if (pSrcCard && !isCardMovableToDesk(*pSrcCard, pTarget)) {
            pSrcCard = NULL;
        }
    } else {
        // try to find card facing up in the source column that would fit on target card
        for (i = 0; i < pSource->m_size; i++) {
            pSrcCard = &pSource->m_cards[i];
            if (!pSrcCard->m_down && isCardMovableToDesk(*pSrcCard, pTarget)) {
                break;    // got it
            }
            pSrcCard = NULL;
        }
    }

    if (!pSrcCard) {
        return false;
    }

    // move cards from one stack to the other
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

bool moveDeskToDesk(int srcNum, int tgtNum) {
    return moveDeskToDesk3(srcNum, tgtNum, false);
}


void moveAnything() {
    // try to move cards between desk stacks
    for (int i = 0; i < STACKS; i++) {
        for (int j = 0; j < STACKS; j++) {
            if ((i != j) && moveDeskToDesk3(i, j, true)) {
                return;
            }
        }
    }
    // try to move the pack card to any desk stack
    for (int i = 0; i < STACKS; i++) {
        if (movePackToDesk(i)) {
            return;
        }
    }

    // try to move any desk card to colors
    for (int i = 0; i < STACKS; i++) {
        if (moveDeskToColors(i)) {
            return;
        }
    }

    // try to move the pack card to colors
    if (movePackToColors()) {
        return;
    }

    // if everything else fails, draw next card
    nextCard();
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


bool controlTable(enum Command cmd) {
    bool rv = true;

    if (activeCmd == cmd_none) {
        if (cmd == cmd_next) {
            nextCard();
        } else if (cmd == cmd_pack || isCmdDesk(cmd) || isCmdColor(cmd)) {
            // want one more key to complete action
            activeCmd = cmd;
        } else if (cmd == cmd_lazy) {
            moveAnything();
            #ifndef TURBO
            sleep(1);
            #endif
        } else {
            return false;
        }

    } else {
        rv = false;

        if (isCmdColor(cmd) || cmd == activeCmd) {
            // move up or cancel action
            if (cmd_pack == activeCmd) {
                rv = movePackToColors();
            } else if (isCmdDesk(activeCmd)) {
                rv = moveDeskToColors(activeCmd - cmd_desk0);
            } else {
                rv = false;
            }
            rv = rv || activeCmd == cmd;

        } else if (isCmdDesk(cmd)) {
            // move to desk
            if (cmd_pack == activeCmd) {
                rv = movePackToDesk(cmd-cmd_desk0);
            } else if (isCmdDesk(activeCmd)) {
                rv = moveDeskToDesk(activeCmd - cmd_desk0, cmd - cmd_desk0);
            } else if (isCmdColor(activeCmd)) {
                rv = moveColorsToDesk(activeCmd - cmd_color0, cmd - cmd_desk0);
            }
        } else if (cmd_pack == cmd) {
            rv = false;
        } else {
            // unknown key
            activeCmd = cmd_none;
            return false;
        }

        activeCmd = cmd_none;
    }

    if (!rv) {
        setMessage("!! Invalid move");
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
