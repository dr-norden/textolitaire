#include "input.h"
#include <stdio.h>
#include <termios.h>

//----------------------------------------------------------------------------
// Private definitions/variables
//----------------------------------------------------------------------------

static struct termios OldIos = { 0 };
static struct termios NewIos = { 0 };

//----------------------------------------------------------------------------
// Private function prototypes
//----------------------------------------------------------------------------

/* Initialize new terminal i/o settings with no echo */
static void InitTermiosNoEcho(void);

/* Restore old terminal i/o settings */
static void ResetTermios(void);

//----------------------------------------------------------------------------
// Public function implementation
//----------------------------------------------------------------------------

char input_ReadKey(void)
{
  char c;
  InitTermiosNoEcho();
  c = getchar();
  ResetTermios();
  return c;
}

//----------------------------------------------------------------------------
// Private function implementation
//----------------------------------------------------------------------------

/* Initialize new terminal I/O settings */
static void InitTermiosNoEcho(void)
{
  tcgetattr(0, &OldIos);
  NewIos = OldIos;
  NewIos.c_lflag &= ~ICANON; // disable buffered I/O
  NewIos.c_lflag &= ~ECHO;   // disable echo mode
  tcsetattr(0, TCSANOW, &NewIos);
}

/* Restore old terminal i/o settings */
static void ResetTermios(void)
{
  tcsetattr(0, TCSANOW, &OldIos);
}
