#ifndef __TABLE_H__
#define __TABLE_H__

#include <stdbool.h>
#include "solstack.h"

#define CARDS 52
#define STACKS 7
#define COLORS 4

/* Setup table for new game */
void initTable();

/* Return true, if there's a winning combination on the table,
 * i.e. if there's a king on top of each color stack. 
 */
bool isVictory();

/* Draw next card from the pack */
void nextCard();

/* Move top of the pack into color stack if possible.
 * Return true on success. 
 */
bool movePackToColors();

/* Move top of the pack into given desk stack if possible.
 * Return true on success. 
 */
bool movePackToDesk(int stackNum);

/* Move top of given desk stack into color stack if possible.
 * Return true on success. 
 */
bool moveDeskToColors(int stackNum);

/* Move top of given color stack into given desk stack if possible.
 * Return true on success. 
 */
bool moveColorsToDesk(int srcNum, int tgtNum);

/* Move chain of cards from one desk stack to another if possible.
 * Return true on success.
 */
bool moveDeskToDesk(int srcNum, int tgtNum);


/* Return the unrevealed pack */
struct SolStack * getPack();

/* Return the revealed pack */
struct SolStack * getRest();

/* Return the array of color stacks */
struct SolStack * getColStacks();

/* Return the array of desk stacks */
struct SolStack * getDeskStacks();


#endif
