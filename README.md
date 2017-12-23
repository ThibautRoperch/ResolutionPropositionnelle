# ResolutionPropositionnelle

Résolution de problèmes en calcul propositionnel, application au problème des pigeons et des pigeonniers

Réalisé dans le cadre du cours de parallélisme, M2 Info

## Contributeurs

*Alice Bazanté
*Pierre Granier--Richard
*Pierre-Olivier Mainfroid
*Thibaut Roperch

## Contenu

Le script `launch.sh` permet d'utiliser le solveur facilement (il compile et exécute le projet) :

    bash launch.sh

Le programme `src/pigeons.cpp` permet de résoudre un problème des pigeons et des pigeonniers.
Le programme `src/main.cpp` permet de résoudre un autre problème du même type que les pigeons et les pigeonniers, depuis un fichier représentant les contraintes.

Le programme `src/comparaison_structures.cpp` permet de comparer en terme de temps les différentes structures envisagées pour ce projet (voir partie *Travail effectuée*).

Le script `perf.sh` permet de lancer et mesurer le temps d'exécution d'une instance satisfiable puis d'une instance insatisfiable du problème des pigeons et des pigeonniers :

    bash perf.sh

Le script `benchmark.sh` permet de tester et comparer toutes les méthodes pour une instance du problème des pigeons et des pigeonniers (pigeons=pigeonniers=4 par défaut) via le graphique qu'il génère :

    bash benchmark.sh 5             # l'entier donné correspond au plus grand problème (nombre max de pigeons et de pigeonniers)


## Compilation manuelle

    make

## Exécution manuelle d'une instance du problème des pigeons et des pigeonniers

Pour les méthodes non-MPI (méthodes 1, 2, 3 et 5) :

    ./bin/pigeons.exe -p 1 -c 2 -m 1 -d

Pour la méthode MPI (méthode 4) :

    mpirun -n 4 ./bin/pigeons.exe -p 1 -c 2 -m 4 -d

Avec :
* -p le nombre de pigeons (1) 
* -c le nombre de pigeonniers (cabanes) (2)
* -m l'identifiant de la méthode de calcul (1, 4)
* -d pour afficher les solutions sur la sortie standard

## Exécution manuelle d'une instance d'un autre problème

Pour les méthodes non-MPI (méthodes 1, 2, 3 et 5) :

    ./bin/main.exe -f fichier -m 1 -d

Pour la méthode MPI (méthode 4) :

    mpirun -n 4 ./bin/main.exe -f fichier -m 4 -d

Avec :
* -f le fichier contenant la représentation des variables et des contraintes du problème
* -m l'identifiant de la méthode de calcul (1, 4)
* -d pour afficher les solutions sur la sortie standard

Un exemple de fichier avec les explications de syntaxe est disponible (`pigeons.txt`).

## Résultats

    p = pigeon
    c = pigeonnier

    m1 = méthode brute  (approche naïve)
    m2 = méthode efficace (récursive) 
    m3 = méthode brute parallélisée avec OpenMP
    m4 = méthode brute parallélisée avec MPI
    m5 = méthode efficace parallélisée avec OpenMP

    une ligne de matrice = un pigeon
    une colonne de matrice = un pigeonnier

| p et c | Nombre de poss | Nombre de sol   | Temps d'exec m1 | Temps d'exec m2 | Temps d'exec m3 | Temps d'exec m4 | Temps d'exec m5 |
|--------|----------------|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|
| 2p 2c  | 15             | 2 solutions     |     0m00.001s   |    0m00.006s    |     0m00.018s   |     0m00.071s   |    0m00.030s    |
| 3p 3c  | 511            | 6 solutions     |     0m00.001s   |    0m00.007s    |     0m00.020s   |     0m00.076s   |    0m00.036s    |
| 4p 4c  | 65535          | 24 solutions    |     0m00.014s   |    0m00.007s    |     0m00.035s   |     0m00.077s   |    0m00.046s    |
| 5p 5c  | 33554431       | 120 solutions   |     0m03.157s   |    0m00.009s    |     0m01.440s   |     0m01.211s   |    0m00.068s    |
| 6p 6c  | 68719476735    | 720 solutions   |   144m39.399s   |    0m00.032s    |    50m26.262s   |    11m28.296s   |    0m00.159s    |
| 7p 7c  | 562949953421311| 5040 solutions  |       -         |    0m00.274s    |    +12h         |    13m08.667s   |    0m01.633s    |
| 8p 8c  |       -        | 40320 solutions |       -         |    0m01.877s    |        -        |    14m01.674s   |    0m22.901s    |


## Interprétation des résultats

Comme nous pouvons le constater avec la baterrie de tests effectuée et le fichier `comparaison.pdf`, on constate que la parallélisation avec openMP est plus rapide qu'avec MPI pour les problèmes de petite taille (5p, 5c). Au delà, MPI devient beaucoup plus performant. De plus, la méthode récursive (efficace) est dans tous les cas la meilleure. En effet, dans cette méthode on teste les contraintes pendant la construction de la solution se qui permet d'éviter de créer pleins de solutions inutilement.

