#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIELD_SIZE 200
#define INSPECTOR_NAME_SIZE 50
#define CATEGORY_SIZE 45

#define SYMBOLIC_PERM_LENGTH 10
#define MAX_LOG_ENTRY 512

#define MAX_PATH 256

#define DIR_PERM 0750
#define FILE_PERM 0664


typedef struct {
        int id;
        char inspectorName[INSPECTOR_NAME_SIZE];
        float latitude; 
        float longitude;
        char category[CATEGORY_SIZE];
        int severity; // possible : 1, 2 or 3
        time_t timestamp;
        char descriptionText[FIELD_SIZE];
    } Report;

void handle_add (const char *district, const char *role, const char *user) {
    char path[MAX_PATH];
    sprintf(path, "%s/reports.dat", district);
    
    struct stat st;
    
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
        
            if (!hasAccess) {
                perror("nu are access!\n");
            }
        }
        
    }

    char permissionString[SYMBOLIC_PERM_LENGTH];




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
