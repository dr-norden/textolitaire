#include "controls.h"

#define KEYS 18

static char DefaultKeys[KEYS];
static char SecondaryKeys[KEYS];

void initControls() {
    DefaultKeys[cmd_none]   = '\0';
    DefaultKeys[cmd_exit]   = 'q';
    DefaultKeys[cmd_reset]  = 'r';
    DefaultKeys[cmd_help]   = 'h';
    DefaultKeys[cmd_scheme] = 's';
    DefaultKeys[cmd_next]   = ' ';
    DefaultKeys[cmd_pack]   = 'p';
    DefaultKeys[cmd_color0] = 'a';
    DefaultKeys[cmd_color1] = 'b';
    DefaultKeys[cmd_color2] = 'c';
    DefaultKeys[cmd_color3] = 'd';
    DefaultKeys[cmd_desk0]  = 'i';
    DefaultKeys[cmd_desk1]  = 'j';
    DefaultKeys[cmd_desk2]  = 'k';
    DefaultKeys[cmd_desk3]  = 'l';
    DefaultKeys[cmd_desk4]  = 'm';
    DefaultKeys[cmd_desk5]  = 'n';
    DefaultKeys[cmd_desk6]  = 'o';

    SecondaryKeys[cmd_none]     = DefaultKeys[cmd_none];
    SecondaryKeys[cmd_exit]     = 'x';
    SecondaryKeys[cmd_reset]    = DefaultKeys[cmd_reset];
    SecondaryKeys[cmd_help]     = '/';
    SecondaryKeys[cmd_scheme]   = DefaultKeys[cmd_scheme];
    SecondaryKeys[cmd_next]     = DefaultKeys[cmd_next];
    SecondaryKeys[cmd_pack]     = '\n';
    SecondaryKeys[cmd_color0]   = DefaultKeys[cmd_color0];
    SecondaryKeys[cmd_color1]   = DefaultKeys[cmd_color1];
    SecondaryKeys[cmd_color2]   = DefaultKeys[cmd_color2];
    SecondaryKeys[cmd_color3]   = DefaultKeys[cmd_color3];
    SecondaryKeys[cmd_desk0]    = DefaultKeys[cmd_desk0];
    SecondaryKeys[cmd_desk1]    = DefaultKeys[cmd_desk1];
    SecondaryKeys[cmd_desk2]    = DefaultKeys[cmd_desk2];
    SecondaryKeys[cmd_desk3]    = DefaultKeys[cmd_desk3];
    SecondaryKeys[cmd_desk4]    = DefaultKeys[cmd_desk4];
    SecondaryKeys[cmd_desk5]    = DefaultKeys[cmd_desk5];
    SecondaryKeys[cmd_desk6]    = DefaultKeys[cmd_desk6];
}


enum Command keyToCmd(char c) {
    for (enum Command cmd = cmd_none; cmd <= cmd_desk6; cmd++) {
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


const char * getHelp() {
    return
        " space      .. draw next card\n"
        " double key .. move to color stack or cancel action\n"
        " P or enter .. choose current card from the pack\n"
        " A-D        .. choose color stack A-D\n"
        " I-O        .. chose desk stack I-O\n"
        " T          .. move to color stack\n"
        " H or ?     .. show/hide help\n"
        " S          .. change text color scheme\n"
        " R          .. restart game\n"
        " Q          .. quit game\n"
        ;
}


