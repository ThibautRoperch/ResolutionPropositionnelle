# ResolutionPropositionnelle

Résolution de problèmes en calcul propositionnel

Application au problème des pigeons et des pigeonniers

## Compiler

    make

## Exécuter

    _./bin/main.exe -p 1 -c 2_
    mpirun -n 4 ./bin/mpi.exe

Avec 1 le nombre de pigeons et 2 le nombre de pigeonniers (cabanes)

## TODO

* Faire le solver intuitivement efficace (solveur_efficace)
* Paralléliser le solver bourrin avec OpenMP et MPI

* Refaire les fichiers au propre (faire une classe de solver, nommer les fichiers comme demandé dans le sujet)

* Faire le script (voir dernière partie de l'énoncé)
* Exporter les résultats vers un graphe, comme au premier TP, pour être le meilleur groupe

## Résultats

    p = pigeon
    c = pigeonnier

    m1 = méthode brute
    m2 = méthode efficace (récursive)

    une ligne de matrice = un pigeon
    une colonne de matrice = un pigeonnier

| p et c | Nombre de sol | Temps d'exec m1 | Temps d'exec m2 | Temps d'exec m1 openMP | Temps d'exec m1 MPI |
|--------|---------------|-----------------|-----------------|------------------------|---------------------|
| 2p 2c  | 2 solutions   |                 |                 |                        |                     |
| 3p 3c  | 6 solutions   |                 |                 |                        |                     |
| 4p 4c  | 24 solutions  |                 |                 |                        |                     |
| 6p 6c  |               |                 |                 |                        |                     |

## Limites d'implémentation

### Algo brut

cabanes * pigeons <= 9999999 (initialisation de la taille de std::bitset, nombre de cases dans la matrice pigeons/pigeonniers)
pow(2, cabanes*pigeons) < 18 446 744 073 709 551 615 (unsigned long long, nombre de matrices possibles)
pow(2, cabanes*pigeons) < myvector.max_size() (nombre de matrices possibles)
