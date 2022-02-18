#define _DEFAULT_SOURCE
#include <locale.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "card.h"
#include "config.h"
#include "display.h"
#include "controls.h"
#include "solstack.h"
#include "table.h"
#include "diskdata.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define CardHeight ( 7 )
#define CardWidth ( 9 )
#define CardDistanceX ( CardHeight + 5 )
#define CardDistanceY ( CardWidth + 1 )
#define PackBaseX ( 1 )
#define PackBaseY ( 1 )
#define ColorsBaseX ( PackBaseX )
#define ColorsBaseY ( PackBaseY + 3*CardDistanceY )
#define DeskBaseX ( PackBaseX + CardDistanceX )
#define DeskBaseY ( PackBaseY )

static bool ShowHelp = false;
const char *Message = "";

const char CardArtData[14][5][8] =
{
    {
        "B B B B",
        " B B B ",
        "B B B B",
        " B B B ",
        "B B B B",
    },
    {
        "CA     ",
        "       ",
        "   C   ",
        "       ",
        "     AC",
    },
    {
        "C2     ",
        "   C   ",
        "       ",
        "   C   ",
        "     2C",
    },
    {
        "C3     ",
        "   C   ",
        "   C   ",
        "   C   ",
        "     3C",
    },
    {
        "C4     ",
        "  C C  ",
        "       ",
        "  C C  ",
        "     4C",
    },
    {
        "C5     ",
        "  C C  ",
        "   C   ",
        "  C C  ",
        "     5C",
    },
    {
        "C6     ",
        "  C C  ",
        "  C C  ",
        "  C C  ",
        "     6C",
    },
    {
        "C7     ",
        "  C C  ",
        " C C C ",
        "  C C  ",
        "     7C",
    },
    {
        "C8     ",
        "       ",
        "   C   ",
        "       ",
        "     8C",
    },
    {
        "C9     ",
        " C C C ",
        "  C C  ",
        " C C C ",
        "     9C",
    },
    {
        "C10    ",
        " C C C ",
        " CC CC ",
        " C C C ",
        "    10C",
    },
    {
        "CJ     ",
        "  \"\"\"  ",
        "  CC|  ",
        "  \\-|  ",
        "     JC",
    },
    {
        "CQ     ",
        "  @@@  ",
        "  CC@  ",
        "  \\/@  ",
        "     QC",
    },
    {
        "CK     ",
        "  WWW  ",
        "  CC|  ",
        "  MM|  ",
        "     KC",
    },
};

const char *ColorSymbols[4] = { "♠", "♥", "♣", "♦" };

void handleVictory(void);
void displayColors(void);
void displayPack(void);
void displayDesk(void);
void displayStats(void);
void displayHelp(void);
void displayFlying(void);
void finalAnimation(void);
void printRectangle(int x, int y, int lines, int columns);
void wprintRectangle(WINDOW *win, int x, int y, int lines, int columns);
void printLabel(int x, int y, const char *label);
void printFullCard(struct Card card, int x, int y);

void initDisplay()
{
    setlocale(LC_ALL, "");

    initscr();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_RED,   COLOR_WHITE);
    init_pair(3, COLOR_BLACK, COLOR_WHITE);
    init_pair(4, COLOR_BLACK, COLOR_CYAN);
    init_pair(5, COLOR_WHITE, COLOR_BLACK);


    wbkgd(stdscr, COLOR_PAIR(4));
}

void clearDisplay()
{
    endwin();
}

void displayMessage(void)
{
    int llen = 0;
    if ((Message != NULL) && (Message[0] != '\0')) {
        llen = strlen(Message) / 2 + 2;
        printLabel(PackBaseX + CardWidth - 1, PackBaseY + CardDistanceY*3 + CardWidth/2 - llen, Message);
    }
}

void displayAll()
{
    handleVictory();
    erase();
    displayColors();
    displayPack();
    displayDesk();
    displayFlying();
    displayStats();

    if (Message[0] != '\0') {
        displayMessage();
    }

    refresh();
    if (ShowHelp) {
        displayHelp();
    }
}


