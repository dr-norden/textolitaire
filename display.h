#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "card.h"
#include "solstack.h"
#include "controls.h"

/* Escape sequences to change text color in console */
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define COLOR_SCHEMES 3

/* Structure to hold all colors used in the game */
struct ColorScheme {
    const char *m_normal;
    const char *m_red;
    const char *m_black;
    const char *m_back;
};

/* Visualize all game vitals on the stdout */
void displayAll();


/* Set the status/error message to display */
void setMessage(const char *message);


/* Print card to stdout */
void printCard(struct Card card);


/* Print stack to stdout */
void printStack(struct SolStack *pStack);


/* Print the top card of the stack or some text representation of empty stack
 * to stdout.
 */
void printTop(struct SolStack *pStack);


/* Change display properties based on the command from the keyboard.
 * Returns true if an action has been taken.
 */
bool controlDisplay(enum Command cmd);


#endif
