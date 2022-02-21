#ifndef __DISKDATA_H__
#define __DISKDATA_H__

/* Initialize data from data file */
void dd_Init(void);

/* Save data to data file */
void dd_Save(void);

/* Return the saved high score */
int dd_GetHighScore(void);

/* Set the new high score */
void dd_SetHighScore(int score);

/* Get index of the saved color scheme for Textolitaire1 */
int dd_GetColorSchemeIdx(void);

/* Set new index of the saved color scheme for Textolitaire1 */
void dd_SetColorSchemeIdx(int index);

/* Get index of the saved color scheme for Textolitaire2 */
int dd_GetColorScheme2Idx(void);

/* Set new index of the saved color scheme for Textolitaire2 */
void dd_SetColorScheme2Idx(int index);

#endif