void setMessage(const char *message)
{
    Message = message;
}

bool controlDisplay(enum Command cmd)
{
    if (ShowHelp) {
        ShowHelp = false;
        return true;
    }

    if (cmd == cmd_hack) {
        setMessage("Hacker! Press any key to stop animation");
        finalAnimation();
        return true;
    }

    if (cmd == cmd_help) {
        ShowHelp = !ShowHelp;
    }

    if (ShowHelp) {
        return true;
    }

    if (cmd == cmd_scheme) {
        setMessage("Changing color scheme not supported yet");
        return true;
    }

    if (cmd == cmd_exit) {
        endwin();
    }
    return false;
}

void printEmptyGrid(int x, int y)
{
    int i, j;

    mvwaddch(stdscr, x, y, ACS_ULCORNER);
    mvwaddch(stdscr, x, y+8, ACS_URCORNER);
    mvwaddch(stdscr, x+6, y+0, ACS_LLCORNER);
    mvwaddch(stdscr, x+6, y+8, ACS_LRCORNER);
    for (j = 1; j < 8; j++) {
      mvwaddch(stdscr, x, y+j, ACS_TTEE);
      mvwaddch(stdscr, x+6, y+j, ACS_BTEE);
    }
    for (i = 0; i < 5; i++) {
        mvwaddch(stdscr, x+i+1, y+0, ACS_LTEE);
        mvwaddch(stdscr, x+i+1, y+8, ACS_RTEE);
        for (j = 0; j < 7; j++) {
            mvwaddch(stdscr, x+i+1, y+j+1, ACS_PLUS);
        }
    }
}

void printCommand(enum Command cmd, int x, int y)
{
    char c = toupper(getCmdKey(cmd));
    switch (c) {
        case ' ':
            mvwprintw(stdscr, x, y, "[space]");
            break;
        case '\n':
            mvwprintw(stdscr, x, y, "[enter]");
            break;
        case '\t':
            mvwprintw(stdscr, x, y, "[tab]");
            break;
        default:
            mvwaddch(stdscr, x, y, c);
            break;
    }
}

void printTop(struct SolStack *pStack, int x, int y)
{
    if (topCard(pStack) == NULL) {
        printEmptyGrid(x, y);
    } else {
        printFullCard(*topCard(pStack), x, y);
    }
}

void displayColors(void)
{
    struct SolStack *colStacks = getColStacks();
    enum Command actCmd = getActiveCmd();

    for (enum CardColor cc = cc_spades; cc <= cc_diamonds; cc++) {
        printCommand(cmd_color0+cc, ColorsBaseX+CardHeight, ColorsBaseY + CardDistanceY*cc + 1);
        printTop(&colStacks[cc], ColorsBaseX, ColorsBaseY + CardDistanceY*cc);
        if (actCmd == cmd_color0+cc) {
            printLabel(ColorsBaseX + 2, ColorsBaseY + 1 + CardDistanceY*cc, "sel");
        }
    }
}

void displayPack(void)
{
    struct SolStack *pPack = getPack();
    struct SolStack *pRest = getRest();
    enum Command actCmd = getActiveCmd();

    printCommand(cmd_next, PackBaseX+CardHeight, PackBaseY+1);
    printTop(pPack, PackBaseX, PackBaseY);
    printCommand(cmd_pack, PackBaseX+CardHeight, PackBaseY+CardDistanceY + 1);
    printTop(pRest, PackBaseX, PackBaseY + CardDistanceY);
    if (actCmd == cmd_pack) {
        printLabel(PackBaseX + 2, PackBaseY + 1 + CardDistanceY, "sel");
    }
}

void printStack(struct SolStack *pStack, int x, int y)
{
    if (pStack->m_size == 0) {
        printEmptyGrid(x, y);
    }
    for (int i = 0; i < pStack->m_size; i++) {
        printFullCard(pStack->m_cards[i], x+2*i, y);
    }
}

