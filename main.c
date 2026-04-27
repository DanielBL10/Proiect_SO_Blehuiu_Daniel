#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>    
#include <unistd.h>   
#include <sys/stat.h> 

#include "city_manager.h"

int main (int argc, char *argv[]) {
    
    char *role = NULL;
    char *district = NULL;
    char *command = NULL;
    char *reportID = NULL;
    char *user = "anonim";
    int targetID = -1;
    int conditionStartIndex = -1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--role") == 0 && (i + 1 < argc)) {
            role = argv[++i];
        }
        else if (strcmp(argv[i], "--user") == 0 && (i + 1 < argc)) {
            user = argv[++i];
        }
        else if (strcmp(argv[i], "--add") == 0 && (i + 1 < argc)) {
            command = "add";
            district = argv[++i];
        }
        else if (strcmp(argv[i], "--remove_report") == 0 && (i + 2 < argc)) {
            command = "remove_report";
            district = argv[++i];
            reportID = argv[++i]; 
        }
        else if (strcmp(argv[i], "--list") == 0 && (i + 1 < argc)) {
            command = "list";
            district = argv[++i];
        }
        else if (strcmp(argv[i], "--view") == 0 && (i + 2 < argc)) {
            command = "view";
            district = argv[++i];
            reportID = argv[++i]; 
        }
        else if (strcmp(argv[i], "--update_threshold") == 0 && (i + 2 < argc)) {
            command = "update_threshold";
            district = argv[++i];
            reportID = argv[++i];  
        }
        else if (strcmp(argv[i], "--filter") == 0 && (i + 1 < argc)) {
            command = "filter";
            district = argv[++i];
            conditionStartIndex = i + 1; // CORECTAT AICI (nu 0)
            break; 
        }
    }

    if (!role || !district || !command) {
        fprintf(stderr, "Lipsesc argumentele esentiale (role, district, comanda)!\n");
        exit(2);
    }

    if ((strcmp(role, "manager") != 0) && (strcmp(role, "inspector") != 0)) {
        fprintf(stderr, "Eroare! Rolul selectat este invalid! Variantele obligatorii sunt manager / inspector!\n");
        exit(3);
    }

  
    if (strcmp(command, "add") == 0) {
        printf("Vom adauga un raport in districtul %s, rolul: %s, utilizatorul: %s\n", district, role, user);
        handleAdd(district, role, user);
    }
    else if (strcmp(command, "remove_report") == 0) {
        if (!reportID) {
            fprintf(stderr, "Eroare! Comanda remove_report necesita utilizarea unui reportID!\n");
            exit(4);
        }
        
        if (strcmp(role, "manager") != 0) {
            fprintf(stderr, "Eroare! Nu aveti dreptul de a sterge rapoarte (necesita rol de manager)!\n");
            exit(5);
        }

        targetID = atoi(reportID);
        if (targetID <= 0) {
            fprintf(stderr, "Eroare! ReportID trebuie sa fie un numar valid, intreg pozitiv!\n");
            exit(6);
        }

        handleRemoveReport(district, targetID, role, user);
    }
    else if (strcmp(command, "list") == 0) {
        printf("Vom lista rapoartele din districtul %s\n", district);
        handleList(district);
    }
    else if (strcmp(command, "view") == 0) {
        if (!reportID) {
            fprintf(stderr, "Eroare! Comanda view necesita un reportID!\n");
            exit(7);
        }
        targetID = atoi(reportID);
        view(district, targetID, role);
    }
    else if (strcmp(command, "update_threshold") == 0) {
        if (!reportID) {
            fprintf(stderr, "Eroare! Lipseste valoarea pentru threshold!\n");
            exit(8);
        }
        int newValue = atoi(reportID);
        update_threshold(district, role, newValue);
    }
    else if (strcmp(command, "filter") == 0) {
        if (conditionStartIndex == -1 || conditionStartIndex >= argc) {
            fprintf(stderr, "Eroare! Comanda filter necesita cel putin o conditie (ex: severity:>=:2)!\n");
            exit(9);
        }

        int numConditions = argc - conditionStartIndex;
        handleFilter(district, numConditions, &argv[conditionStartIndex]);
    }

    return 0;
}