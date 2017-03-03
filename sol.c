#include "sol.h"
#include "getche.h"

#define CARDS 52
#define STACKS 7
#define COLORS 4

struct SolStack pack;
struct SolStack qack;
struct SolStack stacks[STACKS];
struct SolStack colStacks[cc_diamonds+1];

bool ShowHelp = false;
const char * Message = "Press h for help";

static char cmd0 = '\0';
static char cmd1 = '\0';

void initialize() {
    cmd0 = '\0';
    cmd1 = '\0';

    initStack(&pack);
    initStack(&qack);
    initStacks(stacks, STACKS);
    initStacks(colStacks, COLORS);

    // create pack
    for (enum CardColor cc = cc_spades; cc <= cc_diamonds; cc++) {
        for (enum CardType ct = ct_ace; ct <= ct_king; ct++) {
            newCard(&pack, cc, ct);
        }
    }

    shufflePack(pack.m_cards, CARDS);

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
        pushStack(&qack, card);
    } else {
        while (popStack(&qack, &card) != NULL) {
            card.m_down = true;
            pushStack(&pack, card);
        }
    }
}


bool moveCard(struct SolStack *pSource, struct SolStack *pTarget) {
    struct Card card;
    if (popStack(pSource, &card) != NULL) {
        pushStack(pTarget, card);
        topUp(pSource);
        return true;
    }

    return false;
}


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


bool moveToDeck(struct SolStack *pSource, int stackNum) {
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



bool moveInDeck(int srcNum, int tgtNum) {
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
            printCard(helpCard);
            printf("\n");
            pushStack(&helpStack, helpCard);
            printCard(*topCard(&helpStack));
            printf("\n");
        }
    }

    while (popStack(&helpStack, &helpCard) != NULL) {
        printCard(helpCard);
        printf("\n");
        pushStack(pTarget, helpCard);
        printCard(*topCard(pTarget));
        printf("\n");
    }

    topUp(pSource);

    return true;
}

static bool isKeyDesk(char c) {
    return c >= 'i' && c <= 'o';
}

static bool isKeyCol(char c) {
    return c >= 'a' && c <= 'd';
}

static bool isKeyAnyCol(char c) {
    return c == 't' || c == 'u' || isKeyCol(c);
}

static bool isKeyExit(char c) {
    return c == 'x' || c == 'q';
}

static bool isKeyNext(char c) {
    return c == ' ';
}

static bool isKeyPack(char c) {
    return c == 'p' || c == '\n';
}

static bool isKeyHelp(char c) {
    return c == 'h' || c == '?' || c == '/';
}

static bool isKeyRestart(char c) {
    return c == 'r';
}

static bool isKeyColorScheme(char c) {
    return c == 's';
}

bool allColorKings() {
    for (enum CardColor cc = cc_spades; cc <= cc_diamonds; cc++) {
        struct Card *pCard = topCard(&colStacks[cc]);
        if (!pCard || pCard->m_type != ct_king) {
            return false;
        }
    }
    return true;
}


void displayColors() {
    printf("colors\n");
    for (enum CardColor cc = cc_spades; cc <= cc_diamonds; cc++) {
        printf(" %c:", 'A'+cc);
        if (!allColorKings()) {
            printTop(&colStacks[cc]);
        } else {
            printStack(&colStacks[cc]);
            printf("\n");
        }
        if (cmd0 == 'a'+cc) {
            printf("<-");
        }
    }
    printf("\n\n");
}


void displayPack() {
    printf("pack\n");
    printf(" P:");
    printTop(&pack);
    printTop(&qack);
    if (isKeyPack(cmd0)) {
        printf("<-");
    }
    printf("\n\n");

}


void displayDesk() {
    printf("desk\n");

    for (int i = 0; i < STACKS; i++) {
        printf(" %c:", 'I'+i);
        printStack(&stacks[i]);
        if (cmd0 == 'i'+i) {
            printf("<-");
        }
        printf("\n");
    }
    printf("\n");
}

