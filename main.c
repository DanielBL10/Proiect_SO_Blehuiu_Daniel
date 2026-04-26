#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "city_manager.h"

int main (int argc, char *argv[]) {
    
    if (argc < 5) {
        fprintf(stderr, "Eroare numar argumente in linie de comanda!\n");
        exit(1);
    }

    // executabil -> opțiune_rol -> valoare_rol -> acțiune -> district
    
    char *role = NULL;
    char *district = NULL;
    char *command = NULL;
    char *reportID = NULL;
    char *user = "anonim";

    for (int i = 1; i < argc; i++) {
        if (strcmp (argv[i], "--role") == 0 && (i + 1 < argc)) {
            role = argv[++i];
        }
        else if (strcmp(argv[i], "--user") == 0 && (i + 1 < argc)) {
            user = argv[++i];
        }
        else if (strcmp (argv[i], "--add") == 0 && (i + 1 < argc)) {
            command = "add";
            district = argv[++i];
            
        }
        else if (strcmp (argv[i], "--remove_report") == 0 && (i + 2 < argc)) {
            command = "remove_report";
            district = argv[++i];
            reportID = argv[++i]; 
        }
    }

    if (!role || !district || !command) {
            fprintf(stderr, "Lipsesc argumentele esentiale pentru user roles!\n");
            exit(2);
        }

    if ((strcmp (role, "manager") != 0) && (strcmp (role, "inspector") != 0)) {
        fprintf(stderr, "Eroare! Rolul selectat este invalid! Variantele oblogatorii sunt manager / inspector!\n");
        exit(3);
    }

    if (strcmp (command, "add") == 0) {
        printf("Vom adauga un raport in districtul %s, role-ul : %s, utilizatorul : %s\n", district, role, user);
        handleAdd(district, role, user);
    }

    if (strcmp (command, "remove_report") == 0) {
        if (!reportID) {
            fprintf(stderr, "Eroare! Comanda remove_report necesita utlizarea unui reportID!\n");
            exit(4);
        }
        

        if (strcmp (role, "manager") != 0) {
            fprintf(stderr, "Eroare! Role-ul dumneavosatra nu are dreptul de a sterge rapoarte, intrucat nu aveti rolul de manager!\n");
            exit(5);
        }

        handleRemoveReport(district, atoi(reportID));  
    }

    return 0;
}   

// ./city_manager --role manager --remove_report downtown 17 => argc = 6


// gcc -Wall main.c city_manager.c -o city_manager

