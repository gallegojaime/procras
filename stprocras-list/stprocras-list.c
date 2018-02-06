#include "../include.h"

#include <argp.h>
#include <dirent.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

struct ParsedDeadline 
{
	unsigned int yr;
	unsigned int mon;
	unsigned int day;

	unsigned int hr;
	unsigned int min;
	unsigned int sec;

	char title[TITLE_SIZE];
};

struct DeadlineInRelTime
{
	int days;
	char title[TITLE_SIZE];
};

bool dateIsValid(unsigned int yr, unsigned int mon, unsigned int day);
bool datetimeIsValid(struct ParsedDeadline* prsd_p);
bool directoryExists(char** dirPath_p);
bool fileExistsAndIsReadable(const char* const filePathToCheck);
bool fpIsValid(FILE* fp);
bool monthHasThirtyOneDays(unsigned int mon);
bool monthIsFebruary(unsigned int mon);
bool timeIsValid(unsigned int hr, unsigned int min, unsigned int sec);
bool yearIsLeap(unsigned int yr);
void printRelDeadline(struct DeadlineInRelTime rel);
void zeroParsedDeadlineTitle(struct ParsedDeadline* prsd_p);
struct tm generateAndPopulateTm(struct ParsedDeadline* prsd_p);
struct DeadlineInRelTime generateRelDeadline(struct tm* TmDline_p, char* title);

// TODO Add int argc, char* argv[] when implementing inputs
int main(void)
{
	// Return value for final return after cleanup 
	int retVal = 0;

	char** pathDbDir_p;
	char* homePath = malloc(PATH_MAX);
	if (!*homePath)
	{
		fprintf(stderr, "Home path could not be malloc'd!");
		retVal = HALT_MALLOC_HOME_PATH;
		goto cleanup;
	}

	const char* const homeVar = getenv("HOME");

	if (homeVar == NULL)
	{
		pathDbDir_p = &FALLBACK_FILE_PATH;
	}

	else
	{
		pathDbDir_p = &homePath;
	}
	
	if (!directoryExists(pathDbDir_p))
	{
		mkdir(*pathDbDir_p, 0700);
	}

	char* dbFilePath = malloc(PATH_MAX);
	if (!*dbFilePath)
	{
		fprintf(stderr, "Database file path could not be malloc'd!");
		retVal = HALT_MALLOC_DB_FILE_PATH;
		goto cleanup;
	}

	dbFilePath = snprintf(dbFilePath, sizeof(dbFilePath), "%s/%s",
			*pathDbDir_p, DB_FILE_NAME);

	if (!fileExistsAndIsReadable(dbFilePath))
	{
		// Just for purposes of creating file
		FILE* fp = fopen(dbFilePath, "a");
		fclose(fp);
	}
	
	FILE* fp = fopen(dbFilePath, "r");
	if (!fpIsValid(fp))
	{
		fprintf(stderr, "File stream is not valid!\n");
		retVal = HALT_FILE_FP_INVALID;
		goto cleanup;
	}

	size_t lineBufferSize = _POSIX2_LINE_MAX; 

	char* lineBuffer = malloc(lineBufferSize);
	if (!*lineBuffer)
	{
		fprintf(stderr, "Line buffer could not be malloc'd!");
		retVal = HALT_MALLOC_LINE_BUF;
		goto cleanup;
	}

	const char* const baseFormatToModify = "%%4d%%2d%%2dT%%2d%%2d%%2dZ;%%%i[^\t\n]";

	// Check how much space is needed for format fed to scanf
	size_t const formatLength = (size_t) snprintf(NULL, 0,
				baseFormatToModify, TITLE_SIZE);

	// Line format: date + ; + title string	
	char lineFormat[formatLength + 1];

	/* Craft the format that will be used in scanf.
	 * Will keep the title length correct even if the format
	 * layout is changed */
	sprintf(lineFormat, baseFormatToModify, TITLE_SIZE);

	struct ParsedDeadline* prsd_p = malloc(sizeof(struct ParsedDeadline) + 1);
	if (!*prsd_p)
	{
		fprintf(stderr, "Parsed deadline struct could not be malloc'd!");
		retVal = HALT_MALLOC_PARSED_DLINE_STRUCT;
		goto cleanup;
	}

	for (int lineCount = 1;
			getline(&lineBuffer, &lineBufferSize, fp) != -1; ++lineCount) 
	{
		zeroParsedDeadlineTitle(prsd_p);
		sscanf(lineBuffer, lineFormat,
				&(prsd_p->yr), &(prsd_p->mon), &(prsd_p->day),
				&(prsd_p->hr), &(prsd_p->min), &(prsd_p->sec),
				prsd_p->title);

		if (!datetimeIsValid(prsd_p))
		{
			fprintf(stderr, "Invalid datetime in line %i\n", lineCount);

			if (CONTINUE_AFTER_DATE_ERROR)
			{
				continue;
			}

			else
			{
				retVal = HALT_INVALID_DATE;
				goto cleanup;
			}
		}

		struct tm Tm = generateAndPopulateTm(prsd_p);

		struct DeadlineInRelTime rel;

		rel = generateRelDeadline(&Tm, prsd_p->title);

		printRelDeadline(rel);

		// TODO
	}

cleanup:
	if (*homePath) free(homePath);
	if (*dbFilePath) free(dbFilePath);
	if (*prsd_p) free(prsd_p);
	if (*lineBuffer) free(lineBuffer);
	if (*fp) fclose(fp);

	return retVal;
}

