/* This module is used for test purposes only */

#include "controls.h"
#include "input.h"
#include <stdlib.h>
#include <unistd.h>

//----------------------------------------------------------------------------
// Public function implementation
//----------------------------------------------------------------------------

char input_ReadKey(void)
{
  ECommand_t cmd = rand() % NUM_COMMANDS;
  sleep(1);
  return ctrl_GetCmdKey(cmd);
}