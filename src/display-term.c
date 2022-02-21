#include "card.h"
#include "config.h"
#include "controls.h"
#include "diskdata.h"
#include "display.h"
#include "input.h"
#include "solstack.h"
#include "table.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

//----------------------------------------------------------------------------
// Private definitions/variables
//----------------------------------------------------------------------------

static int ColorSchemeIdx = 0;
static bool HackerMode = false;
static bool ShowHelp = false;
static const char* Message = "";

#define NUM_COLOR_SCHEMES 6
static const struct
{
  const char* m_normal;
  const char* m_red;
  const char* m_black;
  const char* m_back;
} Schemes[NUM_COLOR_SCHEMES] = {
  { SCHEME1_TXT, SCHEME1_RED, SCHEME1_BLK, SCHEME1_BCK },
  { SCHEME2_TXT, SCHEME2_RED, SCHEME2_BLK, SCHEME2_BCK },
  { SCHEME3_TXT, SCHEME3_RED, SCHEME3_BLK, SCHEME3_BCK },
  { SCHEME4_TXT, SCHEME4_RED, SCHEME4_BLK, SCHEME4_BCK },
  { SCHEME5_TXT, SCHEME5_RED, SCHEME5_BLK, SCHEME5_BCK },
  { SCHEME6_TXT, SCHEME6_RED, SCHEME6_BLK, SCHEME6_BCK },
};

//----------------------------------------------------------------------------
// Private function prototypes
//----------------------------------------------------------------------------

/* Helper print functions */
static void PrintCard(Card_t card);
static void PrintStack(const SolStack_t* stackPtr);
static void PrintTop(const SolStack_t* stackPtr);
static void PrintCardArt(const char* str);

/* Display functions for parts of the playing screen */
static void DisplayColors(ECommand_t activeCmd);
static void DisplayPack(ECommand_t activeCmd);
static void DisplayDesk(ECommand_t activeCmd);
static void DisplayStats(void);

/* Display functions for special events */
static void DisplayHelp(void);
static void DisplayVictory(void);

/* Return short text representation of card's type or color */
static const char* TypeMark(Card_t card);
static const char* ColorMark(Card_t card);

//----------------------------------------------------------------------------
// Public function implementation
//----------------------------------------------------------------------------

void dspl_Init(void)
{
  ColorSchemeIdx = dd_GetColorSchemeIdx();
  if (ColorSchemeIdx >= NUM_COLOR_SCHEMES) {
    ColorSchemeIdx = 0;
  }
}

void dspl_Clear(void)
{
  // nothing to be done
}

void dspl_Render(ECommand_t activeCmd)
{
  printf("\033[2J\033[1;1H"); // reset screen
  printf("%s", Schemes[ColorSchemeIdx].m_normal);
  printf("TEXTOLITAIRE\n\n");

  if (tbl_IsVictory()) {

  } else {
    DisplayColors(activeCmd);
    DisplayPack(activeCmd);
    DisplayDesk(activeCmd);
    DisplayStats();

    if (ShowHelp) {
      DisplayHelp();
    }
  }

  printf("%s\n", Message);
  printf(KNRM);
}

void dspl_SetMessage(const char* message)
{
  Message = message;
}

void dspl_ShowHelp(void)
{
  ShowHelp = !ShowHelp;
}

void dspl_HackerMode(void)
{
  HackerMode = !HackerMode;
}

void dspl_ChangeColorScheme()
{
  ColorSchemeIdx = (ColorSchemeIdx + 1) % NUM_COLOR_SCHEMES;
  dd_SetColorSchemeIdx(ColorSchemeIdx);
}

char dspl_Victory(void)
{
  DisplayVictory();
  DisplayStats();
  return input_ReadKey();
}

//----------------------------------------------------------------------------
// Private function implementation
//----------------------------------------------------------------------------

static void PrintCard(Card_t card)
{
  if (card.m_face == CF_DOWN) {
    printf("%s", Schemes[ColorSchemeIdx].m_back);
    if (!HackerMode) {
      printf("[??]");
    } else {
      printf("{%s%s}", TypeMark(card), ColorMark(card));
    }
    printf("%s", Schemes[ColorSchemeIdx].m_normal);
  } else {
    printf("%s",
           card_IsRed(card) ? Schemes[ColorSchemeIdx].m_red
                            : Schemes[ColorSchemeIdx].m_black);
    printf("[%s%s]", TypeMark(card), ColorMark(card));
    printf("%s", Schemes[ColorSchemeIdx].m_normal);
  }
}

static void PrintStack(const SolStack_t* stackPtr)
{
  if (stackPtr->m_size == 0) {
    printf("___ ");
  }
  for (int i = 0; i < stackPtr->m_size; i++) {
    PrintCard(stackPtr->m_cards[i]);
  }
}

static void PrintTop(const SolStack_t* stackPtr)
{
  if (stk_CTopCard(stackPtr) == NULL) {
    printf("___ ");
  } else {
    PrintCard(*stk_CTopCard(stackPtr));
  }
}

