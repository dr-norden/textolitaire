#include "game.h"
#include "controls.h"
#include "diskdata.h"
#include "display.h"
#include "input.h"
#include "table.h"
#include <stdbool.h>

//----------------------------------------------------------------------------
// Private definitions/variables
//----------------------------------------------------------------------------

static bool ToExit = false;
static bool FirstFail = true;
static ECommand_t ActiveCmd = CMD_NONE;

//----------------------------------------------------------------------------
// Private function prototypes
//----------------------------------------------------------------------------

/* Initialize new game */
static void InitNewGame(void);

/*
 * Helper functions to control game based on the command from the keyboard.
 * Return true if an action has been taken.
 */
static bool ControlGame(ECommand_t cmd);
static bool ControlDisplay(ECommand_t cmd);
static bool ControlTable(ECommand_t cmd);

/*
 * Check if the game has been won and take an appropriate action.
 * Return true if an action has been taken.
 */
static bool CheckVictory(void);

/* Perform a legal move if possible. Return true on success */
static bool AutoMove(void);

/* Shorthands */
static bool IsCmdDesk(ECommand_t cmd);
static bool IsCmdColor(ECommand_t cmd);
static int GetStackIdx(ECommand_t cmd);

//----------------------------------------------------------------------------
// Public function implementation
//----------------------------------------------------------------------------

void game_Run(void)
{
  dd_Init();
  InitNewGame();

  while (!ToExit) {
    ECommand_t cmd;
    if (CheckVictory()) {
      // restart or exit game
      continue;
    }
    dspl_Render(ActiveCmd);
    dspl_SetMessage("");

    cmd = ctrl_KeyToCmd(input_ReadKey());

    if (ControlGame(cmd) || ControlDisplay(cmd) || ControlTable(cmd)) {
      continue;
    } else {
      if (FirstFail) {
        dspl_SetMessage("!! Invalid key - press H for help");
        FirstFail = false;
      } else {
        dspl_SetMessage("!! Invalid key");
      }
    }
  }

  dd_Save();
  dspl_Clear();
}

//----------------------------------------------------------------------------
// Private function implementation
//----------------------------------------------------------------------------

static void InitNewGame(void)
{
  ToExit = false;
  ActiveCmd = CMD_NONE;
  tbl_Init();
  ctrl_Init();
  dspl_Init();
}

static bool ControlGame(ECommand_t cmd)
{
  if (cmd == CMD_EXIT) {
    ToExit = true;
    return true;
  }

  if (cmd == CMD_RESET) {
    InitNewGame();
    return true;
  }

  return false;
}

static bool ControlDisplay(ECommand_t cmd)
{
  if (cmd == CMD_HELP) {
    dspl_ShowHelp();
    return true;
  }

  if (cmd == CMD_HACK) {
    dspl_HackerMode();
    return true;
  }

  if (cmd == CMD_SCHEME) {
    dspl_ChangeColorScheme();
    return true;
  }

  return false;
}

static bool ControlTable(ECommand_t cmd)
{
  bool rv = true;

  if (ActiveCmd == CMD_NONE) {
    if (cmd == CMD_NEXT) {
      tbl_DrawNextCard();
    } else if (cmd == CMD_PACK || IsCmdDesk(cmd) || IsCmdColor(cmd)) {
      // want one more key to complete action
      ActiveCmd = cmd;
    } else if (cmd == CMD_LAZY) {
      rv = AutoMove();
    } else {
      return false;
    }

  } else {
    rv = false;

    if (IsCmdColor(cmd) || cmd == ActiveCmd) {
      // move up or cancel action
      if (CMD_PACK == ActiveCmd) {
        rv = tbl_MovePackToColors();
      } else if (IsCmdDesk(ActiveCmd)) {
        rv = tbl_MoveDeskToColors(GetStackIdx(cmd));
      } else {
        rv = false;
      }
      rv = rv || ActiveCmd == cmd;

    } else if (IsCmdDesk(cmd)) {
      // move to desk
      if (CMD_PACK == ActiveCmd) {
        rv = tbl_MovePackToDesk(GetStackIdx(cmd));
      } else if (IsCmdDesk(ActiveCmd)) {
        rv = tbl_MoveDeskToDesk(GetStackIdx(ActiveCmd), GetStackIdx(cmd));
      } else if (IsCmdColor(ActiveCmd)) {
        rv = tbl_MoveColorsToDesk(GetStackIdx(cmd), GetStackIdx(cmd));
      }
    } else if (CMD_PACK == cmd) {
      rv = false;
    } else {
      // unsupported command
      ActiveCmd = CMD_NONE;
      return false;
    }

    ActiveCmd = CMD_NONE;
  }

  if (!rv) {
    dspl_SetMessage("!! Invalid move");
  }
  return true;
}

static bool AutoMove(void)
{
  // try to move cards between desk stacks
  for (int i = 0; i < NUM_STACKS; i++) {
    for (int j = 0; j < NUM_STACKS; j++) {
      if ((i != j) && tbl_IsRevealingDeskMove(i, j) &&
          tbl_MoveDeskToDesk(i, j)) {
        return true;
      }
    }
  }

  // try to move the pack card to any desk stack
  for (int i = 0; i < NUM_STACKS; i++) {
    if (tbl_MovePackToDesk(i)) {
      return true;
    }
  }

  // try to move any desk card to colors
  for (int i = 0; i < NUM_STACKS; i++) {
    if (tbl_MoveDeskToColors(i)) {
      return true;
    }
  }

  // try to move the pack card to colors
  if (tbl_MovePackToColors()) {
    return true;
  }

#ifdef TURBO
  // if everything else fails, draw next card
  tbl_DrawNextCard();
  return true;
#else
  return false;
#endif
}

static bool CheckVictory(void)
{
  if (tbl_IsVictory()) {
    ECommand_t cmd;

    int score = tbl_GetCurrentScore();
    if (score > dd_GetHighScore()) {
      dspl_SetMessage("CONGRATULATIONS, YOU HAVE WON WITH NEW TOP SCORE!!!");
    } else {
      dspl_SetMessage("CONGRATULATIONS, YOU HAVE WON!!!");
    }

    dspl_Render(CMD_NONE);

    if (score > dd_GetHighScore()) {
      dd_SetHighScore(score);
    }

    dd_Save();

    cmd = ctrl_KeyToCmd(dspl_Victory());
    if (cmd == CMD_EXIT) {
      ToExit = true;
    } else {
      InitNewGame();
      dspl_SetMessage("");
    }
    return true;
  }
  return false;
}

static bool IsCmdDesk(ECommand_t cmd)
{
  return cmd >= CMD_DESK0 && cmd <= CMD_DESK6;
}

static bool IsCmdColor(ECommand_t cmd)
{
  return cmd >= CMD_COLOR0 && cmd <= CMD_COLOR3;
}

static int GetStackIdx(ECommand_t cmd)
{
  if (IsCmdDesk(cmd)) {
    return cmd - CMD_DESK0;
  }

  if (IsCmdColor(cmd)) {
    return cmd - CMD_COLOR0;
  }

  return 0;
}
