#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INI_FIRST_REVISION "201306051630" //YYYYMMDDHHMM
#define INI_REVISION "201312102005" // 2013-12-10 20:05  //YYYYMMDDHHMM
#define INI_VERSION "1.00"

extern int INI_ERROR;

#define INI_ERROR_NONE           0x00 // 00
#define INI_ERROR_UNKNOWN        0x01 // 01
#define INI_ERROR_FILE           0x02 // 02
#define INI_ERROR_FILEREAD       0x03 // 03
#define INI_ERROR_FILEWRITE      0x04 // 04
#define INI_ERROR_MEMORY         0x05 // 05
#define INI_ERROR_MEMORYREAD     0x06 // 06
#define INI_ERROR_MEMORYWRITE    0x07 // 07
#define INI_ERROR_MEMORYALLOC    0x08 // 08
#define INI_ERROR_PARSE          0x09 // 09
#define INI_ERROR_PARSEINVALID   0x0A // 10

#ifndef INI_ERROR_VERBOSE
    #define INI_ERROR_VERBOSE 1
#endif

#ifndef INI_AUTOTRIM
    #define INI_AUTOTRIM 1
#endif

#ifdef INI_CASESENSITIVE
	#define INI_CASESENSITIVE 1
#else
	#define INI_CASESENSITIVE 0
#endif

#ifndef INI_MAXLINE
    #define INI_MAXLINE 200
#endif

/*

#define INI_MAX 500

struct _INI {
	char *file;
	int sections;
	int values;
	char *section[INI_MAX];
	char *value[][INI_MAX];
} INI;
*/


/*
ini_read(char *file, char* section, char* key, char* dvalue)

	returns:	3 possibilities: dvalue, value, section
	
				-dvalue (Default value), if key was not found in valid section
				-dvalue (Default value), if section was not found, (if key param is "")
				-value (not NULL or Section), if both valid
				-value (not NULL or Section), regardless of section (if section param is "")
				-Section, meaning section exist, (if key param is "")
*/

// [INI Library Functions]
	char *ini_read(char *file, char *section, char *key, char *dvalue);
	int ini_write(char *file, char *section, char *key, char *value);

// [INI Internal Functions]
	int ini_error(int error, int v);
	char *ini_fix(char *str);
	char *ini_strip(char *str);
	char *ini_section(char *line);
	char *ini_key(char *line);

// [INI Utility Functions]
	char *strlower(char *str);
	char *substr(char *str, int start, int length);
	char *strtrim_l(const char *string, int trim);


