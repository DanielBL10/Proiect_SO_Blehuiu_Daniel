//definire structura
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define FIELD_SIZE 200
#define INSPECTOR_NAME_SIZE 200
#define CATEGORY_SIZE 45

#define SYMBOLIC_PERM_LENGTH 10
#define MAX_LOG_ENTRY 512

#define MAX_PATH 256

#define MAX_DESCRIPTION 150

#define MAX_INSPECTOR_NAME 150

#define MAX_CATEGORY_NAME 40

#define DIR_PERM 0750
#define FILE_PERM 0664

typedef struct Report {
        int id;
        char inspectorName[INSPECTOR_NAME_SIZE];
        float latitude;
        float longitude;
        char category[CATEGORY_SIZE];
        int severity; // possible : 1, 2 or 3
        time_t timestamp;
        char descriptionText[FIELD_SIZE];
    } Report;