void displayHelp() {
    printf("help");
    printf("\n space      .. draw next card");
    printf("\n double key .. move to color stack or cancel action");
    printf("\n P or enter .. choose current card from the pack");
    printf("\n A-D        .. choose color stack A-D");
    printf("\n I-O        .. chose desk stack I-O");
    printf("\n T          .. move to color stack");
    printf("\n H or ?     .. show/hide help");
    printf("\n S          .. change text color scheme");
    printf("\n R          .. restart game");
    printf("\n Q          .. quit game");
    printf("\n\n");
}

void displayAll() {

    printf("\033[2J\033[1;1H"); // reset screen
    printf("TEXTOLITAIRE\n\n");

    displayColors();
    displayPack();
    displayDesk();

    if (ShowHelp) {
        displayHelp();
    }

    printf("%s\n", Message);
}


int main(int argc, const char **argv) {
    bool toExit = false;
    bool rv = true;
    struct Card *pCard = NULL;

    initialize();

    while (!toExit) {
        displayAll();

        if (allColorKings()) {
            printf("CONGRATULATIONS, YOU HAVE WON!!!\n");
            cmd0 = getch();
            if (isKeyExit(cmd0)) {
                break;
            } else {
                initialize();
                continue;
            }
        }

        Message = "";
        rv = true;

        if (!cmd0) {
            cmd0 = getch();
            cmd1 = '\0';

            if (isKeyExit(cmd0)) {
                toExit = true;
            } else if (isKeyRestart(cmd0)) {
                initialize();
            } else if (isKeyNext(cmd0)) {
                nextCard();
            } else if (isKeyColorScheme(cmd0)) {
                ColorIdx = (ColorIdx+1) % COLOR_SCHEMES;
            } else if (isKeyHelp(cmd0)) {
                ShowHelp = !ShowHelp;
            } else if (cmd0 == '+') {
                HackerMode = !HackerMode;
            } else if (isKeyPack(cmd0) || isKeyDesk(cmd0) || isKeyCol(cmd0)) {
                // want one more key to complete action
                cmd1 = cmd0;
            }
            else {
                Message = "!! Invalid key";
            }
            cmd0 = cmd1;

        } else {
            cmd1 = getch();
            if (isKeyPack(cmd0)) {
                pCard = topCard(&qack);
                if (pCard) {
                    if (isKeyAnyCol(cmd1) || cmd0 == cmd1) {
                        rv = moveToColors(&qack) || cmd0 == cmd1;
                    } else if (isKeyDesk(cmd1)) {
                        rv = moveToDeck(&qack, cmd1-'i');
                    }
                } else {
                    rv = cmd0 == cmd1;
                }
            } else if (isKeyDesk(cmd0)) {
                pCard = topCard(&stacks[cmd0-'i']);
                if (pCard) {
                    if (isKeyAnyCol(cmd1) || cmd0 == cmd1) {
                        rv = moveToColors(&stacks[cmd0-'i']) || cmd0 == cmd1;
                    } else if (isKeyDesk(cmd1) && cmd0 != cmd1) {
                        rv = moveInDeck(cmd0-'i', cmd1-'i');
                    }
                } else {
                    rv = cmd0 == cmd1;
                }
            } else if (isKeyCol(cmd0)) {
                pCard = topCard(&colStacks[cmd0-'a']);
                if (pCard) {
                    if (isKeyDesk(cmd1)) {
                        rv = moveToDeck(&colStacks[cmd0-'a'], cmd1-'i') || cmd0 == cmd1;
                    }
                } else {
                    rv = cmd0 == cmd1;
                }
            } else {
                Message = "!! Invalid key";
            }

            if (!rv) {
                Message = "!! Invalid move";
            }
            cmd0='\0';
        }
    }
    return 0;
}