void displayDesk(void)
{
    struct SolStack *deskStacks = getDeskStacks();
    enum Command actCmd = getActiveCmd();

    for (int i = 0; i < STACKS; i++) {
        printCommand(cmd_desk0+i, DeskBaseX-1, DeskBaseY + CardDistanceY*i + 1);
        printStack(&deskStacks[i], DeskBaseX, DeskBaseY + CardDistanceY*i);
        if (actCmd == cmd_desk0+i) {
            printLabel(DeskBaseX+deskStacks[i].m_size*2, DeskBaseY + 1 + CardDistanceY*i, "sel");
        }
    }
}

void displayStats(void)
{
    int maxX = getmaxy(stdscr);
    int maxY = getmaxx(stdscr);

    attron(COLOR_PAIR(1));

    for (int y = 1; y < maxY-1; y++) {
        mvwaddch(stdscr, maxX-2, y, ' ');
    }

    mvwprintw(stdscr, maxX-2, 2, "High Score: %-5d   Your Score: %-5d", getHiScore(), getScore());
    printRectangle(maxX-3, 0, 2, maxY-1);

    attroff(COLOR_PAIR(1));
}

void displayHelp(void) {
    WINDOW *helpWin = NULL;
    helpWin = newwin(20, 54, 3, 9);

    wbkgd(helpWin, COLOR_PAIR(5));
    wattron(helpWin, COLOR_PAIR(5));

    mvwprintw(helpWin, 1, 0, getHelp());
    wprintRectangle(helpWin, 0, 0, 19, 53);

    wattroff(helpWin, COLOR_PAIR(5));

    wrefresh(helpWin);
}

void printFullCard(struct Card card, int x, int y)
{
    int i,j,colorNum;
    enum CardType cardType = (card.m_down) ? ct_none : card.m_type;

    if (card.m_down) {
        colorNum = 1;
    }
    else if (isRed(card)) {
        colorNum = 2;
    }
    else {
        colorNum = 3;
    }

    attron(COLOR_PAIR(colorNum));

    mvwaddch(stdscr, x, y, ACS_ULCORNER);
    for (j = 1; j < 8; j++) {
      mvwaddch(stdscr, x, y+j, ACS_HLINE);
    }
    mvwaddch(stdscr, x, y+8, ACS_URCORNER);
    for (i = 0; i < 5; i++) {
        mvwaddch(stdscr, x+i+1, y+0, ACS_VLINE);
        for (j = 0; j < 7; j++) {
            switch (CardArtData[cardType][i][j]) {
                case 'B':
                    mvwprintw(stdscr, x+i+1, y+j+1, "▒");
                    break;
                case 'C':
                    mvwprintw(stdscr, x+i+1, y+j+1, ColorSymbols[card.m_color]);
                    break;
                default:
                    mvwprintw(stdscr, x+i+1, y+j+1, "%c", CardArtData[cardType][i][j]);
                    break;
            }
        }
        mvwaddch(stdscr, x+i+1, y+8, ACS_VLINE);
    }
    mvwaddch(stdscr, x+6, y+0, ACS_LLCORNER);
    for (j = 1; j < 8; j++) {
      mvwaddch(stdscr, x+6, y+j, ACS_HLINE);
    }
    mvwaddch(stdscr, x+6, y+8, ACS_LRCORNER);

    attroff(COLOR_PAIR(colorNum));
}

void printLabel(int x, int y, const char *label)
{
    int llen = strlen(label) + 3;

    attron(COLOR_PAIR(5));

    mvwprintw(stdscr, x+1, y+2, label);
    mvwaddch(stdscr, x+1, y+1, ' ');
    mvwaddch(stdscr, x+1, y+llen-1, ' ');
    printRectangle(x, y, 2, llen);

    attroff(COLOR_PAIR(5));
}

