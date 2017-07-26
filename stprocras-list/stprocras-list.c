#define BOOL int
#define STD_BUF_SIZE 4096
#define TITLE_SIZE 128 
#define TRUE 1
#define FALSE 0

#define BASE_FORMAT_TO_MODIFY "%%4i%%2i%%2iT%%2i%%2i%%2iZ;%%%i[^\t\n]"
#define TESTFILE_PATH "tests/test.pcr"

#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

BOOL fileExistsAndIsReadable(char* filePathToCheck);
BOOL fpIsValid(FILE* fp);

struct Deadline 
{
	int yr;
	int mon;
	int day;

	int hr;
	int min;
	int sec;

	char title[TITLE_SIZE];
};

/* TODO Add int argc, char* argv[] when implementing inputs */
int main(void)
{
	char* csvFilePath = TESTFILE_PATH; /* FIXME Testing */
	if (!fileExistsAndIsReadable(csvFilePath))
	{
		fprintf(stderr, "File does not exist or is not readable!\n");
		return -1;
	}
	
	FILE* fp = fopen(csvFilePath, "r");
	
	if (!fpIsValid(fp))
	{
		fprintf(stderr, "File stream is not valid!\n");
		return -2;
	}

	size_t bufferSize = sizeof(char) * STD_BUF_SIZE;
	char* buffer = malloc(bufferSize);

	/* Check how much space is needed for format fed to scanf */
	size_t const formatLength = (size_t) snprintf(NULL, 0,
				BASE_FORMAT_TO_MODIFY, TITLE_SIZE);

	/* Line format: ISO 8601 + title string */	
	char lineFormat[formatLength + 1];

	/* Craft the format that will be used in scanf.
	 * Will keep the title length correct even if the
	 * constant is changed */
	sprintf(lineFormat, BASE_FORMAT_TO_MODIFY, TITLE_SIZE);

	struct Deadline* cur_p = malloc(sizeof(struct Deadline));

	while(getline(&buffer, &bufferSize, fp) != -1) 
	{
		sscanf(buffer, lineFormat,
				&(cur_p->yr), &(cur_p->mon), &(cur_p->day),
				&(cur_p->hr), &(cur_p->min), &(cur_p->sec),
				cur_p->title);

		/* TODO */
	}
	
	free(cur_p);
	free(buffer);
	fclose(fp);

	return 0;
}

BOOL fileExistsAndIsReadable(char* filePathToCheck)
{
	return (access(filePathToCheck, R_OK) == 0) ? TRUE : FALSE;
}

BOOL fpIsValid(FILE* fp)
{
	return (fp != NULL) ? TRUE : FALSE;
}
