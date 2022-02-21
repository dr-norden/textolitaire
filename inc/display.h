#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "controls.h"

/* Initialize display variables. Should be called after dd_Init() */
void dspl_Init(void);

/* Clear display data. Should be called before application close */
void dspl_Clear(void);

/* Visualize all game vitals */
void dspl_Render(ECommand_t activeCmd);

/* Set the status/error message to display */
void dspl_SetMessage(const char* message);

/* Show help (or hide if already shown) */
void dspl_ShowHelp(void);

/* Enter/exit hacker mode */
void dspl_HackerMode(void);

/* Change color scheme and apply */
void dspl_ChangeColorScheme();

/*
 * Visualize victory in the game and wait for a keypress.
 * Return the read key.
 */
char dspl_Victory(void);

#endif
