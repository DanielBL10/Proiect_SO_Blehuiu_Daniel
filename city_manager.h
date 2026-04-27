#ifndef CITY_MANAGER_H
#define CITY_MANAGER_H

#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_PATH 256
#define MAX_INSPECTOR_NAME 100
#define MAX_CATEGORY_NAME 50
#define MAX_DESCRIPTION 200

#define CATEGORY_SIZE 45
#define FIELD_SIZE 200

#define DIR_PERM 0750    // rwxr-x---
#define FILE_PERM 0640   // rw-rw-r--
#define CFG_PERM 0640    // rw-r-----
#define LOG_PERM 0640    // rw-r--r--

 

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
 
int checkAccess(const char *path, const char *role, char access_type);

void getPermissionsSymbolic(mode_t mode, char *str);

void handleList(const char *district);

void view(const char *district, int targetID, const char *role);

void update_threshold(const char *district, const char *role, int newValue);

int parse_condition(const char *input, char *field, char *op, char *value);

int match_condition(Report *r, const char *field, const char *op, const char *value);

void handleFilter(const char *district, int conditionCount, char **conditions);


#endif
