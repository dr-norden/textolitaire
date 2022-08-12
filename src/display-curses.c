#define _DEFAULT_SOURCE
#include "card.h"
#include "config.h"
#include "controls.h"
#include "diskdata.h"
#include "display.h"
#include "input.h"
#include "solstack.h"
#include "table.h"
#include <ctype.h>
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//----------------------------------------------------------------------------
// Private definitions/variables
//----------------------------------------------------------------------------

typedef enum
{
  CP_CARD_REVERSE = 1,
  CP_CARD_RED,
  CP_CARD_BLACK,
  CP_TABLE,
  CP_LABEL,
  CP_STATS
} EColorPair_t;

#define CARD_WIDTH 9
#define CARD_HEIGHT 7
#define CARD_DISTANCE_X (CARD_WIDTH + 1)
#define CARD_DISTANCE_Y (CARD_HEIGHT + 5)
#define PACK_BASE_X 1
#define PACK_BASE_Y 1
#define COLORS_BASE_X (PACK_BASE_X + 3 * CARD_DISTANCE_X)
#define COLORS_BASE_Y (PACK_BASE_Y)
#define DESK_BASE_X (PACK_BASE_X)
#define DESK_BASE_Y (PACK_BASE_Y + CARD_DISTANCE_Y)

static bool HackerMode = false;
static int ColorSchemeIdx = 0;
static const char* Message = "";
static struct
{
  bool m_enabled;
  Card_t m_card;
  int m_x;
  int m_y;
  int m_dirX;
  int m_dirY;
} FlyingCard = { 0 };

//----------------------------------------------------------------------------
// Private function prototypes
//----------------------------------------------------------------------------

static void HandleVictory(void);
static void DisplayColors(ECommand_t activeCmd);
static void DisplayPack(ECommand_t activeCmd);
static void DisplayDesk(ECommand_t activeCmd);
static void DisplayMessage(void);
static void DisplayStats(void);
static void DisplayHelp(void);
static void DisplayFlying(void);
static void ApplyColorScheme(void);
static void MoveFlying(void);
static void ResetFlying(int maxHeight);
static void DrawRectangle(int x, int y, int height, int width);
static void DrawRectangleInWindow(WINDOW* win,
                                  int x,
                                  int y,
                                  int height,
                                  int width);
static void DrawLabel(const char* label, int x, int y);
static void DrawFullCard(Card_t card, int x, int y);
static void DrawStack(const SolStack_t* stackPtr, int x, int y);
static void DrawEmptyGrid(int x, int y);
static void DrawCmdLabel(ECommand_t cmd, int x, int y);
static void DrawTopCard(const SolStack_t* stackPtr, int x, int y);
static void DrawCardArt(const char artData[],
                        ECardColor_t cardColor,
                        int x,
                        int y,
                        int width);
static void SleepMs(int ms);

//----------------------------------------------------------------------------
// Public function implementation
//----------------------------------------------------------------------------

void dspl_Init(void)
{
  setlocale(LC_ALL, "");

  initscr();
  curs_set(0);
  start_color();

  ColorSchemeIdx = dd_GetColorScheme2Idx();
  ApplyColorScheme();

  wbkgd(stdscr, COLOR_PAIR(CP_TABLE));
}

void dspl_Clear(void)
{
  endwin();
}

void dspl_Render(ECommand_t activeCmd)
{
  erase();
  DisplayColors(activeCmd);
  DisplayPack(activeCmd);
  DisplayDesk(activeCmd);
  DisplayFlying();
  DisplayStats();

  if (Message[0] != '\0') {
    DisplayMessage();
  }

  refresh();
  if (activeCmd == CMD_HELP) {
    DisplayHelp();
  }
}

void dspl_SetMessage(const char* message)
{
  Message = message;
}

void dspl_ShowHelp(void)
{
  dspl_Render(CMD_HELP);
  (void)input_ReadKey();
}

void dspl_HackerMode(void)
{
  HackerMode = !HackerMode;
}

void dspl_ChangeColorScheme()
{
  ColorSchemeIdx++;
  ApplyColorScheme();
}

char dspl_Victory(void)
{
  int maxX = getmaxy(stdscr);
  char key = 0;

  cbreak();
  nodelay(stdscr, TRUE);

  FlyingCard.m_enabled = true;
  while (key <= 0) {
    ResetFlying(maxX);
    int sleepTime = (rand() % 50) + 10;
    for (int i = 0; i < (maxX + CARD_HEIGHT); i++) {
      MoveFlying();
      dspl_Render(CMD_NONE);
      SleepMs(sleepTime);

      key = input_ReadKey();
      if (key) {
        break;
      }
    }
  }
  FlyingCard.m_enabled = false;

  nodelay(stdscr, FALSE);

  return key;
}

