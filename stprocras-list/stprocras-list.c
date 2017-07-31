#define STD_BUF_SIZE 4096
#define TITLE_SIZE 128 

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
	int yr;
	int mon;
	int day;

	int hr;
	int min;
	int sec;

	char title[TITLE_SIZE];
};

struct DeadlineInRelTime
{
	int days;
	char title[TITLE_SIZE];
};

bool dateIsValid(struct ParsedDeadline* prsd_p);
bool fileExistsAndIsReadable(char* filePathToCheck);
bool fpIsValid(FILE* fp);
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

	struct ParsedDeadline* prsd_p = malloc(sizeof(struct ParsedDeadline));

	for (int lineCount = 1;
			getline(&buffer, &bufferSize, fp) != -1; ++lineCount) 
	{
		zeroParsedDeadlineTitle(prsd_p);
		sscanf(buffer, lineFormat,
				&(prsd_p->yr), &(prsd_p->mon), &(prsd_p->day),
				&(prsd_p->hr), &(prsd_p->min), &(prsd_p->sec),
				prsd_p->title);

		if (!dateIsValid(prsd_p))
		{
			fprintf(stderr, "Invalid date in line %i\n", lineCount);

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

bool dateIsValid(struct ParsedDeadline* prsd_p)
{
	// TODO
	
	return TEST_DUMMY_TRUE; // FIXME Testing
}

bool fileExistsAndIsReadable(char* filePathToCheck)
{
	return (access(filePathToCheck, R_OK) == 0) ? true : false;
}

bool fpIsValid(FILE* fp)
{
	return (fp != NULL) ? true : false;
}

void printRelativeDeadline(struct DeadlineInRelTime rel) 
{
	// TODO
	
	return;
}

void zeroParsedDeadlineTitle(struct ParsedDeadline* prsd_p)
{
	
	for (int titleLength = strlen(prsd_p->title), i = 0;
			i < titleLength; i++)
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
