//////////////////////////////////////////////////////////
// Change this file to modify basic controls and colors //
//////////////////////////////////////////////////////////

/* 
 * Escape sequences to change text color in console. Do not change these.
 */

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

/* 
 * Following macros define thee color schemes. Choose from the values above.
 */

#define SCHEME1_TXT  KNRM
#define SCHEME1_RED  KRED
#define SCHEME1_BLK  KBLU
#define SCHEME1_BCK  KWHT

#define SCHEME2_TXT  KCYN
#define SCHEME2_RED  KRED
#define SCHEME2_BLK  KNRM
#define SCHEME2_BCK  KWHT

#define SCHEME3_TXT  KNRM
#define SCHEME3_RED  KRED
#define SCHEME3_BLK  KGRN
#define SCHEME3_BCK  KCYN


/* 
 * Following macros define primary and secondary keys for given actions.
 */

#define CMD_PRI_EXIT_GAME       'q'
#define CMD_PRI_NEW_GAME        'r'
#define CMD_PRI_SHOW_HELP       'h'
#define CMD_PRI_CHANGE_SCHEME   's'
#define CMD_PRI_NEXT_CARD       ' '
#define CMD_PRI_SELECT_PACK     'p'
#define CMD_PRI_SELECT_COLOR1   'a'
#define CMD_PRI_SELECT_COLOR2   'b'
#define CMD_PRI_SELECT_COLOR3   'c'
#define CMD_PRI_SELECT_COLOR4   'd'
#define CMD_PRI_SELECT_DESK1    'i'
#define CMD_PRI_SELECT_DESK2    'j'
#define CMD_PRI_SELECT_DESK3    'k'
#define CMD_PRI_SELECT_DESK4    'l'
#define CMD_PRI_SELECT_DESK5    'm'
#define CMD_PRI_SELECT_DESK6    'n'
#define CMD_PRI_SELECT_DESK7    'o'


#define CMD_SEC_EXIT_GAME       'x'
#define CMD_SEC_NEW_GAME        'r'
#define CMD_SEC_SHOW_HELP       '?'
#define CMD_SEC_CHANGE_SCHEME   's'
#define CMD_SEC_NEXT_CARD       ' '
#define CMD_SEC_SELECT_PACK     '\n'
#define CMD_SEC_SELECT_COLOR1   'a'
#define CMD_SEC_SELECT_COLOR2   'b'
#define CMD_SEC_SELECT_COLOR3   'c'
#define CMD_SEC_SELECT_COLOR4   'd'
#define CMD_SEC_SELECT_DESK1    '1'
#define CMD_SEC_SELECT_DESK2    '2'
#define CMD_SEC_SELECT_DESK3    '3'
#define CMD_SEC_SELECT_DESK4    '4'
#define CMD_SEC_SELECT_DESK5    '5'
#define CMD_SEC_SELECT_DESK6    '6'
#define CMD_SEC_SELECT_DESK7    '7'