## Travail effectué

Nous avons implémenté plusieurs algorithmes, chacun ayant une approche différente du problème :
* Approche naïve/brute (-m 1) : instanciation et vérification de toutes les possibilités avec les contraintes pour ne garder que les solutions (complexité élevée en terme de temps et d'espace)
* Approche efficace/récursive (-m 2) : vérification des contraintes lors de l'instanciation des variables (implantation plus efficace, gain de temps et d'espace par rapport à la méthode précédente)
Nous avons parallélisé la première méthode avec OpenMP ou MPI, donnant lieu respectivement à la 3ème et 4ème méthode de résolution.
Nous avons parallélisé la seconde méthode avec OpenMP, donnant lieu à la 5ème méthode de résolution.

Une comparaison des méthodes pour quelques instances du problème des pigeons et des pigeonniers est proposée dans le fichier `comparaison.pdf`, généré à l'issue de l'exécution du script `benchmark.sh`.

Nous avons également conçu une version ASP du problème afin de vérifier nos résultats (fichier `pigeons-asp`).

Nous avons généralisé le problème des pigeons et des pigeonniers ; ainsi, le programme `src/main.cpp` est capable de lire et traiter un fichier décrivant un problème modélisé en calcul propositionnel sous la forme de contraintes de cardinalité (α, β, V).

Un fichier décrivant un programme doit respecter la syntaxe suivante :
* Un commentaire *one line* est précédé du caractère `#`,
* Une variable est déclarée avec le caractère `v`, suivi du nom de la variable, puis de sa valeur (un entier supérieur à 0),
* Une contrainte (α, β, V) est déclarée avec le caractère `c`, suivi du mot clé `neg` si la contrainte est à inverser, du min (α), du max(β), et de la variable concernée par la contrainte (V).
Un exemple est disponible dans le fichier `pigeons.txt`.

### Stockage

Il faut éviter de stocker toutes les possibilités, car le nombre de possibilités pour un probleme est exponentiel (2^6*6, soit 68719476736 solutions pour 6 pigeons et 6 pigeonniers), saturant ainsi rapidement la mémoire.

Nous ne stockons ainsi que les solutions du problème.

### Structures

Étant donné qu'on connait à l'avance le nombre de solutions, les array sont plus optimisés que les vecteur (voir le fichier `cpp/comparaison_structures.cpp`).

Pour l'approche naïve, une solution est représentée par un entier (type défini `ull`) plutot que par un tableau de bool, simplifiant ainsi la représentation et la manipulation des solutions (moins de pointeurs, donc parrallélisation moins compliquée et moins de risques de problèmes de mémoire).
Une solution est un `ull` (`unsigned long long`), car le nombre de possibilités pour un problème est rapidement très élevé (2^6*6, soit 68719476736 solutions pour 6 pigeons et 6 pigeonniers). Borne max de `ull` : 2(64)-1, l'instance pigeons=pigeonniers=8 est donc intraitable.

Pour l'approche récursive, une solution est représentée par un tableau de bool ; ainsi, la limite du type `ull` (2^(64)-1) n'existe pas, et cette méthode est capable de résoudre des problèmes de plus grande taille que pigeons=pigeonniers=8.

Un tableau de dimensions (`unsigned int`) représente les variables du problème (pigeons et cabanes étant un problème à deux dimensions, les dimensions sont donc p et c, avec p le nombre de pigeons et c le nombre de pigeonniers).

Un contrainte de cardinalité (α, β, V) est est représentée par son signe (contrainte à valider ou à falsifier), un alpha (min), un beta (max) et l'indice de la dimension sur laquelle la containte est appliquée.

## Limites d'implémentation

### Approche naïve/brute (version avec bitset)

cabanes * pigeons <= 9999999 (initialisation de la taille de std::bitset, nombre de cases dans la matrice pigeons/pigeonniers)
pow(2, cabanes*pigeons)-1 < 18 446 744 073 709 551 615 (unsigned long long, nombre de matrices possibles)
pow(2, cabanes*pigeons)-1 < myvector.max_size() (nombre de matrices possibles)

Cette version de la méthode brute n'est plus présente dans notre code ; elle est remplacée par la version suivante.

### Approche naïve/brute (version avec conversion en binaire "à la main")

produit de la taille des dimensions < 4 294 967 295 (taille d'une solution au format binaire)
pow(2, produit de la taille des dimensions)-1 < 2^(64)-1 (nombre de possibilités)

### Approche efficace/récursive

Avec cette approche, le nombre de possibilités n'a pas besoin d'être calculé et toutes les possibilités n'ont pas besoin d'être instanciées. De plus, une solution n'est pas représentée par un entier (`ull`), mais par un tableau de `bool`. Ainsi, la limite du type `ull` (2^(64)-1) n'est pas présente pour cette approche.

## TODO

Interprétation des résultats

Batterie de tests pour m2 et m5 (on peut aller au-delà de 8p 8c)
