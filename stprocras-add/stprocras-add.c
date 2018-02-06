#include "../include.h"

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	const char* const dbFilePath = DB_FILE_PATH; // FIXME Testing

	if (!fileExistsAndIsReadable(dbFilePath))
	{
		fprintf(stderr, "File does not exist or is not readable!\n");
		return HALT_FILE_UNREADABLE;
	}

	FILE* fp = fopen(dbFilePath, "a");

	if (!fpIsValid(fp))
	{
		fprintf(stderr, "File stream is invalid!");
		return HALT_FILE_FP_INVALID;
	}
	
}
