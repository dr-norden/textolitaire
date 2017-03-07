#include "sol.h"
#include "getche.h"
#include "display.h"
#include "controls.h"
#include "table.h"


static bool toExit = false;


int main(int argc, const char **argv) {
    initNewGame();

    while (!toExit) {
        enum Command cmd;
        if (checkVictory()) {
            // restart or exit game
            continue;
        }
        displayAll();
        setMessage("");

        cmd = keyToCmd(getch());

        if (controlGame(cmd) || controlDisplay(cmd) || controlTable(cmd)) {
            continue;
        } else {
            setMessage("!! Invalid key");
        }
    }
    return 0;
}


void initNewGame() {
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
        enum Command cmd;
        setMessage("CONGRATULATIONS, YOU HAVE WON!!!");
        displayAll(cmd_none);
        cmd = keyToCmd(getch());
        if (cmd == cmd_exit) {
            toExit = true;
        } else {
            initNewGame();
        }
        return true;
    }
    return false;
}


