#ifndef __SOL_H__
#define __SOL_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


static bool HackerMode = false;

enum CardType {
    ct_none,
    ct_ace,
    ct_two,
    ct_three,
    ct_four,
    ct_five,
    ct_six,
    ct_seven,
    ct_eight,
    ct_nine,
    ct_ten,
    ct_jack,
    ct_queen,
    ct_king
};

enum CardColor {
    cc_spades,
    cc_hearts,
    cc_clubs,
    cc_diamonds
};

struct Card {
    enum CardType  m_type;
    enum CardColor m_color;
    bool           m_down;
};


bool isRed(struct Card card) {
    return card.m_color == cc_hearts || card.m_color == cc_diamonds;
}

bool oneLess(struct Card card1, struct Card card2) {
    return card1.m_type+1 == card2.m_type;
}

bool oppositeColor(struct Card card1, struct Card card2) {
    return !isRed(card1) != !isRed(card2);
}



const char * typeMark(struct Card card) {
    switch (card.m_type) {
        case ct_ace:    return "A"; break;
        case ct_two:    return "2"; break;
        case ct_three:  return "3"; break;
        case ct_four:   return "4"; break;
        case ct_five:   return "5"; break;
        case ct_six:    return "6"; break;
        case ct_seven:  return "7"; break;
        case ct_eight:  return "8"; break;
        case ct_nine:   return "9"; break;
        case ct_ten:    return "10"; break;
        case ct_jack:   return "J"; break;
        case ct_queen:  return "Q"; break;
        case ct_king:   return "K"; break;
        default:        return "0"; break;
    }

    return "?!";
}

const char * colorMark(struct Card card) {
    switch (card.m_color) {
        case cc_spades:     return "♠"; break;
        case cc_hearts:     return "♥"; break;
        case cc_clubs:      return "♣"; break;
        case cc_diamonds:   return "♦"; break;
        default:            return "?"; break;
    }

    return "!";
}


void printCard(struct Card card) {
    if (card.m_down) {
        printf(KWHT);
        if (!HackerMode) {
            printf("[??]");
        } else {
            printf("{%s%s}", typeMark(card), colorMark(card));
        }
        printf(KNRM);
    } else {
        printf(isRed(card)?KRED:KBLU);
        printf("[%s%s]", typeMark(card), colorMark(card));
        printf(KNRM);
    }
}



void swapCard(struct Card *pCard1, struct Card *pCard2) {
    struct Card card = *pCard1;
    *pCard1 = *pCard2;
    *pCard2 = card;
}


void shufflePack(struct Card pack[], size_t size) {
    srand(time(NULL));
    for (int i = size-1; i >= 0; i--) {
        swapCard(&pack[i], &pack[rand()%(i+1)]);
    }
}

////////////////////////////////////////////////////////////////////

#define SOL_STACK_MAX 52

struct SolStack {
    size_t      m_size;
    struct Card m_cards[SOL_STACK_MAX];
};


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

void printStack(struct SolStack *pStack) {
    for (int i = 0; i < pStack->m_size; i++) {
        printCard(pStack->m_cards[i]);
    }
}


void printStacks(struct SolStack stacks[], size_t size) {
    for (int i = 0; i < size; i++) {
        printf("%2d: ", i+1);
        printStack(&stacks[i]);
        printf("\n");
    }
}


void topUp(struct SolStack *pStack) {
    if (topCard(pStack) != NULL) {
        topCard(pStack)->m_down = false;
    }
}


void printTop(struct SolStack *pStack) {
    if (topCard(pStack) == NULL) {
        printf("___");
    } else {
        printCard(*topCard(pStack));
    }
}

#endif
