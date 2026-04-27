#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include "city_manager.h"

#define DIR_PERM 0750
#define FILE_PERM 0664
#define CFG_PERM 0640
#define LOG_PERM 0644



int checkAccess(const char *path, const char *role, char access_type) {
    struct stat st;
    if (stat(path, &st) == -1) return 0;  

    if (strcmp(role, "manager") == 0) {
       
        if (access_type == 'r') return (st.st_mode & S_IRUSR);
        if (access_type == 'w') return (st.st_mode & S_IWUSR);
    } 
    else if (strcmp(role, "inspector") == 0) {
        
        if (access_type == 'r') return (st.st_mode & S_IRGRP);
        if (access_type == 'w') return (st.st_mode & S_IWGRP);
    }
    
    return 0;  
}

void logOperation(const char *district, const char *role, const char *user, const char *action) {
    char logPath[MAX_PATH];
    snprintf(logPath, MAX_PATH, "%s/logged_district", district);

    struct stat st;
    
    if (stat(logPath, &st) == 0) {
        if (!checkAccess(logPath, role, 'w')) {
            fprintf(stderr, "EROARE SECURITATE: Rolul '%s' nu are permisiune de scriere in jurnal!\n", role);
            return;  
        }
    }
 
    int fd = open(logPath, O_WRONLY | O_CREAT | O_APPEND, LOG_PERM);
    if (fd < 0) {
        perror("Eroare la deschiderea jurnalului");
        return;
    }

    time_t acum = time(NULL);
    char *timp_str = ctime(&acum);
    if (timp_str && timp_str[strlen(timp_str) - 1] == '\n') {
        timp_str[strlen(timp_str) - 1] = '\0';
    }

    char buffer[512];
    int len = snprintf(buffer, sizeof(buffer), "[%s] Role: %s | User: %s | Action: %s\n", 
       timp_str ? timp_str : "N/A", role, user, action);

    if (write(fd, buffer, len) != len) {
        perror("Eroare la scrierea in jurnal");
    }

    fchmod(fd, LOG_PERM); 
    close(fd);
}

void initDistrictConfig(const char *district) {
    char cfgPath[MAX_PATH];
    snprintf(cfgPath, MAX_PATH, "%s/district.cfg", district);

    if (access(cfgPath, F_OK) == -1) {

        int fd = open(cfgPath, O_WRONLY | O_CREAT, CFG_PERM);
   
        if (fd >= 0) {
            const char *defaultCfg = "severity_threshold=2\n";
            write(fd, defaultCfg, strlen(defaultCfg));
            fchmod(fd, CFG_PERM);
            close(fd);
        } else {
            perror("Eroare la crearea fisierului de configurare district.cfg");
        }
    }
}

 

void handleAdd(const char *district, const char *role, const char *user) {
    char path[MAX_PATH];
    char linkName[MAX_PATH];
    Report r;
    struct stat d_stat;

    if (stat(district, &d_stat) == -1) {
        if (strcmp(role, "manager") != 0) {
            fprintf(stderr, "Acces Refuzat: Doar rolul 'manager' poate crea un district nou!\n");
            return;
        }
        if (mkdir(district, DIR_PERM) == -1 && errno != EEXIST) {
            perror("Eroare la creearea directorului!");
            return;
        }
        chmod(district, DIR_PERM);  
        initDistrictConfig(district);
    }

    snprintf(path, MAX_PATH, "%s/reports.dat", district);
    snprintf(linkName, MAX_PATH, "active_reports-%s", district);

    struct stat rep_stat;
    if (stat(path, &rep_stat) == 0) {
        if (!checkAccess(path, role, 'w')) {
            fprintf(stderr, "Acces Refuzat: Rolul '%s' nu are permisiune sa adauge rapoarte in %s!\n", role, path);
            return;
        }
    } else {
        if (!checkAccess(district, role, 'w')) {
            fprintf(stderr, "Acces Refuzat: Rolul '%s' nu poate crea baza de date in %s!\n", role, district);
            return;
        }
    }
     
    unlink(linkName);  
    symlink(path, linkName);  

    
    int fd = open(path, O_WRONLY | O_CREAT | O_APPEND, FILE_PERM);
    if (fd >= 0) {
        
        memset(&r, 0, sizeof(Report));    
        r.id = (int)time(NULL);
        r.timestamp = time(NULL); 
        strncpy(r.inspectorName, user, MAX_INSPECTOR_NAME - 1); 
        r.inspectorName[MAX_INSPECTOR_NAME - 1] = '\0';
        printf("\n=== ADAUGARE RAPORT NOU IN '%s' ===\n", district);
        
        
        printf("Latitudine: ");
        if (scanf("%f", &r.latitude) != 1) r.latitude = 0.0;

        printf("Longitudine: ");
        if (scanf("%f", &r.longitude) != 1) r.longitude = 0.0;

        int c;
        while ((c = getchar()) != '\n' && c != EOF);  

        printf("Categoria (road / lighting / flooding): ");
        if (fgets(r.category, CATEGORY_SIZE, stdin)) {
            r.category[strcspn(r.category, "\n")] = 0;  
        }

        printf("Severitate (1 = minor, 2 = moderate, 3 = critical): ");
        if (scanf("%d", &r.severity) != 1) r.severity = 1; 
            
        while ((c = getchar()) != '\n' && c != EOF);  

        printf("Scurta descriere: ");
        if (fgets(r.descriptionText, FIELD_SIZE, stdin)) {
            r.descriptionText[strcspn(r.descriptionText, "\n")] = 0;
        }
    
        write(fd, &r, sizeof(Report));
        close(fd);
     
        chmod(path, FILE_PERM);  

        printf("\nOperatiune realizata cu succes! ID: %d\n", r.id);
            
        logOperation(district, role, user, "ADD_REPORT");

    } else {
        perror("Eroare la deschiderea fisierului reports.dat");
    }
} 



