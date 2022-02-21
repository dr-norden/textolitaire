#include "controls.h"
#include "config.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

//----------------------------------------------------------------------------
// Private definitions/variables
//----------------------------------------------------------------------------

static char DefaultKeys[NUM_COMMANDS];
static char SecondaryKeys[NUM_COMMANDS];
static char HelpString[1024];

//----------------------------------------------------------------------------
// Private function prototypes
//----------------------------------------------------------------------------

/* Helper functions to generate help */
static void FormatKey(char* buffer, char key);
static void FormatCmdHelp(char* buffer,
                          ECommand_t cmd,
                          const char* description);
static void GenerateHelp(void);

//----------------------------------------------------------------------------
// Public function implementation
//----------------------------------------------------------------------------

void ctrl_Init(void)
{
  DefaultKeys[CMD_NONE] = '\0';
  DefaultKeys[CMD_EXIT] = KEY_PRI_EXIT_GAME;
  DefaultKeys[CMD_RESET] = KEY_PRI_NEW_GAME;
  DefaultKeys[CMD_HELP] = KEY_PRI_SHOW_HELP;
  DefaultKeys[CMD_SCHEME] = KEY_PRI_CHANGE_SCHEME;
  DefaultKeys[CMD_NEXT] = KEY_PRI_NEXT_CARD;
  DefaultKeys[CMD_PACK] = KEY_PRI_SELECT_PACK;
  DefaultKeys[CMD_COLOR0] = KEY_PRI_SELECT_COLOR1;
  DefaultKeys[CMD_COLOR1] = KEY_PRI_SELECT_COLOR2;
  DefaultKeys[CMD_COLOR2] = KEY_PRI_SELECT_COLOR3;
  DefaultKeys[CMD_COLOR3] = KEY_PRI_SELECT_COLOR4;
  DefaultKeys[CMD_DESK0] = KEY_PRI_SELECT_DESK1;
  DefaultKeys[CMD_DESK1] = KEY_PRI_SELECT_DESK2;
  DefaultKeys[CMD_DESK2] = KEY_PRI_SELECT_DESK3;
  DefaultKeys[CMD_DESK3] = KEY_PRI_SELECT_DESK4;
  DefaultKeys[CMD_DESK4] = KEY_PRI_SELECT_DESK5;
  DefaultKeys[CMD_DESK5] = KEY_PRI_SELECT_DESK6;
  DefaultKeys[CMD_DESK6] = KEY_PRI_SELECT_DESK7;
  DefaultKeys[CMD_LAZY] = '.';
  DefaultKeys[CMD_HACK] = '+';

  SecondaryKeys[CMD_NONE] = '\0';
  SecondaryKeys[CMD_EXIT] = KEY_SEC_EXIT_GAME;
  SecondaryKeys[CMD_RESET] = KEY_SEC_NEW_GAME;
  SecondaryKeys[CMD_HELP] = KEY_SEC_SHOW_HELP;
  SecondaryKeys[CMD_SCHEME] = KEY_SEC_CHANGE_SCHEME;
  SecondaryKeys[CMD_NEXT] = KEY_SEC_NEXT_CARD;
  SecondaryKeys[CMD_PACK] = KEY_SEC_SELECT_PACK;
  SecondaryKeys[CMD_COLOR0] = KEY_SEC_SELECT_COLOR1;
  SecondaryKeys[CMD_COLOR1] = KEY_SEC_SELECT_COLOR2;
  SecondaryKeys[CMD_COLOR2] = KEY_SEC_SELECT_COLOR3;
  SecondaryKeys[CMD_COLOR3] = KEY_SEC_SELECT_COLOR4;
  SecondaryKeys[CMD_DESK0] = KEY_SEC_SELECT_DESK1;
  SecondaryKeys[CMD_DESK1] = KEY_SEC_SELECT_DESK2;
  SecondaryKeys[CMD_DESK2] = KEY_SEC_SELECT_DESK3;
  SecondaryKeys[CMD_DESK3] = KEY_SEC_SELECT_DESK4;
  SecondaryKeys[CMD_DESK4] = KEY_SEC_SELECT_DESK5;
  SecondaryKeys[CMD_DESK5] = KEY_SEC_SELECT_DESK6;
  SecondaryKeys[CMD_DESK6] = KEY_SEC_SELECT_DESK7;
  SecondaryKeys[CMD_LAZY] = '.';
  SecondaryKeys[CMD_HACK] = '+';

  GenerateHelp();
}

