#!/bin/bash

# Ce script permet d'utiliser le solveur de problèmes en calcul propositionnel

# Compile les sources si le dossier bin est vide ou inexistant
contenu_bin=$(ls -a bin | sed -e "/\.$/d" | wc -l)
if [ $contenu_bin -eq 0 ]
then
    make
fi

while [ true ]
do
    # Menu principal
    echo -e ""
    echo -e "[1] Recompiler les sources"
    echo -e "[2] Supprimer l'exécutable"
    echo -e "[3] Exécuter une instance du problème des pigeons et des pigeonniers"
    echo -e "[4] Résoudre un autre problème"
    echo -e "[0] Quitter"
    read action

    clear

    if [ "$action" = "0" ] # quitter
    then
        exit 0
    elif [ "$action" = "1" ] # compiler les sources
    then
        make
    elif [ "$action" = "2" ] # nettoyer l'executable
    then
        make clean
    elif [ "$action" = "3" ] # problème des pigeons et des pigeonniers
    then
        echo -e "Nombre de pigeons : "
        read pigeons

        echo -e "Nombre de pigeonniers : "
        read cabanes

        echo -e "Quelle méthode utiliser ?"
        echo -e " [1] Brute"
        echo -e " [2] Efficace"
        echo -e " [3] Brute OpenMP"
        echo -e " [4] Brute MPI"
        read methode

        echo -e "Afficher les solutions sur la sortie standard ? [O/n]"
        read afficher_solutions

        if [ "$afficher_solutions" = "O" ] || [ "$afficher_solutions" = "o" ]
        then
            afficher_solutions="-d"
        else
            afficher_solutions=""
        fi

        echo -e "\n"

        if [ "$methode" = "4" ]
        then
            echo -e "Nombre de processus à utiliser :"
            read processus
            time mpirun -n $processus ./bin/main.exe -p $pigeons -c $cabanes -m $methode $afficher_solutions
        else
            time ./bin/main.exe -p $pigeons -c $cabanes -m $methode $afficher_solutions
        fi
    elif [ "$action" = "4" ] # autre problème
    then
        # demander de donner le nom du fichier contenant les variables et les contraintes
        echo -e "- un autre parametre pour que le tab fonctionne"
    fi
done
