#ifndef __TABLE_H__
#define __TABLE_H__

#include "controls.h"
#include "solstack.h"
#include <stdbool.h>

#define NUM_STACKS 7

/* Setup table for new game */
void tbl_Init(void);

/*
 * Return true, if there's a winning combination on the table,
 * i.e. if there's a king on top of each color stack.
 */
bool tbl_IsVictory(void);

/* Draw next card from the pack */
void tbl_DrawNextCard(void);

/*
 * Move top of the pack into color stack if possible.
 * Return true on success.
 */
bool tbl_MovePackToColors(void);

/*
 * Move top of the pack into given desk stack if possible.
 * Return true on success.
 */
bool tbl_MovePackToDesk(int stackIdx);

/*
 * Move top of given desk stack into color stack if possible.
 * Return true on success.
 */
bool tbl_MoveDeskToColors(int stackIdx);

/*
 * Move top of given color stack into given desk stack if possible.
 * Return true on success.
 */
bool tbl_MoveColorsToDesk(int sourceIdx, int targetIdx);

/*
 * Move chain of cards from one desk stack to another if possible.
 * Return true on success.
 */
bool tbl_MoveDeskToDesk(int sourceIdx, int targetIdx);

/*
 * Return true if given desk-to-desk move move would reveal
 * a new card or vacate an empty space. The latter does not
 * apply for stack with a king on top, as the move would
 * just mean trading one empty space for another.
 */
bool tbl_IsRevealingDeskMove(int sourceIdx, int targetIdx);

/* Return the unrevealed pack */
const SolStack_t* tbl_GetPack(void);

/* Return the revealed pack */
const SolStack_t* tbl_GetRest(void);

/* Return the array of color stacks */
const SolStack_t* tbl_GetColorStacks(void);

/* Return the array of desk stacks */
const SolStack_t* tbl_GetDeskStacks(void);

/* Return game score */
int tbl_GetCurrentScore(void);

#endif
