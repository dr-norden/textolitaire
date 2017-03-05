#ifndef __SOL_H__
#define __SOL_H__

#include <stdbool.h>
#include "controls.h"


/* Initialize new game */
void initNewGame();

/* Quit/restart game based on the command from the keyboard.
 * Return true if an action has been taken.
 */
bool controlGame(enum Command cmd);


/* Check if the game has been won and take an appropriate action.
 * Return true if an action has been taken.
 */
bool checkVictory();

#endif
