#define STD_BUF_SIZE 4096
#define TITLE_SIZE 128 

#define EPOCH_YEAR 1970

#define BASE_FORMAT_TO_MODIFY "%%4i%%2i%%2iT%%2i%%2i%%2iZ;%%%i[^\t\n]"
#define TESTFILE_PATH "tests/test.pcr"

#define CONTINUE_AFTER_DATE_ERROR true // FIXME Testing until flag can be parsed

// Dummy returns so that GCC does not complain & to keep track of these tests
#define TEST_DUMMY_TRUE 1
#define TEST_DUMMY_FALSE 0

// Error codes
#define HALT_FILE_UNREADABLE -1
#define HALT_FILE_FP_INVALID -2
#define HALT_INVALID_DATE -3

#include <argp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
bool fileExistsAndIsReadable(char* filePathToCheck);
bool fpIsValid(FILE* fp);
bool monthHasThirtyOneDays(unsigned int mon);
bool monthIsFebruary(unsigned int mon);
bool timeIsValid(unsigned int hr, unsigned int min, unsigned int sec);
bool yearIsLeap(unsigned int yr);
void printRelativeDeadline(struct DeadlineInRelTime rel);
void zeroParsedDeadlineTitle(struct ParsedDeadline* prsd_p);
struct DeadlineInRelTime generateRelDeadline(struct ParsedDeadline* prsd_p);

// TODO Add int argc, char* argv[] when implementing inputs
int main(void)
{
	char* csvFilePath = TESTFILE_PATH; // FIXME Testing
	if (!fileExistsAndIsReadable(csvFilePath))
	{
		fprintf(stderr, "File does not exist or is not readable!\n");
		return HALT_FILE_UNREADABLE;
	}
	
	FILE* fp = fopen(csvFilePath, "r");
	
	if (!fpIsValid(fp))
	{
		fprintf(stderr, "File stream is not valid!\n");
		return HALT_FILE_FP_INVALID;
	}

	size_t bufferSize = sizeof(char) * STD_BUF_SIZE;
	char* buffer = malloc(bufferSize);

	// Check how much space is needed for format fed to scanf
	size_t const formatLength = (size_t) snprintf(NULL, 0,
				BASE_FORMAT_TO_MODIFY, TITLE_SIZE);

	// Line format: ISO 8601 + title string	
	char lineFormat[formatLength + 1];

	/* Craft the format that will be used in scanf.
	 * Will keep the title length correct even if the format
	 * layout is changed */
	sprintf(lineFormat, BASE_FORMAT_TO_MODIFY, TITLE_SIZE);

	struct ParsedDeadline* prsd_p = malloc(sizeof(struct ParsedDeadline) + 1);

	for (int lineCount = 1;
			getline(&buffer, &bufferSize, fp) != -1; ++lineCount) 
	{
		zeroParsedDeadlineTitle(prsd_p);
		sscanf(buffer, lineFormat,
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
				return HALT_INVALID_DATE;
			}
		}

		// TODO
	}
	
	free(prsd_p);
	free(buffer);
	fclose(fp);

	return 0;
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

bool fileExistsAndIsReadable(char* filePathToCheck)
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
	// That leaves Jan, Mar, May, Jul, Aug, Oct, Dec as true
	
	switch(mon)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
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

void printRelativeDeadline(struct DeadlineInRelTime rel) 
{
	// TODO
	
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

struct DeadlineInRelTime generateRelDeadline(struct ParsedDeadline* prsd_p)
{
	struct DeadlineInRelTime rel = {0, ""}; // FIXME Testing

	// TODO
	
	return rel;
}
