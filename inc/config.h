//////////////////////////////////////////////////////////
// Change this file to modify basic controls and colors //
//////////////////////////////////////////////////////////

/* Escape sequences to change text color in terminal. Do not change these. */

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

/* Following macros define six color schemes. Choose from the values above. */

#define SCHEME1_TXT KNRM
#define SCHEME1_RED KRED
#define SCHEME1_BLK KBLU
#define SCHEME1_BCK KWHT

#define SCHEME2_TXT KCYN
#define SCHEME2_RED KRED
#define SCHEME2_BLK KNRM
#define SCHEME2_BCK KWHT

#define SCHEME3_TXT KNRM
#define SCHEME3_RED KRED
#define SCHEME3_BLK KGRN
#define SCHEME3_BCK KCYN

#define SCHEME4_TXT KNRM
#define SCHEME4_RED KYEL
#define SCHEME4_BLK KCYN
#define SCHEME4_BCK KMAG

#define SCHEME5_TXT KYEL
#define SCHEME5_RED KGRN
#define SCHEME5_BLK KCYN
#define SCHEME5_BCK KNRM

#define SCHEME6_TXT KGRN
#define SCHEME6_RED KMAG
#define SCHEME6_BLK KCYN
#define SCHEME6_BCK KWHT

/* Following macros define primary and secondary keys for given actions. */

#define KEY_PRI_EXIT_GAME 'q'
#define KEY_PRI_NEW_GAME 'r'
#define KEY_PRI_SHOW_HELP 'h'
#define KEY_PRI_CHANGE_SCHEME 's'
#define KEY_PRI_NEXT_CARD ' '
#define KEY_PRI_SELECT_PACK 'p'
#define KEY_PRI_SELECT_COLOR1 'a'
#define KEY_PRI_SELECT_COLOR2 'b'
#define KEY_PRI_SELECT_COLOR3 'c'
#define KEY_PRI_SELECT_COLOR4 'd'
#define KEY_PRI_SELECT_DESK1 'i'
#define KEY_PRI_SELECT_DESK2 'j'
#define KEY_PRI_SELECT_DESK3 'k'
#define KEY_PRI_SELECT_DESK4 'l'
#define KEY_PRI_SELECT_DESK5 'm'
#define KEY_PRI_SELECT_DESK6 'n'
#define KEY_PRI_SELECT_DESK7 'o'

#define KEY_SEC_EXIT_GAME 'x'
#define KEY_SEC_NEW_GAME 'r'
#define KEY_SEC_SHOW_HELP '?'
#define KEY_SEC_CHANGE_SCHEME 's'
#define KEY_SEC_NEXT_CARD ' '
#define KEY_SEC_SELECT_PACK '\n'
#define KEY_SEC_SELECT_COLOR1 'a'
#define KEY_SEC_SELECT_COLOR2 'b'
#define KEY_SEC_SELECT_COLOR3 'c'
#define KEY_SEC_SELECT_COLOR4 'd'
#define KEY_SEC_SELECT_DESK1 '1'
#define KEY_SEC_SELECT_DESK2 '2'
#define KEY_SEC_SELECT_DESK3 '3'
#define KEY_SEC_SELECT_DESK4 '4'
#define KEY_SEC_SELECT_DESK5 '5'
#define KEY_SEC_SELECT_DESK6 '6'
#define KEY_SEC_SELECT_DESK7 '7'