bool datetimeIsValid(struct ParsedDeadline* prsd_p)
{
	return (dateIsValid(prsd_p->yr, prsd_p->mon, prsd_p->day) &&
			timeIsValid(prsd_p->hr, prsd_p->min, prsd_p->sec));
}

bool dateIsValid(unsigned int yr, unsigned int mon, unsigned int day)
{
	// Year bounded by epoch
	if (yr < EPOCH_YEAR)
	{
		return false;
	}

	else if (mon < 1 || mon > 12)
	{
		return false;
	}

	else if (monthIsFebruary(mon))
	{
		if (yearIsLeap(yr) && day <= 29)
		{
			return true;
		}

		else if (day <= 28)
		{
			return true;
		}
	}

	else if (day >= 1 && day <= 30)
	{
		return true;
	}

	else if (day >= 1 && monthHasThirtyOneDays(mon) && day <= 31)
	{
		return true;
	}
	
	return false;
}

bool directoryExists(char** dirPath_p)
{
	DIR* dir = opendir(*dirPath_p);
	
	if (dir)
	{
		closedir(dir);
		return true;
	}

	else
	{
		return false;
	}
}

bool fileExistsAndIsReadable(const char* const filePathToCheck)
{
	return (access(filePathToCheck, R_OK) == 0);
}

bool fpIsValid(FILE* fp)
{
	return (fp != NULL);
}

bool monthHasThirtyOneDays(unsigned int mon)
{
	// "Thirty days hath September / April, June and November ..."	
	// February should also return false
	// That leaves Jan, Mar, May, Jul, Aug, Oct, Dec as returning true
	
	switch(mon)
	{
		case JANUARY_NUM:
		case MARCH_NUM:
		case MAY_NUM:
		case JULY_NUM:
		case AUGUST_NUM:
		case OCTOBER_NUM:
		case DECEMBER_NUM:
			return true;
		default:
			return false;
	}
}

bool monthIsFebruary(unsigned int mon)
{
	return (mon == 2);
}

bool timeIsValid(unsigned int hr, unsigned int min, unsigned int sec)
{
	return (hr <= 23 && min <= 59 && sec <= 59);
}

bool yearIsLeap(unsigned int yr)
{
	if (yr % 4 == 0)
	{
		if (!(yr % 400 == 0) && (yr % 100 == 0))
		{
			return false;
		}

		return true;
	}

	return false;
}

void printRelDeadline(struct DeadlineInRelTime rel) 
{
	printf("%i\t%s\n", rel.days, rel.title);
	
	return;
}

void zeroParsedDeadlineTitle(struct ParsedDeadline* prsd_p)
{
	for (int i = 0;	i < TITLE_SIZE + 1; i++)
	{
		prsd_p->title[i] = '\0';
	}

	return;
}

struct tm generateAndPopulateTm(struct ParsedDeadline* prsd_p)
{
	struct tm Tm;

	Tm.tm_sec  = prsd_p->sec;
	Tm.tm_min  = prsd_p->min;
	Tm.tm_hour = prsd_p->hr;
	Tm.tm_mday = prsd_p->day;
	Tm.tm_mon  = (prsd_p->mon) - 1; // tm struct mon goes 0 to 11
	Tm.tm_year = (prsd_p->yr) - 1900; // tm struct year starts at 1900

	Tm.tm_isdst = -1; // Negative value forces OS to look if DST applies 

	return Tm;
}

struct DeadlineInRelTime generateRelDeadline(struct tm* TmDline_p, char* title)
{
	struct DeadlineInRelTime rel = {0, ""}; // FIXME Testing

	struct tm TmNow = ctime(time(NULL));

	time_t diffSeconds = timeDeadline - timeNow;
	time_t diffDays = diffSeconds / 86400; // Intentionally truncated

	rel.days = diffDays;
	strncpy(rel.title, title, TITLE_SIZE);
	return rel;
}
