# -> TOOL FOLOSIT : Gemini 3 Flash 
Prompt trimis către AI:

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

Cerința 1: 
  Implementează funcția int parse_condition(const char *input, char *field, char *op, char *value);. Trebuie să fragmenteze un string de tip field:operator:value (ex: severity:>=:3). Folosește sscanf cu limitatori de lățime pentru a preveni buffer overflow. Validează câmpurile: severity, category, inspector, timestamp. Validează operatorii: ==, !=, <, <=, >, >=. Returnează 0 pentru succes și -1 pentru eroare de format sau validare. Cerința 2: Implementează funcția int match_condition(Report *r, const char *field, const char *op, const char *value);. Trebuie să evalueze dacă recordul r satisface condiția. Gestionarea tipurilor: severity (int), timestamp (long), category și inspector (string). Pentru string-uri, se vor suporta doar operatorii == și !=. Returnează 1 dacă este îndeplinită condiția, 0 în caz contrar. 
Te rog să oferi o implementare robustă, conformă cu standardul POSIX și ușor de explicat la prezentarea proiectului.”
