#include "config.h"
#include "display.h"
#include "controls.h"
#include "solstack.h"
#include "table.h"
#include "diskdata.h"
#include <ctype.h>
#include <stdio.h>

static int ColorIdx = 0;
static bool HackerMode = false;
static bool ShowHelp = false;
const char * Message = "Press h for help";

static const struct ColorScheme Schemes[COLOR_SCHEMES] = {
    { SCHEME1_TXT, SCHEME1_RED, SCHEME1_BLK, SCHEME1_BCK },
    { SCHEME2_TXT, SCHEME2_RED, SCHEME2_BLK, SCHEME2_BCK },
    { SCHEME3_TXT, SCHEME3_RED, SCHEME3_BLK, SCHEME3_BCK }
};


void initDisplay() {
    ColorIdx = getColorSchemeIdx();
}


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
    if (pStack->m_size == 0) {
        printf("___ ");
    }
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


void displayColors() {
    struct SolStack *colStacks = getColStacks();
    enum Command actCmd = getActiveCmd();

    printf("colors\n");
    for (enum CardColor cc = cc_spades; cc <= cc_diamonds; cc++) {
        printf(" %c:", toupper(getCmdKey(cmd_color0+cc)));
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


void displayPack() {
    struct SolStack *pPack = getPack();
    struct SolStack *pRest = getRest();
    enum Command actCmd = getActiveCmd();

    printf("pack\n");
    printf(" %c:", toupper(getCmdKey(cmd_pack)));
    if (HackerMode) {
        printStack(pPack);
        printf(" : ");
        printStack(pRest);
    } else {
        printTop(pPack);
        printTop(pRest);
    }

    if (actCmd == cmd_pack) {
        printf("<-");
    }
    printf("\n\n");

}


void displayDesk() {
    struct SolStack *deskStacks = getDeskStacks();
    enum Command actCmd = getActiveCmd();

    printf("desk\n");
    for (int i = 0; i < STACKS; i++) {
        printf(" %c:", toupper(getCmdKey(cmd_desk0+i)));
        printStack(&deskStacks[i]);
        if (actCmd == cmd_desk0+i) {
            printf("<-");
        }
        printf("\n");
    }
    printf("\n");
}


void displayStats() {
    struct SolStack *pPack = getPack();
    struct SolStack *pRest = getRest();
    printf("remain (%ld)  draft (%ld)  score (%d) record (%d)\n\n",
                pPack->m_size,
                pRest->m_size,
                getScore(),
                getHiScore());
}

void displayHelp() {
    printf("help\n");
    printf("%s", getHelp());
    printf("\n");
}


void displayAll() {

    printf("\033[2J\033[1;1H"); // reset screen
    printf("%s", Schemes[ColorIdx].m_normal);
    printf("TEXTOLITAIRE\n\n");

    displayColors();
    displayPack();
    displayDesk();
    displayStats();

    if (ShowHelp) {
        displayHelp();
    }

    printf("%s\n", Message);
    printf(KNRM);
}


bool controlDisplay(enum Command cmd) {
    if (cmd == cmd_scheme) {
        ColorIdx = (ColorIdx+1) % COLOR_SCHEMES;
        setColorSchemeIdx(ColorIdx);
        return true;

    } else if (cmd == cmd_help) {
        ShowHelp = !ShowHelp;
        return true;

    } else if (cmd == cmd_hack) {
        HackerMode = !HackerMode;
        return true;
    }

    return false;
}
