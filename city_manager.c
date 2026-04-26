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



void handle_add (const char *district, const char *role, const char *user) {
    char path[MAX_PATH];
    sprintf(path, "%s/reports.dat", district);

    struct stat st;
    Report r;

    if (mkdir(district, 0750) == -1) {
        perror("Eroare creare director!");
        exit(1);
    }

    chmod(district, 0750);

    snprintf(path, MAX_PATH, "%s/reports.dat", district);

//verificare permisiuni

    if (stat(path, &st) == 0) {
        int hasAccess = 0;

        if (strcmp(role, "manager") == 0) {
            if (st.st_mode & S_IWUSR) {
                hasAccess = 1;
            }
        }
        else {
            if (strcmp(role, "inspector") == 0) {
                if (st.st_mode & S_IWGRP) {
                    hasAccess = 1;
                }
            }
        }
 
            if (!hasAccess) {
                perror("nu are access!\n");
            }
    }

    memset(&r, 0, sizeof(Report));

    r.id = (int)time(NULL);
    r.timestamp = time(NULL);
    r.severity = 2;
    r.latitude = 44.4268;
    r.longitude = 1025;
 

    r.id = (int)time(NULL);
    r.timestamp = time(NULL);
    r.severity = 2;
    r.latitude = 44.4268;
    r.longitude = 1025;

    strncpy(r.inspectorName, user, MAX_INSPECTOR_NAME - 1);

    strncpy(r.category, "road", MAX_CATEGORY_NAME - 1);

    strncpy(r.descriptionText, "Problema detectata!\n", MAX_DESCRIPTION - 1);

    int fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0664);

    if (fd < 0) {
        perror("Eroare la deschiderea fisierului reports.dat");
        exit(3);
    }

    if (fd < 0) {
        perror("Eroare la deschiderea fisierului reports.dat");
        exit(3);
    } 


    chmod(path, 0664);

    if (write(fd, &r, sizeof(Report)) != sizeof(Report)) {
        perror("Eroare la scrierea raportului!\n");
        exit(4);
    }
    else {
        printf("SUCCES : Raportul %d a fost salvat!\n", r.id);
    }
    close(fd);

    char linkName[MAX_PATH];
    snprintf(linkName, MAX_PATH - 1, "active report %s", district);

    unlink(linkName);

    if (symlink(path, linkName) == 0) {
        printf("Symlink creat %s -> %s", linkName, path);
    }

    logAction(district, role, user, "ADD REPORT");

    // char permissionString[SYMBOLIC_PERM_LENGTH];

}

int main (int argc, char *argv[]) {
    char *role = NULL;
    char *user = NULL;
    char *command = NULL;
    char *district = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--role") == 0 && ((i + 1) < argc)) {
            role = argv[++i];
        }
        else if (strcmp(argv[i], "--user") == 0 && ((i + 1) < argc)) {
            user = argv[++i];
        }
        else if (!command) {
            command = argv[i];
            if ((i + 1) < argc) {
                district = argv[++i];
            }
        }

        if (!role || !user || !command || !district) {
            perror("Eroare! Lipsesc argumentele esentiale!\n");
            exit(1);
        }
        printf("||---------------------------CITY MANAGER----------------------------------------||\n\n");

        printf("|| User : %s ----- Role : %s ----- Command : %s ------ District : %s ||\n\n", user, role, command, district);

        if (strcmp(command, "add") == 0) {

            //add_comand => urmeaza implementare :)

        }
        

    }
    return 0;
}