//----------------------------------------------------------------------------
// Private function implementation
//----------------------------------------------------------------------------

static void DrawEmptyGrid(int x, int y)
{
  int edgeX = x + CARD_WIDTH - 1;
  int edgeY = y + CARD_HEIGHT - 1;

  // corners
  mvwaddch(stdscr, y, x, ACS_ULCORNER);
  mvwaddch(stdscr, y, edgeX, ACS_URCORNER);
  mvwaddch(stdscr, edgeY, x, ACS_LLCORNER);
  mvwaddch(stdscr, edgeY, edgeX, ACS_LRCORNER);

  // horizontal teeth
  for (int j = x + 1; j < edgeX; j++) {
    mvwaddch(stdscr, y, j, ACS_TTEE);
    mvwaddch(stdscr, edgeY, j, ACS_BTEE);
  }

  // vertical teeth
  for (int i = y + 1; i < edgeY; i++) {
    mvwaddch(stdscr, i, x, ACS_LTEE);
    mvwaddch(stdscr, i, edgeX, ACS_RTEE);
  }

  // internal mesh
  for (int i = y + 1; i < edgeY; i++) {
    for (int j = x + 1; j < edgeX; j++) {
      mvwaddch(stdscr, i, j, ACS_PLUS);
    }
  }
}

static void DrawCmdLabel(ECommand_t cmd, int x, int y)
{
  char c = toupper(ctrl_GetCmdKey(cmd));
  switch (c) {
    case ' ': mvwprintw(stdscr, y, x, "[space]"); break;
    case '\n': mvwprintw(stdscr, y, x, "[enter]"); break;
    case '\t': mvwprintw(stdscr, y, x, "[tab]"); break;
    default: mvwaddch(stdscr, y, x, c); break;
  }
}

static void DrawTopCard(const SolStack_t* stackPtr, int x, int y)
{
  if (stk_CTopCard(stackPtr) == NULL) {
    DrawEmptyGrid(x, y);
  } else {
    DrawFullCard(*stk_CTopCard(stackPtr), x, y);
  }
}

static void DisplayMessage(void)
{
  int llen = 0;
  if ((Message != NULL) && (Message[0] != '\0')) {
    llen = strlen(Message) / 2 + 2;
    DrawLabel(Message,
              PACK_BASE_X + CARD_DISTANCE_X * 3 + CARD_WIDTH / 2 - llen,
              PACK_BASE_Y + CARD_HEIGHT + 1);
  }
}

static void DisplayColors(ECommand_t activeCmd)
{
  const SolStack_t* colStacks = tbl_GetColorStacks();

  for (ECardColor_t cc = CC_SPADES; cc <= CC_DIAMONDS; cc++) {
    DrawCmdLabel(CMD_COLOR0 + cc,
                 COLORS_BASE_X + CARD_DISTANCE_X * cc + 1,
                 COLORS_BASE_Y + CARD_HEIGHT);
    DrawTopCard(&colStacks[cc],
                COLORS_BASE_X + CARD_DISTANCE_X * cc,
                COLORS_BASE_Y);
    if (activeCmd == CMD_COLOR0 + cc) {
      DrawLabel("sel",
                COLORS_BASE_X + 1 + CARD_DISTANCE_X * cc,
                COLORS_BASE_Y + 2);
    }
  }
}

static void DisplayPack(ECommand_t activeCmd)
{
  const SolStack_t* packPtr = tbl_GetPack();
  const SolStack_t* restPtr = tbl_GetRest();

  DrawCmdLabel(CMD_NEXT, PACK_BASE_X + 1, PACK_BASE_Y + CARD_HEIGHT);
  DrawTopCard(packPtr, PACK_BASE_X, PACK_BASE_Y);
  DrawCmdLabel(CMD_PACK,
               PACK_BASE_X + CARD_DISTANCE_X + 1,
               PACK_BASE_Y + CARD_HEIGHT);
  DrawTopCard(restPtr, PACK_BASE_X + CARD_DISTANCE_X, PACK_BASE_Y);
  if (activeCmd == CMD_PACK) {
    DrawLabel("sel", PACK_BASE_X + 1 + CARD_DISTANCE_X, PACK_BASE_Y + 2);
  }
}

