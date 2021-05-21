#ifndef __DISKDATA_H__
#define __DISKDATA_H__

/* Initialize data from data file */
void initDiskData();

/* Save data to data file */
void saveDiskData();

/* Return the saved high score */
int getHiScore();

/* Set the new high score */
void setHiScore(int score);

/* Get the saved color scheme index */
int getColorSchemeIdx();

/* Set the new color scheme index */
void setColorSchemeIdx(int index);

#endif
