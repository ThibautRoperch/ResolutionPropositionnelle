#!/bin/sh

# Compilation des sources

make clean
make

# Définition des dimensions et des autres variables

pp=`seq 1 5` # nb de pigeons et de pigeonniers
methodes=`seq 1 4` # méthodes
executable="bin/main.exe"
outputs="outputs/"; # sortie des fichiers

# Exécution de l'exécutable pour chaque couple pigeons/pigeonniers et pour chaque méthode

#todo

gnuplot "comparaison.gps"
