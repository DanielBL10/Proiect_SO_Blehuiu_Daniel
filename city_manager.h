#ifndef CITY_MANAGER_H
#define CITY_MANAGER_H

#include <time.h>

#define MAX_PATH 256
#define MAX_INSPECTOR_NAME 100
#define MAX_CATEGORY_NAME 50
#define MAX_DESCRIPTION 200

#define DIR_PREM 0750
#define BIN_PERM 0664
#define CFG_PERM 0640
#define LOG_PERM 0644

typedef struct Report {
    int id;
    char inspectorName[MAX_INSPECTOR_NAME];
    float latitude;
    float longitude;
    char category[MAX_CATEGORY_NAME];
    int severity;
    time_t timestamp;
    char descriptionText[MAX_DESCRIPTION];
} Report;



#endif
