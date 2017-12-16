# ResolutionPropositionnelle

Résolution de problèmes en calcul propositionnel, application au problème des pigeons et des pigeonniers

Alice Bazanté
Pierre Granier--Richard
Pierre-Olivier Mainfroid
Thibaut Roperch

## Compiler

    make

## Exécuter

Avec :
* -p le nombre de pigeons (1) 
* -c le nombre de pigeonniers (cabanes) (2)
* -m l'identifiant de la méthode de calcul (1)

Pour les méthodes non-MPI (méthodes 1 à 3) :

    ./bin/main.exe -p 1 -c 2 -m 1

Pour la méthode MPI (méthode 4) :

    mpirun -n 4 ./bin/main.exe -p 1 -c 2 -m 4

## Travail effectué

Nous avons implémenté plusieurs algos...
version asp avec les contraintes des pigeons pour vérifier les résultats

pour set les constraints, go dans headers/constraints.h

### Stockage

ne pas stocker toutes les possibilités, car la mémoire risque de saturer rapidement
stocker les entiers correspondant auxsolutions, plutot que les matrices de binaires
changer la méthode de print d'une solution en conséquence

### Structures

Étant donné qu'on connait à l'avance le nombre de solutions, les array sont plus optis que les vecteur (comparaison_structures.cpp)
Une solution est représentée par un entier (ulong) plutot que par un tableau de bool, simplifiant ainsi la représentation et la manipulation des solutions (moins de pointeurs donc parrallélisation moins compliquée et moins de risques de problèmes de mémoire)
On travail avec des ulong, car le nombre de possibilités pour un probleme est exponentiel (68719476736 solutions pour 6 pigeons et 6 pigeonniers (2^6*6))

Tableau de dimensions pour représenter les variables du problème (pigeons, cabanes étant un pb à deux dimensions)

## Résultats

    p = pigeon
    c = pigeonnier

    m1 = méthode brute 
    m2 = méthode efficace (récursive) 

    une ligne de matrice = un pigeon
    une colonne de matrice = un pigeonnier

| p et c | Nombre de sol | Temps d'exec m1 (brut) | Temps d'exec m2 (efficace) | Temps d'exec m3 (brut avec openMP) | Temps d'exec m4 (brut avec MPI) |
|--------|---------------|------------------------|----------------------------|------------------------------------|---------------------------------|
| 2p 2c  | 2 solutions   |      0m00.001s         |                            |                                    |           0m00.071s             |
| 3p 3c  | 6 solutions   |      0m00.001s         |                            |                                    |           0m00.076s             |
| 4p 4c  | 24 solutions  |      0m00.008s         |                            |              01.644s               |                                 | 
| 5p 5c  | 120 solutions |      0m03.157s         |                            |              08.468s               |           0m01.211s             | 
| 6p 6c  | 720 solutions |    144m39.399s         |                            |                                    |          11m28.296s             |
| 7p 7c  | ??? solutions |                        |                            |                                    |          13m08.667s             |

## Limites d'implémentation

### Algo brut (version avec bitset)

cabanes * pigeons <= 9999999 (initialisation de la taille de std::bitset, nombre de cases dans la matrice pigeons/pigeonniers)
pow(2, cabanes*pigeons) < 18 446 744 073 709 551 615 (unsigned long long, nombre de matrices possibles)
pow(2, cabanes*pigeons) < myvector.max_size() (nombre de matrices possibles)

### Algo brut (version avec conversion en binaire "à la main")

produit de la taille des dimensions < 4 294 967 295 (taille d'une solution au format binaire)
pow(2, produit de la taille des dimensions) < 18 446 744 073 709 551 615 (nombre de possibilités)

## TODO

* Faire le solver intuitivement efficace (solveur_efficace)

* Exporter les résultats vers un graphe, comme au premier TP, pour être le meilleur groupe

* Représentation des contraintes
