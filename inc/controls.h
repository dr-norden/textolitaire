#ifndef __CONTROLS_H__
#define __CONTROLS_H__

#include <stdbool.h>

/* List of game commands  */
typedef enum
{
  CMD_NONE = 0, // do nothing
  CMD_EXIT,     // exit game
  CMD_RESET,    // restart game
  CMD_HELP,     // toggle help
  CMD_SCHEME,   // change color scheme
  CMD_NEXT,     // draw next card
  CMD_PACK,     // select actual card from the pack
  CMD_COLOR0,   // select color stack 0
  CMD_COLOR1,   // select color stack 1
  CMD_COLOR2,   // select color stack 2
  CMD_COLOR3,   // select color stack 3
  CMD_DESK0,    // select desk stack 0
  CMD_DESK1,    // select desk stack 1
  CMD_DESK2,    // select desk stack 2
  CMD_DESK3,    // select desk stack 3
  CMD_DESK4,    // select desk stack 4
  CMD_DESK5,    // select desk stack 5
  CMD_DESK6,    // select desk stack 6
  CMD_LAZY,     // secret key for lazy gamers
  CMD_HACK,     // super secret key for hackers
  NUM_COMMANDS,
} ECommand_t;

/* Initialize keyboard controls */
void ctrl_Init(void);

/* Translate keyboard characted to command */
ECommand_t ctrl_KeyToCmd(char key);

/* Return default keyboard character for given command */
char ctrl_GetCmdKey(ECommand_t cmd);

/* Return help string for the game controls */
const char* ctrl_GetHelp(void);

#endif