void printRectangle(int x, int y, int lines, int columns)
{
    mvwaddch(stdscr, x, y, ACS_ULCORNER);
    mvwaddch(stdscr, x, y+columns, ACS_URCORNER);
    mvwaddch(stdscr, x+lines, y, ACS_LLCORNER);
    mvwaddch(stdscr, x+lines, y+columns, ACS_LRCORNER);

    for (int i = 1; i < lines; i++) {
        mvwaddch(stdscr, x+i, y, ACS_VLINE);
        mvwaddch(stdscr, x+i, y+columns, ACS_VLINE);
    }

    for (int i = 1; i < columns; i++) {
        mvwaddch(stdscr, x, y+i, ACS_HLINE);
        mvwaddch(stdscr, x+lines, y+i, ACS_HLINE);
    }
}

void wprintRectangle(WINDOW *win, int x, int y, int lines, int columns)
{
    mvwaddch(win, x, y, ACS_ULCORNER);
    mvwaddch(win, x, y+columns, ACS_URCORNER);
    mvwaddch(win, x+lines, y, ACS_LLCORNER);
    mvwaddch(win, x+lines, y+columns, ACS_LRCORNER);

    for (int i = 1; i < lines; i++) {
        mvwaddch(win, x+i, y, ACS_VLINE);
        mvwaddch(win, x+i, y+columns, ACS_VLINE);
    }

    for (int i = 1; i < columns; i++) {
        mvwaddch(win, x, y+i, ACS_HLINE);
        mvwaddch(win, x+lines, y+i, ACS_HLINE);
    }
}

static struct
{
    bool m_enabled;
    struct Card m_card;
    int m_x;
    int m_y;
    int m_dirX;
    int m_dirY;
} FlyingCard = {
    false,
    {ct_king, cc_spades, false},
    0, 0, 1, 1
};

void resetFlyingCard(int maxHeight)
{
    int maxWidth = 7 * CardWidth;

    FlyingCard.m_dirX = (rand() % 2) ? 1 : -1;
    FlyingCard.m_x = (FlyingCard.m_dirX > 0) ? -CardHeight : maxHeight;

    FlyingCard.m_dirY = (rand() % 7) -3;
    FlyingCard.m_y = rand() % CardWidth;

    FlyingCard.m_card.m_color = rand() % 4;
    FlyingCard.m_card.m_type = (rand() % ct_king) + 1;
    FlyingCard.m_card.m_down = (rand() % 5) == 0;
}

void displayFlying(void)
{
    if (FlyingCard.m_enabled) {
        printFullCard(FlyingCard.m_card, FlyingCard.m_x, FlyingCard.m_y);
    }
}

void moveFlying(void)
{
    if (FlyingCard.m_enabled) {
        FlyingCard.m_x += FlyingCard.m_dirX;
        FlyingCard.m_y += FlyingCard.m_dirY;
        if ((FlyingCard.m_y < 0) || (FlyingCard.m_y >= (7 * CardWidth))) {
            FlyingCard.m_dirY *= -1;
        }
    }
}

static void sleepMs(int ms)
{
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 1000L * 1000L * (long)ms;
    nanosleep(&ts, NULL);
}

void finalAnimation(void)
{
    int maxX = getmaxy(stdscr);
    bool keyPressed = false;

    cbreak();
    nodelay(stdscr, TRUE);

    FlyingCard.m_enabled = true;
    while (!keyPressed) {
        resetFlyingCard(maxX);
        int sleepTime = (rand()%50) + 10;
        for (int i = 0; i < (maxX + CardHeight); i++) {
            moveFlying();
            displayAll();
            sleepMs(sleepTime);

            if (getch() > 0) {
                keyPressed = true;
                break;
            }
        }
    }
    FlyingCard.m_enabled = false;
    setMessage("");

    nodelay(stdscr, FALSE);
}

void handleVictory(void)
{
    static bool inAnimation = false;
    if (!inAnimation && isVictory()) {
        inAnimation = true;
        finalAnimation();
        inAnimation = false;
    }
}
