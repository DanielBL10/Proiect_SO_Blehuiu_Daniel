#ifndef CITY_MANAGER_H
#define CITY_MANAGER_H

#include <time.h>

#define MAX_PATH 256
#define MAX_INSPECTOR_NAME 100
#define MAX_CATEGORY_NAME 50
#define MAX_DESCRIPTION 200

#define CATEGORY_SIZE 45
#define FIELD_SIZE 200

#define DIR_PREM 0770//0750 modif
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

void initDistrictConfig(const char *district);

void handleAdd (const char *district, const char *role, const char *user);
void handleRemoveReport(const char *district, int targetID, const char *role, const char *user);

void logOperation(const char *district, const char *role, const char *user, const char *action);
 



#endif
