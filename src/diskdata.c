#include "diskdata.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define RET_OK 0

static int hiScore = 0;
static int colorSchemeIdx = 0;
static const char * subDirName = ".textolitaire";
static const char * fileName = "data.txt";
static const char * hiScoreStr = "hiscore:";
static const char * colorSchemeIdxStr = "colorschemeidx:";


FILE *openDataFile(const char *mode) {
    char filePath[256];
    struct stat sb;
    const char *homeDir = getenv("HOME");

    if (homeDir) {
        sprintf(filePath, "%s/%s", homeDir, subDirName);
        if (stat(filePath, &sb) == RET_OK && S_ISDIR(sb.st_mode)) {
            // directory exits
        } else {
            if (mkdir(filePath, 0750) != RET_OK) {
                return NULL; // failed to create directory
            }
        }

        sprintf(filePath, "%s/%s/%s", homeDir, subDirName, fileName);
        return fopen(filePath, mode);
    }

    return NULL;
}


void initDiskData() {
    char buffer[1024];
    int number;
    FILE *fp = openDataFile("r");

    if (fp) {
        while (fscanf(fp, "%s%d", buffer, &number) != EOF) {
            //printf("string: '%s', number: '%d'\n", buffer, number);
            if (strcmp(buffer, hiScoreStr) == RET_OK) {
                hiScore = number;
            } else if (strcmp(buffer, colorSchemeIdxStr) == RET_OK) {
                colorSchemeIdx = number;
            }
        }
    }
}


void saveDiskData() {
    FILE *fp = openDataFile("w");
    if (fp) {
        fprintf(fp, "%s %d\n", hiScoreStr, hiScore);
        fprintf(fp, "%s %d\n", colorSchemeIdxStr, colorSchemeIdx);
        fclose(fp);
    }
}

int getHiScore() {
    return hiScore;
}

void setHiScore(int score) {
    hiScore = score;
}

int getColorSchemeIdx() {
    return colorSchemeIdx;
}

void setColorSchemeIdx(int index) {
    colorSchemeIdx = index;
}
