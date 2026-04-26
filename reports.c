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

void handleAdd (const char *district, const char *role, const char *user) {
    char path[MAX_PATH];
    struct stat st;
    Report r;

    if (mkdir (district, DIR_PERM) == -1 && errno != EEXIST ) {
        //perror("Eroare la creearea directorului!");
        return ;
    }

    snprintf(path, MAX_PATH, "%s/reports.dat", district);

    if (stat (district, &st) == 0) {
        int hasAccess = 0;

    if (strcmp (role, "manager") == 0 && (st.st_mode & S_IWUSR)) {
            hasAccess = 1;
        }
    else if (strcmp (role, "inspector") == 0 && (st.st_mode & S_IWGRP)) {
        hasAccess = 1;
    }

    if (!hasAccess) {
        fprintf (stderr, "Acces respins! Rolul %s nu are drepturi in %s!\n", role, district);
        return;
    }

        int fd = open (path, O_WRONLY | O_CREAT | O_APPEND, FILE_PERM);
        if (fd >= 0) {
            memset (&r, 0, sizeof (Report));
            r.id = (int) time (NULL);
            strncpy(r.inspectorName, user, MAX_INSPECTOR_NAME - 1);
            write(fd, &r, sizeof (Report));
            close(fd);
            printf("Operatiune realizata cu succes! Raportul adaugat in %s cu ID-ul %d\n", district, r.id);
        }
        else {
            perror("Eroare la deschiderea fisierului reports.dat");
        }
    }
}

void handleRemoveReport (const char *district, int targetID) {
    char path[MAX_PATH];
    snprintf(path, MAX_PATH, "%s/reports.dat", district);

    int fd_in = open (path, O_RDONLY);

    if (fd_in < 0) {
        fprintf(stderr, "Eroare! Nu exista rapoarte in districtul %s\n", district);
        return ;
    }

    int fd_out = open ("temp.dat" , O_WRONLY | O_CREAT | O_TRUNC, FILE_PERM);
    
    if (fd_out < 0) {
        perror("Eroare critica la crearea fisierului temporar");
        close(fd_in);  
        return;
    }

    Report r;
    int found = 0;

    while (read (fd_in, &r, sizeof (Report)) == sizeof (Report)) {
        if (r.id != targetID) {
            write(fd_out, &r, sizeof (Report));
        }
        else {
            found = 1;
        }
    }

    close (fd_in);
    close (fd_out);

    if (found == 1) {
        unlink (path);
        rename ("temp.dat", path);
        fprintf(stderr, "Raportul cu ID-ul %d,  a fost eliminat cu succes din districtul %s\n", targetID, district);
    } else {
        unlink ("temp.dat");
        fprintf (stderr, "Raportul cu ID-ul %d nu a fost gasit!\n", targetID);
    }



}