ECommand_t ctrl_KeyToCmd(char c)
{
  for (ECommand_t cmd = CMD_NONE; cmd < NUM_COMMANDS; cmd++) {
    if (DefaultKeys[cmd] == c || SecondaryKeys[cmd] == c) {
      return cmd;
    }
  }

  return CMD_NONE;
}

char ctrl_GetCmdKey(ECommand_t cmd)
{
  return DefaultKeys[cmd];
}

const char* ctrl_GetHelp(void)
{
  return HelpString;
}

//----------------------------------------------------------------------------
// Private function implementation
//----------------------------------------------------------------------------

static void FormatKey(char* buffer, char key)
{
  switch (key) {
    case ' ': sprintf(buffer, "space"); break;
    case '\n': sprintf(buffer, "enter"); break;
    case '\b': sprintf(buffer, "bkspc"); break;
    case '\t': sprintf(buffer, "tab"); break;
    case '\x1B': sprintf(buffer, "esc"); break;
    case '\x7f': sprintf(buffer, "del"); break;
    default: sprintf(buffer, "%c", toupper(key)); break;
  }
}

static void FormatCmdHelp(char* buffer, ECommand_t cmd, const char* description)
{
  char keyBuf[20];
  FormatKey(keyBuf, DefaultKeys[cmd]);

  if (DefaultKeys[cmd] != SecondaryKeys[cmd]) {
    char keyBuf2[10];
    FormatKey(keyBuf2, SecondaryKeys[cmd]);
    strcat(keyBuf, " or ");
    strcat(keyBuf, keyBuf2);
  }

  sprintf(&buffer[strlen(buffer)], "  %-10s .. %s\n", keyBuf, description);
}

static void GenerateHelp(void)
{
  sprintf(HelpString, "  double key .. move to color stack or cancel action\n");
  FormatCmdHelp(HelpString, CMD_NEXT, "draw next card");
  FormatCmdHelp(HelpString, CMD_PACK, "choose current card from the pack");
  FormatCmdHelp(HelpString, CMD_COLOR0, "choose stack of spades");
  FormatCmdHelp(HelpString, CMD_COLOR1, "choose stack of hearts");
  FormatCmdHelp(HelpString, CMD_COLOR2, "choose stack of clubs");
  FormatCmdHelp(HelpString, CMD_COLOR3, "choose stack of diamonds");
  FormatCmdHelp(HelpString, CMD_DESK0, "choose desk stack 1");
  FormatCmdHelp(HelpString, CMD_DESK1, "choose desk stack 2");
  FormatCmdHelp(HelpString, CMD_DESK2, "choose desk stack 3");
  FormatCmdHelp(HelpString, CMD_DESK3, "choose desk stack 4");
  FormatCmdHelp(HelpString, CMD_DESK4, "choose desk stack 5");
  FormatCmdHelp(HelpString, CMD_DESK5, "choose desk stack 6");
  FormatCmdHelp(HelpString, CMD_DESK6, "choose desk stack 7");
  FormatCmdHelp(HelpString, CMD_SCHEME, "change text color scheme");
  FormatCmdHelp(HelpString, CMD_HELP, "show/hide help");
  FormatCmdHelp(HelpString, CMD_RESET, "restart game");
  FormatCmdHelp(HelpString, CMD_EXIT, "quit game");
}