static void DrawStack(const SolStack_t* stackPtr, int x, int y)
{
  if (stackPtr->m_size == 0) {
    DrawEmptyGrid(x, y);
  }
  for (int i = 0; i < stackPtr->m_size; i++) {
    DrawFullCard(stackPtr->m_cards[i], x, y + 2 * i);
  }
}

static void DisplayDesk(ECommand_t activeCmd)
{
  const SolStack_t* deskStacks = tbl_GetDeskStacks();

  for (int i = 0; i < NUM_STACKS; i++) {
    DrawCmdLabel(CMD_DESK0 + i,
                 DESK_BASE_X + CARD_DISTANCE_X * i + 1,
                 DESK_BASE_Y - 1);
    DrawStack(&deskStacks[i], DESK_BASE_X + CARD_DISTANCE_X * i, DESK_BASE_Y);
    if (activeCmd == CMD_DESK0 + i) {
      DrawLabel("sel",
                DESK_BASE_X + 1 + CARD_DISTANCE_X * i,
                DESK_BASE_Y + deskStacks[i].m_size * 2);
    }
  }
}

static void DisplayStats(void)
{
  int maxX = getmaxx(stdscr);
  int maxY = getmaxy(stdscr);

  attron(COLOR_PAIR(CP_STATS));

  for (int x = 1; x < maxX - 1; x++) {
    mvwaddch(stdscr, maxY - 2, x, ' ');
  }

  mvwprintw(stdscr,
            maxY - 2,
            2,
            "High Score: %-5d   Your Score: %-5d",
            dd_GetHighScore(),
            tbl_GetCurrentScore());
  DrawRectangle(0, maxY - 3, maxX - 1, 2);

  attroff(COLOR_PAIR(CP_STATS));
}

static void DisplayHelp(void)
{
  enum
  {
    HELP_WIN_X = 8,
    HELP_WIN_Y = 3,
    HELP_WIN_WIDTH = 55,
    HELP_WIN_HEIGHT = 20,
  };
  WINDOW* helpWin = NULL;
  helpWin = newwin(HELP_WIN_HEIGHT, HELP_WIN_WIDTH, HELP_WIN_Y, HELP_WIN_X);

  wbkgd(helpWin, COLOR_PAIR(5));
  wattron(helpWin, COLOR_PAIR(5));

  mvwprintw(helpWin, 1, 0, "%s", ctrl_GetHelp());
  DrawRectangleInWindow(helpWin, 0, 0, HELP_WIN_WIDTH - 1, HELP_WIN_HEIGHT - 1);

  wattroff(helpWin, COLOR_PAIR(5));

  wrefresh(helpWin);
}

static void DrawLabel(const char* label, int x, int y)
{
  int llen = strlen(label) + 3;

  attron(COLOR_PAIR(CP_LABEL));

  mvwprintw(stdscr, y + 1, x + 2, "%s", label);
  mvwaddch(stdscr, y + 1, x + 1, ' ');
  mvwaddch(stdscr, y + 1, x + llen - 1, ' ');
  DrawRectangle(x, y, llen, 2);

  attroff(COLOR_PAIR(CP_LABEL));
}

static void DrawRectangle(int x, int y, int width, int height)
{
  DrawRectangleInWindow(stdscr, x, y, width, height);
}

static void DrawRectangleInWindow(WINDOW* win,
                                  int x,
                                  int y,
                                  int width,
                                  int height)
{
  mvwaddch(win, y, x, ACS_ULCORNER);
  mvwaddch(win, y, x + width, ACS_URCORNER);
  mvwaddch(win, y + height, x, ACS_LLCORNER);
  mvwaddch(win, y + height, x + width, ACS_LRCORNER);

  for (int i = 1; i < height; i++) {
    mvwaddch(win, y + i, x, ACS_VLINE);
    mvwaddch(win, y + i, x + width, ACS_VLINE);
  }

  for (int i = 1; i < width; i++) {
    mvwaddch(win, y, x + i, ACS_HLINE);
    mvwaddch(win, y + height, x + i, ACS_HLINE);
  }
}

