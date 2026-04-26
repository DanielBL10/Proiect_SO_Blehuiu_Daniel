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

void logAction (const char *district, const char *role, const char *user, const char *action) {
    char path[MAX_PATH];
    snprintf(path, MAX_PATH, "%s/logged_district", district);

    int fd = open(path, O_WRONLY | O_CREAT |O_APPEND, 0644);

    if (fd < 0) {
        return;
    }

    char entry[MAX_LOG_ENTRY];
    time_t now = time(NULL);

    int len = snprintf(entry, MAX_LOG_ENTRY, "[%ld] %s: %s %s\n", now, role, user, action);

    write(fd, entry, len);
    close(fd);
}

