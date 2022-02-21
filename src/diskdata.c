#include "diskdata.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

//----------------------------------------------------------------------------
// Private definitions/variables
//----------------------------------------------------------------------------

#define RET_OK 0

static int HighScore = 0;
static int ColorSchemeIdx = 0;
static int ColorScheme2Idx = 0;
static const char* SubDirName = ".textolitaire";
static const char* FileName = "data.txt";
static const char* HighScoreLabel = "hiscore:";
static const char* ColorSchemeIdxLabel = "colorschemeidx:";
static const char* ColorScheme2IdxLabel = "colorscheme2idx:";

//----------------------------------------------------------------------------
// Private function prototypes
//----------------------------------------------------------------------------

static FILE* OpenDataFile(const char* mode);

//----------------------------------------------------------------------------
// Public function implementation
//----------------------------------------------------------------------------

void dd_Init(void)
{
  char buffer[1024];
  int number;
  FILE* fp = OpenDataFile("r");

  if (fp) {
    while (fscanf(fp, "%s%d", buffer, &number) != EOF) {
      // printf("string: '%s', number: '%d'\n", buffer, number);
      if (strcmp(buffer, HighScoreLabel) == RET_OK) {
        HighScore = number;
      } else if (strcmp(buffer, ColorSchemeIdxLabel) == RET_OK) {
        ColorSchemeIdx = number;
      } else if (strcmp(buffer, ColorScheme2IdxLabel) == RET_OK) {
        ColorScheme2Idx = number;
      }
    }
  }
}

void dd_Save(void)
{
  FILE* fp = OpenDataFile("w");
  if (fp) {
    fprintf(fp, "%s %d\n", HighScoreLabel, HighScore);
    fprintf(fp, "%s %d\n", ColorSchemeIdxLabel, ColorSchemeIdx);
    fprintf(fp, "%s %d\n", ColorScheme2IdxLabel, ColorScheme2Idx);
    fclose(fp);
  }
}

int dd_GetHighScore(void)
{
  return HighScore;
}

void dd_SetHighScore(int score)
{
  HighScore = score;
}

int dd_GetColorSchemeIdx(void)
{
  return ColorSchemeIdx;
}

void dd_SetColorSchemeIdx(int index)
{
  ColorSchemeIdx = index;
}

int dd_GetColorScheme2Idx(void)
{
  return ColorScheme2Idx;
}

void dd_SetColorScheme2Idx(int index)
{
  ColorScheme2Idx = index;
}

//----------------------------------------------------------------------------
// Private function implementation
//----------------------------------------------------------------------------

static FILE* OpenDataFile(const char* mode)
{
  char filePath[256];
  struct stat sb;
  const char* homeDir = getenv("HOME");

  if (homeDir) {
    sprintf(filePath, "%s/%s", homeDir, SubDirName);
    if (stat(filePath, &sb) == RET_OK && S_ISDIR(sb.st_mode)) {
      // directory exits
    } else {
      if (mkdir(filePath, 0750) != RET_OK) {
        return NULL; // failed to create directory
      }
    }

    sprintf(filePath, "%s/%s/%s", homeDir, SubDirName, FileName);
    return fopen(filePath, mode);
  }

  return NULL;
}
