#ifndef __CONTROLS_H__
#define __CONTROLS_H__

#include <stdbool.h>


/* List of game commands  */
enum Command {
    cmd_none,       // do nothing
    cmd_exit,       // exit game
    cmd_reset,      // restart game
    cmd_help,       // toggle help
    cmd_scheme,     // change color scheme
    cmd_next,       // draw next card
    cmd_pack,       // select actual card from the pack
    cmd_color0,     // select color stack 0
    cmd_color1,     // select color stack 1
    cmd_color2,     // select color stack 2
    cmd_color3,     // select color stack 3
    cmd_desk0,      // select desk stack 0
    cmd_desk1,      // select desk stack 1
    cmd_desk2,      // select desk stack 2
    cmd_desk3,      // select desk stack 3
    cmd_desk4,      // select desk stack 4
    cmd_desk5,      // select desk stack 5
    cmd_desk6,      // select desk stack 6
    cmd_hack        // super secret key
};

/* Initialize keyboard controls */
void initControls();

/* Return true if given command selects a desk stack */
bool isCmdDesk(enum Command cmd);

/* Return true if given command selects a color stack */
bool isCmdColor(enum Command cmd);

/* Translate keyboard characted to command */
enum Command keyToCmd(char key);

/* Return help string for the game controls */
const char * getHelp();

#endif
