#define TITLE_SIZE 128 

#define DB_FILE_NAME "db.csv"
#define FALLBACK_FILE_PATH "/opt/st-procras"

#define EPOCH_YEAR 1970

#define CONTINUE_AFTER_DATE_ERROR true // FIXME Testing until flag can be parsed

// Dummy returns so that GCC does not complain & to keep track of these tests
#define TEST_DUMMY_TRUE 1
#define TEST_DUMMY_FALSE 0

#define JANUARY_NUM 1
#define MARCH_NUM 3
#define MAY_NUM 5
#define JULY_NUM 7
#define AUGUST_NUM 8
#define OCTOBER_NUM 10
#define DECEMBER_NUM 12

// Error codes
#define HALT_FILE_UNREADABLE -1
#define HALT_FILE_FP_INVALID -2
#define HALT_INVALID_DATE -3
#define HALT_MALLOC_HOME_PATH -4
#define HALT_MALLOC_DB_FILE_PATH -5
#define HALT_MALLOC_LINE_BUF -7
#define HALT_MALLOC_PARSED_DLINE_STRUCT -8
