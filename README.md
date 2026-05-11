### City Manager - Manual de Utilizare

## Phase 1

# 1. Introducere

City Manager este un utilitar de linie de comandă dezvoltat în C pentru sisteme Linux/UNIX. Acesta permite gestionarea rapoartelor de infrastructură urbană prin 
stocare binară eficientă și un sistem de securitate bazat pe roluri (RBAC - Role Based Access Control).

# 2 Instalare și Compilare

Programul necesită un compilator GCC și un mediu POSIX (Linux, macOS sau WSL).

 # Sintaxa  
gcc -Wall main.c reports.c -o city_manager

# 3. Arhitectura de Securitate
Accesul la date este restricționat programatic. Aplicația verifică bitul de permisiune al fișierului reports.dat înainte de execuție:

Manager: Are drepturi de scriere/citire asupra fișierelor de tip owner (S_IRUSR, S_IWUSR).

Inspector: Are drepturi de scriere/citire asupra fișierelor de tip group (S_IRGRP, S_IWGRP).

# 4. Ghid de Comenzi

# Sintaxa

./city_manager --role [manager/inspector] --user [nume] --add [district] [lat] [long] [categorie] [severitate] "[descriere]"

./city_manager --role inspector --user bob --add Sector1 44.4 26.1 road 3 "Groapa mare"

4.2. Listarea rapoartelor (--list)
Afișează metadatele fișierului (permisiuni, mărime, timp) și toate înregistrările.

./city_manager --role inspector --user bob --list Sector1
4.3. Filtrarea avansată (--filter)
Căutare după criterii multiple. Operatorii acceptați: ==, !=, <, <=, >, >=.


./city_manager --role inspector --user bob --filter Sector1 category:==:road severity:>=:2
4.4. Administrare (--remove_report, --view)
Ștergere: Doar pentru Manageri. ./city_manager --role manager --user alice --remove_report Sector1 [ID]

./city_manager --role inspector --user bob --view Sector1 [ID]

# 5. Structura Datelor (Tehnic)
Informația este salvată într-o structură binară fixă (struct Report), asigurând o viteză de procesare ridicată și o amprentă mică pe disc.Componentele structurii sunt:
id, inspectorName, latitude, float longitude, category, severity, timestamp, descriptionText.

# Comenzi (mai general)

gcc -Wall main.c reports.c -o city_manager

./city_manager --role manager --user sefu --add Sector1
 
cat Sector1/logged_district

./city_manager --role inspector --user dorel --list Sector1

./city_manager --role inspector --user dorel --view Sector1 `<ID>`
 
./city_manager --role inspector --user dorel --filter Sector1 "severity:>=:1"
 
./city_manager --role manager --user sefu --update_threshold Sector1 3

./city_manager --role manager --user sefu --remove_report Sector1 `<ID>`

# 7. Exemplu comenzi (pas cu pas)

gcc -Wall main.c reports.c -o city_manager

./city_manager --role manager --user sefu --add Sector1

cat Sector1/logged_district

./city_manager --role inspector --user dorel --list Sector1

./city_manager --role inspector --user dorel --view Sector1 1777247848

./city_manager --role inspector --user dorel --filter Sector1 "severity:>=:1"

./city_manager --role manager --user sefu --update_threshold Sector1 3

./city_manager --role manager --user sefu --remove_report Sector1 1777247848


#8 Poze Phase 1

<img width="1514" height="1442" alt="image" src="https://github.com/user-attachments/assets/355d43c0-2416-4a55-9e15-f9a84b85401f" />

<img width="1685" height="772" alt="image" src="https://github.com/user-attachments/assets/d0b667c0-5f44-4e8a-a3ac-2a913bf27acf" />

<img width="1395" height="1454" alt="image" src="https://github.com/user-attachments/assets/0c0c98f8-5a6b-4405-a9ad-cc41750d5e35" />

<img width="978" height="1469" alt="image" src="https://github.com/user-attachments/assets/cea8168e-c95a-43a1-9c45-d5515f012d94" />

<img width="1008" height="719" alt="image" src="https://github.com/user-attachments/assets/835a478e-313e-431b-800f-fce38991c9e1" />


## Phase 2 : Processes and Signals

#2.1. Compilarea aplicațiilor
```bash
gcc -Wall monitor_reports.c -o monitor_reports
gcc -Wall main.c reports.c -o city_manager

# 2.2. Demonstrare IPC (Semnale) și Jurnalizare

Terminal 1 :
./monitor_reports

Terminal 2: Adăugarea unui raport de test (declanșează SIGUSR1)
./city_manager --role manager --user sefu --add Sector_Test

Terminal 2 : cat Sector_Test/logged_district



# 2.3. Demonstrare creare procese (fork + exec) și pipe
Terminal 2: Ștergerea recursivă a districtului de test
./city_manager --role manager --user sefu --remove_district Sector_Test

# 2.4. Demonstrare robuștețe (kill 0)
Terminal 1: Se oprește monitorul folosind Ctrl+C (declanșează SIGINT și șterge .monitor_pid)

Terminal 2: Adăugare raport cu monitorul oprit pentru a testa fallback-ul
./city_manager --role manager --user sefu --add Sector1

Terminal 2: Verificarea tratării erorii în log
cat Sector1/logged_district

# 2.5. Generarea structurii finale (Cerința: min. 2 districte, 5 rapoarte)
Se creează al doilea district obligatoriu (Sector1 are deja 4 rapoarte din testele anterioare)
Terminal 2 : ./city_manager --role manager --user sefu --add Sector2
Terminal 2 : Afișarea arborelui de directoare pentru a dovedi existența fișierelor și a symlink-urilor
tree

# Poze Phase 2

<img width="975" height="142" alt="image" src="https://github.com/user-attachments/assets/52df1a73-8a7a-4833-bb96-95cc31aab611" />

<img width="975" height="240" alt="image" src="https://github.com/user-attachments/assets/6ec3cbb3-4e73-4024-96de-203f09a3a444" />

<img width="975" height="166" alt="image" src="https://github.com/user-attachments/assets/6945e763-b632-4750-9c9f-7357d2c876e5" />

<img width="975" height="226" alt="image" src="https://github.com/user-attachments/assets/d1379c30-7785-402c-9d17-494b0931e77d" />

<img width="975" height="213" alt="image" src="https://github.com/user-attachments/assets/d5611a9b-85c8-4830-9952-af6bbae50567" />

<img width="975" height="198" alt="image" src="https://github.com/user-attachments/assets/9db93e5c-e701-45d1-8148-3836d4fa8c51" />