static void PrintCardArt(const char* str)
{
  const char* types = " A234567890JQK";
  const char* colors = "shcd";
  int len = 0;
  Card_t card;
  card.m_face = CF_UP;
  card.m_color = CC_SPADES;

  len = strlen(str);
  for (int i = 0; i < len; i++) {
    card.m_type = CT_NONE;
    for (ECardType_t ct = CT_ACE; ct <= CT_KING; ct++) {
      if (types[ct] == str[i]) {
        card.m_type = ct;
        break;
      }
    }

    if (card.m_type != CT_NONE) {
      PrintCard(card);
    } else if (str[i] == '\n') {
      printf("\n");
    } else if (strchr(colors, str[i]) != NULL) {
      for (ECardColor_t cc = CC_SPADES; cc <= CC_DIAMONDS; cc++) {
        if (colors[cc] == str[i]) {
          card.m_color = cc;
        }
      }
    } else {
      printf("    ");
    }
  }
}

static void DisplayColors(ECommand_t activeCmd)
{
  const SolStack_t* colStacks = tbl_GetColorStacks();

  printf("colors\n");
  for (ECardColor_t cc = CC_SPADES; cc <= CC_DIAMONDS; cc++) {
    printf(" %c:", toupper(ctrl_GetCmdKey(CMD_COLOR0 + cc)));
    PrintTop(&colStacks[cc]);
    if (activeCmd == CMD_COLOR0 + cc) {
      printf("<-");
    }
  }
  printf("\n\n");
}

static void DisplayPack(ECommand_t activeCmd)
{
  const SolStack_t* packPtr = tbl_GetPack();
  const SolStack_t* restPtr = tbl_GetRest();

  printf("pack\n");
  printf(" %c:", toupper(ctrl_GetCmdKey(CMD_PACK)));
  if (HackerMode) {
    PrintStack(packPtr);
    printf(" : ");
    PrintStack(restPtr);
  } else {
    PrintTop(packPtr);
    PrintTop(restPtr);
  }

  if (activeCmd == CMD_PACK) {
    printf("<-");
  }
  printf("\n\n");
}

static void DisplayDesk(ECommand_t activeCmd)
{
  const SolStack_t* deskStacks = tbl_GetDeskStacks();

  printf("desk\n");
  for (int i = 0; i < NUM_STACKS; i++) {
    printf(" %c:", toupper(ctrl_GetCmdKey(CMD_DESK0 + i)));
    PrintStack(&deskStacks[i]);
    if (activeCmd == CMD_DESK0 + i) {
      printf("<-");
    }
    printf("\n");
  }
  printf("\n");
}

static void DisplayStats(void)
{
  const SolStack_t* packPtr = tbl_GetPack();
  const SolStack_t* restPtr = tbl_GetRest();
  printf("remain (%ld)  draft (%ld)  score (%d) record (%d)\n\n",
         packPtr->m_size,
         restPtr->m_size,
         tbl_GetCurrentScore(),
         dd_GetHighScore());
}

static void DisplayHelp(void)
{
  printf("help\n");
  printf("%s", ctrl_GetHelp());
  printf("\n");
}

static void DisplayVictory(void)
{
  enum
  {
    CaWinSz = 15
  };
  static const char* CaWin[CaWinSz] = {
    "sA   2 c Q4  s7  3\n", "s3   J cJ  K sQ  4\n",
    "s Q 5  c9  2 s5  J\n", "s  6   cQ  6 s8  K\n",
    "s  7   c8  J sJ  3\n", "s  K   c3  A sJ  6\n",
    "s  8   c 57  s 9A \n", "\n",
    "hJ   Q 8 d3   A h0\n", "h2   7 A d2   7 s0\n",
    "hA   3 9 d86  K d0\n", "h4 2 K 3 dJ 5 J c0\n",
    "h7 6 8 Q d7  74 hA\n", "hQ3 4J 7 d2   9  \n",
    "h5   9 J dQ   6 cQ\n"
  };

  printf("\n");
  for (int i = 0; i < CaWinSz; i++) {
    PrintCardArt(CaWin[i]);
  }
  printf("\n");
}

static const char* TypeMark(Card_t card)
{
  switch (card.m_type) {
    case CT_ACE: return "A"; break;
    case CT_TWO: return "2"; break;
    case CT_THREE: return "3"; break;
    case CT_FOUR: return "4"; break;
    case CT_FIVE: return "5"; break;
    case CT_SIX: return "6"; break;
    case CT_SEVEN: return "7"; break;
    case CT_EIGHT: return "8"; break;
    case CT_NINE: return "9"; break;
    case CT_TEN: return "10"; break;
    case CT_JACK: return "J"; break;
    case CT_QUEEN: return "Q"; break;
    case CT_KING: return "K"; break;
    default: return "0"; break;
  }

  return "?!";
}

static const char* ColorMark(Card_t card)
{
  switch (card.m_color) {
    case CC_SPADES: return "♠"; break;
    case CC_HEARTS: return "♥"; break;
    case CC_CLUBS: return "♣"; break;
    case CC_DIAMONDS: return "♦"; break;
    default: return "?"; break;
  }

  return "!";
}
