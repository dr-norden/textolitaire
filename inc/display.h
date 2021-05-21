#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "card.h"
#include "solstack.h"
#include "controls.h"

#define COLOR_SCHEMES 3

/* Structure to hold all colors used in the game */
struct ColorScheme {
    const char *m_normal;
    const char *m_red;
    const char *m_black;
    const char *m_back;
};


/* Initialize display variables. Should be called after initDiskData() */
void initDisplay();

/* Clear display data. Should be called before application close */
void clearDisplay();

/* Visualize all game vitals on the stdout */
void displayAll();


/* Set the status/error message to display */
void setMessage(const char *message);

/* Change display properties based on the command from the keyboard.
 * Returns true if an action has been taken.
 */
bool controlDisplay(enum Command cmd);


#endif
