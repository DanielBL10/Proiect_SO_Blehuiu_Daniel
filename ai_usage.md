# -> TOOL FOLOSIT : Gemini 3 Flash 

# Prompt trimis către AI:   
```
  Sunt student la Sisteme de Operare(SO) și lucrez la un proiect în C care gestionează rapoarte urbane. Am nevoie de asistență pentru implementarea a două funcții critice pentru motorul de filtrare al aplicației.

Structura de date utilizată este: 

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
```
# Cerința 1: 
  Implementează funcția int parse_condition(const char *input, char *field, char *op, char *value);. Trebuie să fragmenteze un string de tip field:operator:value (ex: severity:>=:3). Folosește sscanf cu limitatori de lățime pentru a preveni buffer overflow. Validează câmpurile: severity, category, inspector, timestamp. Validează operatorii: ==, !=, <, <=, >, >=. Returnează 0 pentru succes și -1 pentru eroare de format sau validare. 

# Raspuns 1:
Salut! Iată implementarea profesională a celor două funcții, optimizate pentru siguranță și claritate.
A. Funcția parse_condition
Această funcție utilizează seturi de scanare ([^:]) pentru a extrage datele fără a altera string-ul original.

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int parse_condition(const char *input, char *field, char *op, char *value) {
    if (!input || !field || !op || !value) return -1;

    // Scanare sigura cu limite de lungime
    // %15[^:] citeste pana la 15 caractere pana la intalnirea ':'
    int matched = sscanf(input, "%15[^:]:%3[^:]:%31s", field, op, value);

    if (matched != 3) return -1;

    // Validare campuri
    const char *valid_fields[] = {"severity", "category", "inspector", "timestamp"};
    int field_ok = 0;
    for (int i = 0; i < 4; i++) {
        if (strcmp(field, valid_fields[i]) == 0) { field_ok = 1; break; }
    }
    if (!field_ok) return -1;

    // Validare operatori
    const char *valid_ops[] = {"==", "!=", "<", "<=", ">", ">="};
    int op_ok = 0;
    for (int i = 0; i < 6; i++) {
        if (strcmp(op, valid_ops[i]) == 0) { op_ok = 1; break; }
    }
    if (!op_ok) return -1;

    return 0;
}
```

 # Cerința 2:
  Implementează funcția int match_condition(Report *r, const char *field, const char *op, const char *value);. Trebuie să evalueze dacă recordul r satisface condiția. Gestionarea tipurilor: severity (int), timestamp (long), category și inspector (string). Pentru string-uri, se vor suporta doar operatorii == și !=. Returnează 1 dacă este îndeplinită condiția, 0 în caz contrar. 
Te rog să oferi o implementare robustă, conformă cu standardul POSIX și ușor de explicat la prezentarea proiectului.”

# Raspuns 2:

Această funcție realizează conversia necesară a valorii de test (din string în tipul numeric corespunzător) și aplică operatorul logic.

```c
int match_condition(Report *r, const char *field, const char *op, const char *value) {
    if (!r || !field || !op || !value) return 0;

    // --- CAZUL 1: Campuri Numerice (severity, timestamp) ---
    if (strcmp(field, "severity") == 0 || strcmp(field, "timestamp") == 0) {
        long r_val = (strcmp(field, "severity") == 0) ? r->severity : (long)r->timestamp;
        long test_val = atol(value);

        if (strcmp(op, "==") == 0) return r_val == test_val;
        if (strcmp(op, "!=") == 0) return r_val != test_val;
        if (strcmp(op, "<") == 0)  return r_val < test_val;
        if (strcmp(op, "<=") == 0) return r_val <= test_val;
        if (strcmp(op, ">") == 0)  return r_val > test_val;
        if (strcmp(op, ">=") == 0) return r_val >= test_val;
    }

    // --- CAZUL 2: Campuri String (category, inspector) ---
    // Mapam 'inspector' din terminal la 'inspectorName' din structura
    const char *r_str = (strcmp(field, "category") == 0) ? r->category : r->inspectorName;
    int cmp = strcmp(r_str, value);

    if (strcmp(op, "==") == 0) return cmp == 0;
    if (strcmp(op, "!=") == 0) return cmp != 0;
    if (strcmp(op, "<") == 0)  return cmp < 0;   // r_str e inainte de value alfabetic
    if (strcmp(op, "<=") == 0) return cmp <= 0;
    if (strcmp(op, ">") == 0)  return cmp > 0;   // r_str e dupa value alfabetic
    if (strcmp(op, ">=") == 0) return cmp >= 0;

    return 0; 
}

```
