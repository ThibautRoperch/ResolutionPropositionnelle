# ResolutionPropositionnelle

Résolution de problèmes en calcul propositionnel, application au problème des pigeons et des pigeonniers

Réalisé dans le cadre du cours de parallélisme, M2 Info

## Participants

Alice Bazanté
Pierre Granier--Richard
Pierre-Olivier Mainfroid
Thibaut Roperch

## Contenu

Le script launch.sh permet d'utiliser le solveur facilement (il compile et exécute les programmes) :

    bash launch.sh

Le programme src/solver.cpp permet de résoudre un problème de manière non parallèle, via deux approches et méthodes différentes.
Le programme src/psolver.cpp permet de résoudre un problème de manière parallèle en utilisant MPI (4 threads) ou OpenMP.

Le programme src/comparaison_structures.cpp permet de comparer en terme de temps les différentes structures envisagées pour ce projet.

Le script perf.sh permet de lancer et mesurer le temps d'exécution d'une instance satisfiable puis d'une instance insatisfiable du problème des pigeons.

Le script benchamrk.sh permet de tester et comparer toutes les méthodes pour une instance du problème des pigeons et des pigeonniers (de 1 à 6) via le graphique qu'il génère.

## Compilation

    make

## Exécution d'une instance du problème des pigeons et des pigeonniers

Pour les méthodes non-MPI (méthodes 1 à 3) :

    ./bin/main.exe -p 1 -c 2 -m 1 -d

Pour la méthode MPI (méthode 4) :

    mpirun -n 4 ./bin/main.exe -p 1 -c 2 -m 4 -d

Avec :
* -p le nombre de pigeons (1) 
* -c le nombre de pigeonniers (cabanes) (2)
* -m l'identifiant de la méthode de calcul (1, 4)
* -d pour afficher les solutions sur la sortie standard

## Exécution d'une instance d'un autre problème

Pour les méthodes non-MPI (méthodes 1 à 3) :

    ./bin/main.exe -f fichier -m 1 -d

Pour la méthode MPI (méthode 4) :

    mpirun -n 4 ./bin/main.exe -f fichier -m 4 -d

Avec :
* -f le fichier contenant la représentation des variables et des contraintes du problème
* -m l'identifiant de la méthode de calcul (1, 4)
* -d pour afficher les solutions sur la sortie standard

## Travail effectué

Nous avons implémenté plusieurs algorithmes, chacun ayant une approche différente du problème :
* Approche naïve/brute (-m 1) : instanciation et vérification de toutes les possibilités avec les contraintes pour ne garder que les solutions (complexité élevée en terme de temps et d'espace)
* Approche efficace (-m 2) : vérification des contraintes lors de l'instanciation des variables (implantation plus efficace)
Nous avons parallélisé la seconde méthode avec OpenMP ou MPI, donnant lieu respectivement à la 3 ème et 4 ème méthode de résolution d'un problème.

Une comparaison des méthodes pour quelques instances du problème des pigeons et des pigeonniers est proposée dans le fichier comparaison.pdf, généré à l'issue de l'exécution du script benchmark.sh.

Nous avons également conçu une version ASP du problème afin de vérifier nos résultats.

### Stockage

Il faut éviter de stocker toutes les possibilités, car le nombre de possibilités pour un probleme est exponentiel (2^6*6, soit 68719476736 solutions pour 6 pigeons et 6 pigeonniers), saturant ainsi rapidement la mémoire.

Nous ne stockons ainsi que les solutions du problème.

### Structures

Étant donné qu'on connait à l'avance le nombre de solutions, les array sont plus optimisés que les vecteur (voir le fichier cpp/comparaison_structures.cpp).

Une solution est représentée par un entier (type défini ull) plutot que par un tableau de bool, simplifiant ainsi la représentation et la manipulation des solutions (moins de pointeurs, donc parrallélisation moins compliquée et moins de risques de problèmes de mémoire)

Une solution est un ull (unsigned long long), car le nombre de possibilités pour un problème est rapidement très élevé (2^6*6, soit 68719476736 solutions pour 6 pigeons et 6 pigeonniers). Borne max de ull : 2(64)-1, l'instance pigeons=pigeonniers=8 est donc intraitable.

Un tableau de dimensions (unsigned int) représente les variables du problème (pigeons et cabanes étant un problème à deux dimensions, les dimensions sont donc p et c, avec p le nombre de pigeons et c le nombre de pigeonniers).

## Résultats

    p = pigeon
    c = pigeonnier

    m1 = méthode brute 
    m2 = méthode efficace (récursive) 
    m3 = méthode brute parallélisée avec OpenMP
    m4 = méthode brute parallélisée avec MPI

    une ligne de matrice = un pigeon
    une colonne de matrice = un pigeonnier

| p et c | Nombre de poss | Nombre de sol | Temps d'exec m1 | Temps d'exec m2 | Temps d'exec m3 | Temps d'exec m4 |
|--------|----------------|---------------|-----------------|-----------------|-----------------|-----------------|
| 2p 2c  | 15             | 2 solutions   |     0m00.001s   |                 |     0m00.018s   |     0m00.071s   |
| 3p 3c  | 511            | 6 solutions   |     0m00.001s   |                 |     0m00.020s   |     0m00.076s   |
| 4p 4c  | 65535          | 24 solutions  |     0m00.008s   |                 |     0m00.031s   |     0m00.077s   | 
| 5p 5c  | 33554431       | 120 solutions |     0m03.157s   |                 |     0m01.440s   |     0m01.211s   | 
| 6p 6c  | 68719476735    | 720 solutions |   144m39.399s   |                 |    50m26.262s   |    11m28.296s   |
| 7p 7c  | 562949953421311| ??? solutions |                 |                 |                 |    13m08.667s   |
| 8p 8c  |                | ??? solutions |                 |                 |                 |    14m01.674s   |

## Limites d'implémentation

### Approche naïve/brute (version avec bitset)

cabanes * pigeons <= 9999999 (initialisation de la taille de std::bitset, nombre de cases dans la matrice pigeons/pigeonniers)
pow(2, cabanes*pigeons)-1 < 18 446 744 073 709 551 615 (unsigned long long, nombre de matrices possibles)
pow(2, cabanes*pigeons)-1 < myvector.max_size() (nombre de matrices possibles)

### Approche naïve/brute (version avec conversion en binaire "à la main")

produit de la taille des dimensions < 4 294 967 295 (taille d'une solution au format binaire)
pow(2, produit de la taille des dimensions)-1 < 2^(64)-1 (nombre de possibilités)

## TODO

* Faire le solver intuitivement efficace (solveur_efficace)

* Exporter les résultats vers un graphe, comme au premier TP, pour être le meilleur groupe

* Représentation des contraintes -> tibo du 16 au 17/12

* Renommer les fichiers comme il faut (cf l'énoncé), virer les fichiers old -> je le fais ce soir (tibo, sam 16/12)

pour set les contraintes, go dans contraints.h