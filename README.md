# ResolutionPropositionnelle

Résolution de problèmes en calcul propositionnel, application au problème des pigeons et des pigeonniers

Alice Bazanté
Pierre Granier--Richard
Pierre-Olivier Mainfroid
Thibaut Roperch

## Compiler

    make

## Exécuter

-p : nombre de pigeon (1)
-c : nombre de pigeonniers, ou cabanes (2)
-m : méthode de calcul
-h : aide

Pour les méthodes non-MPI (méthodes 1 à 3) :

    ./bin/main.exe -p 1 -c 2 -m 1

Pour la méthode MPI (méthode 4) :

    mpirun -n 4 ./bin/main.exe -p 1 -c 2 -m 4

## Travail effectué

Nous avons implémenté plusieurs algos...

Choix de la structure : étant donné qu'on connait à l'avance le nombre de solutions, les array sont plus optis que les vecteur (comparaison_structures.cpp)
Une solution est un int plutot que un tableau de bool

## Résultats

    p = pigeon
    c = pigeonnier

    m1 = méthode brute
    m2 = méthode efficace (récursive)

    une ligne de matrice = un pigeon
    une colonne de matrice = un pigeonnier

| p et c | Nombre de sol | Temps d'exec m1 (brut) | Temps d'exec m2 (efficace) | Temps d'exec m3 (brut avec openMP) | Temps d'exec m4 (brut avec MPI) |
|--------|---------------|------------------------|----------------------------|------------------------------------|---------------------------------|
| 2p 2c  | 2 solutions   |                        |                            |                                    | 0m0.101s                        |
| 3p 3c  | 6 solutions   |                        |                            |                                    | 0m0.119s                        |
| 4p 4c  | 24 solutions  |                        |                            |                                    |                                 |
| 6p 6c  |               |                        |                            |                                    |                                 |

## Limites d'implémentation

### Algo brut

cabanes * pigeons <= 9999999 (initialisation de la taille de std::bitset, nombre de cases dans la matrice pigeons/pigeonniers)
pow(2, cabanes*pigeons) < 18 446 744 073 709 551 615 (unsigned long long, nombre de matrices possibles)
pow(2, cabanes*pigeons) < myvector.max_size() (nombre de matrices possibles)


## TODO

* Faire le solver intuitivement efficace (solveur_efficace)
* Paralléliser le solver bourrin avec OpenMP et MPI

* Refaire les fichiers au propre (faire une classe de solver, nommer les fichiers comme demandé dans le sujet)

* Faire le script (voir dernière partie de l'énoncé)
* Exporter les résultats vers un graphe, comme au premier TP, pour être le meilleur groupe

Remplacer unsigned long long par ulong

stockage :
ne pas stocker toutes les possibilités
stocker les entiers correspondant auxsolutions, plutot que les matrices de binaires
changer la méthode de print d'une solution en conséquence

structures :
une solution est un entier
tableau de dimensions à la place de cabanes * pigeons
