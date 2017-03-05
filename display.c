#include "display.h"
#include "controls.h"
#include "solstack.h"
#include "table.h"
#include <stdio.h>

static int ColorIdx = 0;
static bool HackerMode = false;
static bool ShowHelp = false;
const char * Message = "Press h for help";

static const struct ColorScheme Schemes[COLOR_SCHEMES] = {
    { KNRM, KRED, KBLU, KWHT },
    { KCYN, KRED, KNRM, KWHT },
    { KNRM, KRED, KGRN, KBLU }
};


void setMessage(const char *message) {
    Message = message;
}


void printCard(struct Card card) {
    if (card.m_down) {
        printf("%s", Schemes[ColorIdx].m_back);
        if (!HackerMode) {
            printf("[??]");
        } else {
            printf("{%s%s}", typeMark(card), colorMark(card));
        }
        printf("%s", Schemes[ColorIdx].m_normal);
    } else {
        printf("%s", isRed(card) ? Schemes[ColorIdx].m_red
                                 : Schemes[ColorIdx].m_black);
        printf("[%s%s]", typeMark(card), colorMark(card));
        printf("%s", Schemes[ColorIdx].m_normal);
    }
}


void printStack(struct SolStack *pStack) {
    for (int i = 0; i < pStack->m_size; i++) {
        printCard(pStack->m_cards[i]);
    }
}


void printTop(struct SolStack *pStack) {
    if (topCard(pStack) == NULL) {
        printf("___ ");
    } else {
        printCard(*topCard(pStack));
    }
}


void displayColors(enum Command actCmd) {
    struct SolStack *colStacks = getColStacks();

    printf("colors\n");
    for (enum CardColor cc = cc_spades; cc <= cc_diamonds; cc++) {
        printf(" %c:", 'A'+cc);
        if (!isVictory()) {
            printTop(&colStacks[cc]);
        } else {
            printStack(&colStacks[cc]);
            printf("\n");
        }
        if (actCmd == cmd_color0+cc) {
            printf("<-");
        }
    }
    printf("\n\n");
}


void displayPack(enum Command actCmd) {
    struct SolStack *pPack = getPack();
    struct SolStack *pRest = getRest();

    printf("pack\n");
    printf(" P:");
    printTop(pPack);
    printTop(pRest);
    if (actCmd == cmd_pack) {
        printf("<-");
    }
    printf("\n\n");

}


void displayDesk(enum Command actCmd) {
    struct SolStack *deskStacks = getDeskStacks();

    printf("desk\n");
    for (int i = 0; i < STACKS; i++) {
        printf(" %c:", 'I'+i);
        printStack(&deskStacks[i]);
        if (actCmd == cmd_desk0+i) {
            printf("<-");
        }
        printf("\n");
    }
    printf("\n");
}


void displayHelp() {
    printf("help\n");
    printf("%s", getHelp());
    printf("\n");
}


void displayAll(enum Command actCmd) {

    printf("\033[2J\033[1;1H"); // reset screen
    printf("%s", Schemes[ColorIdx].m_normal);
    printf("TEXTOLITAIRE\n\n");

    displayColors(actCmd);
    displayPack(actCmd);
    displayDesk(actCmd);

    if (ShowHelp) {
        displayHelp();
    }

    printf("%s\n", Message);
    printf(KNRM);
}


bool controlDisplay(enum Command cmd) {
    if (cmd == cmd_scheme) {
        ColorIdx = (ColorIdx+1) % COLOR_SCHEMES;
        return true;

    } else if (cmd == cmd_help) {
        ShowHelp = !ShowHelp;
        return true;

    } else if (cmd == '+') {    // super secret key
        HackerMode = !HackerMode;
        return true;
    }

    return false;
}
