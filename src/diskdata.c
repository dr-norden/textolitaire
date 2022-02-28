#include "diskdata.h"
#include <stdbool.h>
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

typedef struct
{
  int m_highScore;
  int m_colorScheme1;
  int m_colorScheme2;
} DiskData_t;

static DiskData_t StoredData = { 0 };
static DiskData_t ActiveData = { 0 };
static const char* SubDirName = ".textolitaire";
static const char* FileName = "data.txt";
static const char* HighScoreLabel = "hiscore:";
static const char* ColorScheme1IdxLabel = "colorschemeidx:";
static const char* ColorScheme2IdxLabel = "colorscheme2idx:";

//----------------------------------------------------------------------------
// Private function prototypes
//----------------------------------------------------------------------------

static FILE* OpenDataFile(const char* mode);
static bool IsDataModified(void);

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
      if (strcmp(buffer, HighScoreLabel) == RET_OK) {
        StoredData.m_highScore = number;
      } else if (strcmp(buffer, ColorScheme1IdxLabel) == RET_OK) {
        StoredData.m_colorScheme1 = number;
      } else if (strcmp(buffer, ColorScheme2IdxLabel) == RET_OK) {
        StoredData.m_colorScheme2 = number;
      }
    }
    ActiveData = StoredData;
  }
}

void dd_Save(void)
{
  if (IsDataModified()) {
    FILE* fp = OpenDataFile("w");
    if (fp) {
      fprintf(fp, "%s %d\n", HighScoreLabel, ActiveData.m_highScore);
      fprintf(fp, "%s %d\n", ColorScheme1IdxLabel, ActiveData.m_colorScheme1);
      fprintf(fp, "%s %d\n", ColorScheme2IdxLabel, ActiveData.m_colorScheme2);
      fclose(fp);
      StoredData = ActiveData;
    }
  }
}

int dd_GetHighScore(void)
{
  return ActiveData.m_highScore;
}

void dd_SetHighScore(int score)
{
  ActiveData.m_highScore = score;
}

int dd_GetColorSchemeIdx(void)
{
  return ActiveData.m_colorScheme1;
}

void dd_SetColorSchemeIdx(int index)
{
  ActiveData.m_colorScheme1 = index;
}

int dd_GetColorScheme2Idx(void)
{
  return ActiveData.m_colorScheme2;
}

void dd_SetColorScheme2Idx(int index)
{
  ActiveData.m_colorScheme2 = index;
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

static bool IsDataModified(void)
{
  return (ActiveData.m_highScore != StoredData.m_highScore) ||
         (ActiveData.m_colorScheme1 != StoredData.m_colorScheme1) ||
         (ActiveData.m_colorScheme2 != StoredData.m_colorScheme2);
}