void handleRemoveReport(const char *district, int targetID, const char *role, const char *user) {
    char path[MAX_PATH];
    snprintf(path, MAX_PATH, "%s/reports.dat", district);

    if (!checkAccess(path, role, 'w')) {
        fprintf(stderr, "Eroare! Acces respins la stergere pentru rolul %s!\n", role);
        return;
    }

    int fd = open(path, O_RDWR);
    if (fd < 0) {
        perror("Eroare la deschiderea bazei de date");
        return;
    }

    Report r;
    int found = 0;
    off_t write_pos = 0;

    while (read(fd, &r, sizeof(Report)) == sizeof(Report)) {
        if (r.id == targetID) {
            found = 1;
            write_pos = lseek(fd, 0, SEEK_CUR) - sizeof(Report);
            break;
        }
    }    

    if (found) {
        off_t read_pos = write_pos + sizeof(Report); 
        
        while (lseek(fd, read_pos, SEEK_SET) != -1 && read(fd, &r, sizeof(Report)) == sizeof(Report)) {          
            lseek(fd, write_pos, SEEK_SET);     
            if (write(fd, &r, sizeof(Report)) != sizeof (Report)) {
                perror ("Eroare critica la scriere in timpul shiftarii!");
                close(fd);
                return;
            }  

            write_pos += sizeof(Report);
            read_pos += sizeof(Report);       
        }

        if (ftruncate(fd, write_pos) == -1) {
            perror("Eroare critica la ftruncate");
        } else {
            printf("Raportul cu ID %d a fost eliminat cu succes.\n", targetID);
            logOperation(district, role, user, "REMOVE_REPORT");
        }

        fchmod(fd, FILE_PERM); 
    } else {
        fprintf(stderr, "Raportul cu ID %d nu a fost gasit in %s!\n", targetID, district);
    }

    close(fd);
}

void getPermissionsSymbolic(mode_t mode, char *str) {
    str[0] = (mode & S_IRUSR) ? 'r' : '-';
    str[1] = (mode & S_IWUSR) ? 'w' : '-';
    str[2] = (mode & S_IXUSR) ? 'x' : '-';
    str[3] = (mode & S_IRGRP) ? 'r' : '-';
    str[4] = (mode & S_IWGRP) ? 'w' : '-';
    str[5] = (mode & S_IXGRP) ? 'x' : '-';
    str[6] = (mode & S_IROTH) ? 'r' : '-';
    str[7] = (mode & S_IWOTH) ? 'w' : '-';
    str[8] = (mode & S_IXOTH) ? 'x' : '-';
    str[9] = '\0';
}

void handleList(const char *district) {
    char path[MAX_PATH];
    snprintf(path, MAX_PATH, "%s/reports.dat", district);

    struct stat st;

    if (stat(path, &st) == -1) {
        perror("Eroare stat (reports.dat)");
        return;
    }

    char perm[10];
    getPermissionsSymbolic(st.st_mode, perm);

    char timeBuf[32];
    strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", localtime(&st.st_mtime));

    printf("\n=== LISTARE DISTRICT: %s ===\n", district);
    printf("Permisiuni: %s | Marime: %ld bytes\n", perm, st.st_size);
    printf("Ultima modificare: %s\n", timeBuf);

    if (!(st.st_mode & S_IRUSR) && !(st.st_mode & S_IRGRP)) {
        fprintf(stderr, "Acces Refuzat: Permisiunile fisierului nu permit citirea!\n");
        return;
    }

    int fd = open(path, O_RDONLY);

    if (fd < 0) {
        perror("Eroare open");
        return;
    }

    Report r;
    int count = 0;

    while (read(fd, &r, sizeof(Report)) == sizeof(Report)) {
        count++;
        r.inspectorName[strcspn(r.inspectorName, "\n")] = 0;

        printf("[%d] Inspector: %s | Categorie: %s | Severitate: %d\n", r.id, r.inspectorName, r.category, r.severity);
        printf("Descriere: %s\n", r.descriptionText);
    }

    if (count == 0) printf("Districtul nu are rapoarte salvate.\n");
    else printf("Total rapoarte: %d\n", count);
    
    close(fd);
}