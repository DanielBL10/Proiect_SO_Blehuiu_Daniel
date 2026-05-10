### City Manager - Manual de Utilizare

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


<img width="967" height="728" alt="image" src="https://github.com/user-attachments/assets/d8ecc5fb-2ae6-4454-90c1-0d9c5aad4189" />
