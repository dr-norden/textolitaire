#include "sol.h"
#include "getche.h"
#include "display.h"
#include "controls.h"
#include "table.h"


static enum Command cmd0 = cmd_none;
static enum Command cmd1 = cmd_none;
static bool toExit = false;


int main(int argc, const char **argv) {
    initNewGame();

    while (!toExit) {
        bool rv = true;
        if (checkVictory()) {
            // restart or exit game
            continue;
        }
        displayAll(cmd0);


        setMessage("");

        if (!cmd0) {
            cmd0 = keyToCmd(getch());
            cmd1 = cmd_none;

            if (controlGame(cmd0) || controlDisplay(cmd0)) {
                cmd0 = cmd_none;
                continue;
            } else if (cmd0 == cmd_next) {
                nextCard();
            } else if (cmd0 == cmd_pack || isCmdDesk(cmd0) || isCmdColor(cmd0)) {
                // want one more key to complete action
                cmd1 = cmd0;
            }
            else {
                setMessage("!! Invalid key");
            }
            cmd0 = cmd1;

        } else {
            rv = false;
            cmd1 = keyToCmd(getch());

            if (controlGame(cmd0) || controlDisplay(cmd0)) {
                cmd0 = cmd_none;
                continue;
            } else if (cmd0 == cmd_pack) {
                if (isCmdColor(cmd1) || cmd0 == cmd1) {
                    rv = movePackToColors();
                } else if (isCmdDesk(cmd1)) {
                    rv = movePackToDesk(cmd1-cmd_desk0);
                }
            } else if (isCmdDesk(cmd0)) {
                if (isCmdColor(cmd1) || cmd0 == cmd1) {
                    rv = moveDeskToColors(cmd0-cmd_desk0);
                } else if (isCmdDesk(cmd1) && cmd0 != cmd1) {
                    rv = moveDeskToDesk(cmd0-cmd_desk0, cmd1-cmd_desk0);
                }
            } else if (isCmdColor(cmd0)) {
                if (isCmdDesk(cmd1)) {
                    rv = moveColorsToDesk(cmd0-cmd_color0, cmd1-cmd_desk0);
                }
            } else {
                setMessage("!! Invalid key");
            }

            rv = rv || cmd0 == cmd1;
            if (!rv) {
                setMessage("!! Invalid move");
            }

            cmd0=cmd_none;
        }
    }
    return 0;
}


void initNewGame() {
    cmd0 = cmd1 = cmd_none;
    toExit = false;
    initTable();
    initControls();
}


bool controlGame(enum Command cmd) {
    if (cmd == cmd_exit) {
        toExit = true;
        return true;
    } else if (cmd == cmd_reset) {
        initNewGame();
        return true;
    }
    return false;
}


bool checkVictory() {
    if (isVictory()) {
        setMessage("CONGRATULATIONS, YOU HAVE WON!!!");
        displayAll(cmd_none);
        cmd0 = keyToCmd(getch());
        if (cmd0 == cmd_exit) {
            toExit = true;
        } else {
            initNewGame();
        }
        return true;
    }
    return false;
}


