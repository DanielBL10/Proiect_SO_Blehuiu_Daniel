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

# Ce am făcut

   Sistem de filtrare binară: Am implementat un motor de căutare care citește direct structuri Report din fișierul reports.dat. ,am lucrat pur cu apeluri de sistem (read, open, lseek).
  Parsare manuală de condiții: Am construit un parser care descompune argumente de tip câmp:operator:valoare. Am ales să folosesc pointeri și adrese de memorie (strchr) pentru a extrage sub-stringurile, asigurând o validare riguroasă a lungimii datelor introduse.
  Gestiunea argumentelor (argv): Am sincronizat logica din main.c cu funcția de filtrare, astfel încât programul să știe exact unde se termină flag-urile de sistem și unde încep condițiile utilizatorului, evitând interpretarea eronată a numelui executabilului.

# Ce am schimbat

•	Eliminarea redundanței: Am eliminat verificările if(!field) din match_condition.
•	Controlul Indexului (conditionStartIndex): Am modificat manual logica din main.c pentru a fixa indexul de unde începe citirea condițiilor.
•	Sincronizarea câmpurilor: Am redenumit variabilele din codul generat pentru a se potrivi cu structura mea.






# Utilizarea AI (Inteligență Artificială) - Faza 1 și Faza 2
**Proiect:** City Manager System
**Tool Folosit:** Gemini 3 Flash

---

## FAZA 1: Gestiunea Fișierelor și a Permisiunilor

### 1. Unde am folosit AI
* În implementarea operațiunilor de intrare/ieșire la nivel scăzut (system calls: `open`, `read`, `write`) pentru manipularea fișierelor binare (`reports.dat`).
* La setarea și verificarea permisiunilor folosind măști pe biți direct pe sistemul de fișiere (`chmod`, `fchmod`, `stat`).
* În logica de creare și gestionare a legăturilor simbolice (symlinks) și a directoarelor (`mkdir`, `unlink`).

### 2. De ce am folosit AI
* Pentru a mă asigura că respect cu strictețe constrângerile proiectului (evitarea funcțiilor din biblioteca standard C precum `fopen` sau `fprintf` pentru parsarea/scrierea datelor brute).
* Pentru a depana erori de memorie (Segmentation Faults) apărute la gestionarea pointerilor și parsarea argumentelor din linia de comandă.
* Pentru a înțelege corect logica de shiftare a blocurilor de memorie (`lseek`, `ftruncate`) necesară la ștergerea unei înregistrări din interiorul fișierului binar, fără a corupe restul datelor.

### 3. Cum am folosit AI (Studiu de caz: Motorul de filtrare)
Am oferit asistentului AI fragmente din cerința PDF și am solicitat exclusiv abordări compatibile cu standardul POSIX. Iată un exemplu concret pentru funcțiile critice `parse_condition` și `match_condition`.

**Prompt trimis către AI:**
> Sunt student la Sisteme de Operare (SO) și lucrez la un proiect în C care gestionează rapoarte urbane. Am nevoie de asistență pentru implementarea a două funcții critice pentru motorul de filtrare al aplicației... (am furnizat structura `Report` și cerințele specifice pentru parsarea string-urilor de tip `field:operator:value`). Te rog să oferi o implementare robustă, conformă cu standardul POSIX și ușor de explicat.

**Rezultat generat de AI (extras):**
AI-ul a furnizat o implementare bazată pe `sscanf` cu limitatori de lățime (`%15[^:]:%3[^:]:%31s`) pentru a preveni buffer overflow și o logică de validare strictă a operatorilor și câmpurilor.

**Implementare și Modificări Personale:**
* **Ce am făcut:** Am integrat motorul de căutare să citească direct structuri `Report` din `reports.dat` folosind pur apeluri de sistem (`read`, `open`, `lseek`). Am sincronizat logica din `main.c` cu funcția de filtrare pentru a separa corect flag-urile de sistem de condițiile utilizatorului.
* **Ce am schimbat:** Nu am preluat codul "orb". Am eliminat redundanța (ex: verificările `if(!field)` inutile din `match_condition`), am modificat manual logica din `main.c` pentru a controla indexul de pornire (`conditionStartIndex`) și am redenumit variabilele pentru a se potrivi exact cu arhitectura proiectului meu.

---

## FAZA 2: Procese, Semnale și IPC

### 1. Unde am folosit AI
* În scrierea programului independent `monitor_reports.c` pentru tratarea asincronă a semnalelor (`SIGINT`, `SIGUSR1`).
* În programul `city_manager`, la comanda `remove_district`, pentru crearea proceselor copil (`fork`) și apelarea sigură a comenzilor externe prin familia `exec*` (`execlp`).
* În comunicarea între procese (IPC), utilizând `kill(pid, 0)` pentru verificarea existenței monitorului și `pipe` pentru transmiterea codurilor de eroare de la procesul fiu la procesul părinte.

### 2. De ce am folosit AI
* Pentru a înlocui corect vechiul apel `signal()` cu funcția recomandată și robustă `sigaction()`, conform cerințelor stricte impuse pentru această fază.
* Pentru a implementa un mecanism de siguranță la apelarea comenzii `rm -rf` dintr-un proces fiu, prevenind ștergerea accidentală a datelor utile de pe sistem (verificarea directoarelor `/` sau `.`).
* Pentru a gestiona corect "race conditions" și a asigura scrierea "async-signal-safe" în handlerele de semnal (folosirea exclusivă a `write` la ieșirea standard, în detrimentul `printf`).

### 3. Cum am folosit AI
* **Generare și adaptare:** Am solicitat structuri de bază pentru configurarea `sigaction` și izolarea proceselor, adaptându-le apoi la logica aplicației mele.
* **Depanare avansată (Debugging logic):** AI-ul a identificat erori subtile de flux, cum ar fi necesitatea de a jurnaliza operațiunea de ștergere a districtului *înainte* ca fișierul de log să fie distrus fizic de comanda `rm` din copil.
* **Scenarii de testare:** Am folosit AI-ul pentru a concepe pașii exacți de testare concurentă (dual-terminal), validând atât comunicarea cu succes, cât și robuștețea sistemului în cazul în care monitorul lipsește (fallback error handling).