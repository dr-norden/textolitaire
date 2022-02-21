#include "input.h"
#include <ncurses.h>

//----------------------------------------------------------------------------
// Public function implementation
//----------------------------------------------------------------------------

char input_ReadKey(void)
{
  char c = '\0';
  int i = getch();
  if (i > 0) {
    c = (char)i;
  }
  return c;
}