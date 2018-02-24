#include "config.h"
#include "controls.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#define KEYS 19

static char DefaultKeys[KEYS];
static char SecondaryKeys[KEYS];
static char Help[1024];

/* Generate help string from initialized controls */
static void generateHelp();

void initControls() {
    DefaultKeys[cmd_none]   = '\0';
    DefaultKeys[cmd_exit]   = CMD_PRI_EXIT_GAME;
    DefaultKeys[cmd_reset]  = CMD_PRI_NEW_GAME;
    DefaultKeys[cmd_help]   = CMD_PRI_SHOW_HELP;
    DefaultKeys[cmd_scheme] = CMD_PRI_CHANGE_SCHEME;
    DefaultKeys[cmd_next]   = CMD_PRI_NEXT_CARD;
    DefaultKeys[cmd_pack]   = CMD_PRI_SELECT_PACK;
    DefaultKeys[cmd_color0] = CMD_PRI_SELECT_COLOR1;
    DefaultKeys[cmd_color1] = CMD_PRI_SELECT_COLOR2;
    DefaultKeys[cmd_color2] = CMD_PRI_SELECT_COLOR3;
    DefaultKeys[cmd_color3] = CMD_PRI_SELECT_COLOR4;
    DefaultKeys[cmd_desk0]  = CMD_PRI_SELECT_DESK1;
    DefaultKeys[cmd_desk1]  = CMD_PRI_SELECT_DESK2;
    DefaultKeys[cmd_desk2]  = CMD_PRI_SELECT_DESK3;
    DefaultKeys[cmd_desk3]  = CMD_PRI_SELECT_DESK4;
    DefaultKeys[cmd_desk4]  = CMD_PRI_SELECT_DESK5;
    DefaultKeys[cmd_desk5]  = CMD_PRI_SELECT_DESK6;
    DefaultKeys[cmd_desk6]  = CMD_PRI_SELECT_DESK7;
    DefaultKeys[cmd_lazy]   = '.';
    DefaultKeys[cmd_hack]   = '+';

    SecondaryKeys[cmd_none]   = '\0';
    SecondaryKeys[cmd_exit]   = CMD_SEC_EXIT_GAME;
    SecondaryKeys[cmd_reset]  = CMD_SEC_NEW_GAME;
    SecondaryKeys[cmd_help]   = CMD_SEC_SHOW_HELP;
    SecondaryKeys[cmd_scheme] = CMD_SEC_CHANGE_SCHEME;
    SecondaryKeys[cmd_next]   = CMD_SEC_NEXT_CARD;
    SecondaryKeys[cmd_pack]   = CMD_SEC_SELECT_PACK;
    SecondaryKeys[cmd_color0] = CMD_SEC_SELECT_COLOR1;
    SecondaryKeys[cmd_color1] = CMD_SEC_SELECT_COLOR2;
    SecondaryKeys[cmd_color2] = CMD_SEC_SELECT_COLOR3;
    SecondaryKeys[cmd_color3] = CMD_SEC_SELECT_COLOR4;
    SecondaryKeys[cmd_desk0]  = CMD_SEC_SELECT_DESK1;
    SecondaryKeys[cmd_desk1]  = CMD_SEC_SELECT_DESK2;
    SecondaryKeys[cmd_desk2]  = CMD_SEC_SELECT_DESK3;
    SecondaryKeys[cmd_desk3]  = CMD_SEC_SELECT_DESK4;
    SecondaryKeys[cmd_desk4]  = CMD_SEC_SELECT_DESK5;
    SecondaryKeys[cmd_desk5]  = CMD_SEC_SELECT_DESK6;
    SecondaryKeys[cmd_desk6]  = CMD_SEC_SELECT_DESK7;
    SecondaryKeys[cmd_lazy]   = '.';
    SecondaryKeys[cmd_hack]   = '+';

    generateHelp();
}


enum Command keyToCmd(char c) {
    for (enum Command cmd = cmd_none; cmd <= cmd_hack; cmd++) {
        if (DefaultKeys[cmd] == c || SecondaryKeys[cmd] == c) {
            return cmd;
        }
    }

    return cmd_none;
}


bool isCmdDesk(enum Command cmd) {
    return cmd >= cmd_desk0 && cmd <= cmd_desk6;
}

bool isCmdColor(enum Command cmd) {
    return cmd >= cmd_color0 && cmd <= cmd_color3;
}

char getCmdKey(enum Command cmd) {
    return DefaultKeys[cmd];
}


void formatKey(char *buffer, char key) {
    switch (key) {
        case ' ':    sprintf(buffer, "space"); break;
        case '\n':   sprintf(buffer, "enter"); break;
        case '\b':   sprintf(buffer, "bkspc"); break;
        case '\t':   sprintf(buffer, "tab");   break;
        case '\x1B': sprintf(buffer, "esc");   break;
        case '\x7f': sprintf(buffer, "del");   break;
        default: sprintf(buffer, "%c", toupper(key)); break;
    }
}


void formatCmdHelp(char *buffer, enum Command cmd, const char *description) {
    char keyBuf[20];
    formatKey(keyBuf, DefaultKeys[cmd]);

    if (DefaultKeys[cmd] != SecondaryKeys[cmd]) {
        char keyBuf2[10];
        formatKey(keyBuf2, SecondaryKeys[cmd]);
        strcat(keyBuf, " or ");
        strcat(keyBuf, keyBuf2);
    }

    sprintf(&buffer[strlen(buffer)], " %-10s .. %s\n", keyBuf, description);
}

void generateHelp() {
    sprintf(Help, " double key .. move to color stack or cancel action\n");
    formatCmdHelp(Help, cmd_next,   "draw next card");
    formatCmdHelp(Help, cmd_pack,   "choose current card from the pack");
    formatCmdHelp(Help, cmd_color0, "choose stack of spades");
    formatCmdHelp(Help, cmd_color1, "choose stack of hearts");
    formatCmdHelp(Help, cmd_color2, "choose stack of clubs");
    formatCmdHelp(Help, cmd_color3, "choose stack of diamonds");
    formatCmdHelp(Help, cmd_desk0,  "choose desk stack 1");
    formatCmdHelp(Help, cmd_desk1,  "choose desk stack 2");
    formatCmdHelp(Help, cmd_desk2,  "choose desk stack 3");
    formatCmdHelp(Help, cmd_desk3,  "choose desk stack 4");
    formatCmdHelp(Help, cmd_desk4,  "choose desk stack 5");
    formatCmdHelp(Help, cmd_desk5,  "choose desk stack 6");
    formatCmdHelp(Help, cmd_desk6,  "choose desk stack 7");
    formatCmdHelp(Help, cmd_scheme, "change text color scheme");
    formatCmdHelp(Help, cmd_help,   "show/hide help");
    formatCmdHelp(Help, cmd_reset,  "restart game");
    formatCmdHelp(Help, cmd_exit,   "quit game");
}

const char * getHelp() {
    return Help;
}


