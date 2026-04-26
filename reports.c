#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include "city_manager.h"

#define DIR_PERM 0770
#define FILE_PERM 0664


void logOperation(const char *district, const char *role, const char *user, const char *action) {
    char logPath[MAX_PATH];
    
    snprintf(logPath, MAX_PATH, "%s/logged_district", district);

     
    int fd = open(logPath, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) {
        perror("Eroare la deschiderea jurnalului de operatiuni");
        return;
    }

    time_t acum = time(NULL);
    char *timp_str = ctime(&acum);
    
    if (timp_str[strlen(timp_str) - 1] == '\n') {
        timp_str[strlen(timp_str) - 1] = '\0';
    }

    char buffer[512];
    int len = snprintf(buffer, sizeof(buffer), "[%s] Role: %s | User: %s | Action: %s\n", 
        timp_str, role, user, action);

    if (write(fd, buffer, len) != len) {
        perror("Eroare la scrierea in jurnal");
    }
    
    close(fd);
}

void initDistrictConfig(const char *district) {
    char cfgPath[MAX_PATH];
    snprintf(cfgPath, MAX_PATH, "%s/district.cfg", district);

    if (access(cfgPath, F_OK) == -1) {
        int fd = open(cfgPath, O_WRONLY | O_CREAT, FILE_PERM);
        if (fd >= 0) {
            const char *defaultCfg = "severity_threshold=2\n";
            write(fd, defaultCfg, strlen(defaultCfg));
            close(fd);
        } else {
            perror("Eroare la crearea fisierului de configurare district.cfg");
        }
    }
}

void handleAdd(const char *district, const char *role, const char *user) {
    char path[MAX_PATH];
    struct stat st;
    Report r;

     
    if (mkdir(district, DIR_PERM) == -1 && errno != EEXIST) {
        perror("Eroare la creearea directorului!");
        return;
    }

 
    initDistrictConfig(district);
    
    char linkName[MAX_PATH];
    snprintf(linkName, MAX_PATH, "active_reports-%s", district);
    snprintf(path, MAX_PATH, "%s/reports.dat", district);
    
    unlink(linkName);  
    symlink(path, linkName);  

   
    if (stat(district, &st) == 0) {
        int hasAccess = 0;

        if (strcmp(role, "manager") == 0 && (st.st_mode & S_IWUSR)) {
            hasAccess = 1;
        } else if (strcmp(role, "inspector") == 0 && (st.st_mode & S_IWGRP)) {
            hasAccess = 1;
        }

        if (!hasAccess) {
            fprintf(stderr, "Acces respins! Rolul %s nu are drepturi in %s!\n", role, district);
            return; 
        }

      
    int fd = open(path, O_WRONLY | O_CREAT | O_APPEND, FILE_PERM);
    if (fd >= 0) {
        memset(&r, 0, sizeof(Report));    
        r.id = (int)time(NULL);
        r.timestamp = time(NULL); 
        strncpy(r.inspectorName, user, MAX_INSPECTOR_NAME - 1);          
        printf("\n=== ADAUGARE RAPORT NOU IN '%s' ===\n", district);
            
        printf("Latitudine : ");
    
        if (scanf("%f", &r.latitude) != 1) {
            r.latitude = 0.0;
        }

        printf("Longitudine ");

        if (scanf("%f", &r.longitude) != 1) {
            r.longitude = 0.0;
        }

        int c;
        
        while ((c = getchar()) != '\n' && c != EOF);  

        printf("Categoria (road/lighting/flooding): ");
    
        if (fgets(r.category, CATEGORY_SIZE, stdin)) {
            r.category[strcspn(r.category, "\n")] = 0;  
        }

        printf("Severitate (1=minor, 2=moderate, 3=critical): ");
    
        if (scanf("%d", &r.severity) != 1) {
            r.severity = 1; 
        }
            
        while ((c = getchar()) != '\n' && c != EOF);  

        printf("Scurta descriere: ");
            
        if (fgets(r.descriptionText, FIELD_SIZE, stdin)) {
            r.descriptionText[strcspn(r.descriptionText, "\n")] = 0;
        }
             
        write(fd, &r, sizeof(Report));
        close(fd);
        printf("\nOperatiune realizata cu succes! Raportul adaugat in %s cu ID-ul %d\n", district, r.id);
            
        logOperation(district, role, user, "ADD_REPORT");
        } else {
            perror("Eroare la deschiderea fisierului reports.dat");
        }
    } else {
        perror("Eroare la citirea permisiunilor directorului");
    }
}

void handleRemoveReport(const char *district, int targetID, const char *role, const char *user) {
    
    if (strcmp(role, "manager") != 0) {
        fprintf(stderr, "Eroare: Acces respins! Doar managerii pot sterge rapoarte.\n");
        return;
    }

    char path[MAX_PATH];
    snprintf(path, MAX_PATH, "%s/reports.dat", district);

    int fd = open(path, O_RDWR);
    if (fd < 0) {
        perror("Eroare la deschiderea fisierului sau districtul nu exista");
        return;
    }

    struct stat st;
    if (fstat(fd, &st) == 0 && !(st.st_mode & S_IWUSR)) {
        fprintf(stderr, "Eroare: Permisiuni insuficiente pe fisierul reports.dat!\n");
        close(fd);
        return;
    }

    Report r;
    int found = 0;

    while (read(fd, &r, sizeof(Report)) == sizeof(Report)) {
        if (r.id == targetID) {
            found = 1;

            off_t overwrite_pos = lseek(fd, -sizeof(Report), SEEK_CUR); 
            off_t read_pos = overwrite_pos + sizeof(Report);         

             
            while (lseek(fd, read_pos, SEEK_SET) != -1 && read(fd, &r, sizeof(Report)) == sizeof(Report)) {
                read_pos += sizeof(Report);          
                
                lseek(fd, overwrite_pos, SEEK_SET);     
                write(fd, &r, sizeof(Report));          
                overwrite_pos += sizeof(Report);       
            }

          
            if (ftruncate(fd, overwrite_pos) == -1) {
                perror("Eroare critica la ftruncate");
            } else {
                printf("Raportul cu ID %d a fost eliminat cu succes.\n", targetID);
                logOperation(district, role, user, "REMOVE_REPORT");
            }
            break;  
        }
    }

    if (!found) {
        fprintf(stderr, "Raportul cu ID-ul %d nu a fost gasit in %s!\n", targetID, district);
    }

    close(fd);
}
