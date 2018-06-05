#include "sol.h"
#include "getche.h"
#include "display.h"
#include "diskdata.h"
#include "controls.h"
#include "table.h"


static bool toExit = false;


int main(int argc, const char **argv) {
    initDiskData();
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

    saveDiskData();
    clearDisplay();

    return 0;
}


void initNewGame() {
    toExit = false;
    initTable();
    initControls();
    initDisplay();
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

        int score = getScore();
        if (score > getHiScore()) {
            setMessage("CONGRATULATIONS, YOU HAVE WON WITH NEW TOP SCORE!!!");
        } else {
            setMessage("CONGRATULATIONS, YOU HAVE WON!!!");
        }

        displayAll(cmd_none);

        if (score > getHiScore()) {
            setHiScore(score);
        }

        saveDiskData();

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