static void ApplyColorScheme(void)
{
  enum
  {
    NUM_SCHEMES = 6
  };

  static struct
  {
    short m_fg;
    short m_bg;
  } ColorSchemes[NUM_SCHEMES] = {
    { COLOR_WHITE, COLOR_BLUE },   { COLOR_BLACK, COLOR_GREEN },
    { COLOR_WHITE, COLOR_BLACK },  { COLOR_WHITE, COLOR_MAGENTA },
    { COLOR_BLACK, COLOR_YELLOW }, { COLOR_RED, COLOR_BLACK }
  };

  if (ColorSchemeIdx >= NUM_SCHEMES) {
    ColorSchemeIdx = 0;
  }

  dd_SetColorScheme2Idx(ColorSchemeIdx);

  init_pair(CP_CARD_REVERSE,
            ColorSchemes[ColorSchemeIdx].m_fg,
            ColorSchemes[ColorSchemeIdx].m_bg);
  init_pair(CP_CARD_RED, COLOR_RED, COLOR_WHITE);
  init_pair(CP_CARD_BLACK, COLOR_BLACK, COLOR_WHITE);
  init_pair(CP_TABLE, COLOR_BLACK, COLOR_CYAN);
  init_pair(CP_LABEL, COLOR_WHITE, COLOR_BLACK);
  init_pair(CP_STATS, COLOR_WHITE, COLOR_BLUE);
}

static void ResetFlying(int maxHeight)
{
  const int maxWidth = (NUM_STACKS - 1) * (CARD_WIDTH + 1);

  FlyingCard.m_dirX = (rand() % 7) - 3;
  FlyingCard.m_x = rand() % maxWidth;

  FlyingCard.m_dirY = (rand() % 2) ? 1 : -1;
  FlyingCard.m_y = (FlyingCard.m_dirY > 0) ? -CARD_HEIGHT : maxHeight;

  FlyingCard.m_card.m_color = rand() % 4;
  FlyingCard.m_card.m_type = (rand() % CT_KING) + 1;
  FlyingCard.m_card.m_face = (rand() % 5) == 0 ? CF_DOWN : CF_UP;
}

static void DisplayFlying(void)
{
  if (FlyingCard.m_enabled) {
    DrawFullCard(FlyingCard.m_card, FlyingCard.m_x, FlyingCard.m_y);
  }
}

static void MoveFlying(void)
{
  const int maxWidth = (NUM_STACKS - 1) * (CARD_WIDTH + 1);

  if (FlyingCard.m_enabled) {
    FlyingCard.m_x += FlyingCard.m_dirX;
    if ((FlyingCard.m_x < 0) || (FlyingCard.m_x > maxWidth)) {
      FlyingCard.m_dirX *= -1;
    }
    FlyingCard.m_y += FlyingCard.m_dirY;
  }
}

static void SleepMs(int ms)
{
  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = 1000L * 1000L * (long)ms;
  nanosleep(&ts, NULL);
}

static void DrawCardArt(const char artData[],
                        ECardColor_t cardColor,
                        int x,
                        int y,
                        int width)
{
  static const char* ColorSymbols[NUM_CARD_COLORS] = { "♠", "♥", "♣", "♦" };
  for (int i = 0; i < width; i++) {
    switch (artData[i]) {
      case 'B': mvwprintw(stdscr, y, x + i, "▒"); break;
      case 'C': mvwprintw(stdscr, y, x + i, "%s", ColorSymbols[cardColor]); break;
      default: mvwprintw(stdscr, y, x + i, "%c", artData[i]); break;
    }
  }
}

static void DrawFullCard(Card_t card, int x, int y)
{
  enum
  {
    IMG_WIDTH = CARD_WIDTH - 2,
    IMG_HEIGHT = CARD_HEIGHT - 2,
  };

  static const char CardArtData[NUM_CARD_TYPES][IMG_HEIGHT][IMG_WIDTH + 1] = {
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
      " C C C ",
      "  C C  ",
      " C C C ",
      "     8C",
    },
    {
      "C9     ",
      " C C C ",
      " C C C ",
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

  EColorPair_t colorPair;
  ECardType_t cardType =
    (card.m_face == CF_DOWN && !HackerMode) ? CT_NONE : card.m_type;

  if (card.m_face == CF_DOWN) {
    colorPair = CP_CARD_REVERSE;
  } else if (card_IsRed(card)) {
    colorPair = CP_CARD_RED;
  } else {
    colorPair = CP_CARD_BLACK;
  }

  attron(COLOR_PAIR(colorPair));
  for (int i = 0; i < IMG_HEIGHT; i++) {
    DrawCardArt(CardArtData[cardType][i],
                card.m_color,
                x + 1,
                y + i + 1,
                IMG_WIDTH);
  }
  DrawRectangle(x, y, CARD_WIDTH - 1, CARD_HEIGHT - 1);
  attroff(COLOR_PAIR(colorPair));
}
