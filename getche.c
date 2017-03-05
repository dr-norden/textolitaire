#include "getche.h"
#include <termios.h>
#include <stdio.h>


static struct termios oldIos, newIos;

/* Initialize new terminal i/o settings */
void initTermios(int echo) {
  tcgetattr(0, &oldIos); /* grab old terminal i/o settings */
  newIos = oldIos; /* make new settings same as old settings */
  newIos.c_lflag &= ~ICANON; /* disable buffered i/o */
  newIos.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &newIos); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios() {
  tcsetattr(0, TCSANOW, &oldIos);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) {
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

char getch() {
  return getch_(0);
}

char getche() {
  return getch_(1);
}
