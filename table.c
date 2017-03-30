#include "table.h"
#include "display.h"

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

void updateScore(int num) {
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


bool controlTable(enum Command cmd) {
    if (activeCmd == cmd_none) {
        if (cmd == cmd_next) {
            nextCard();
        } else if (cmd == cmd_pack || isCmdDesk(cmd) || isCmdColor(cmd)) {
            // want one more key to complete action
            activeCmd = cmd;
        } else {
            return false;
        }
        return true;

    } else {
        bool rv = false;

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

        if (!rv) {
            setMessage("!! Invalid move");
        }

        activeCmd = cmd_none;
        return true;
    }
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
