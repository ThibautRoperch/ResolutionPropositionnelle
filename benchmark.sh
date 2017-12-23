#!/bin/sh

# Compilation des sources

make clean
make

# Définition des dimensions et des autres variables

if [ $1 ]
then
	dimensions=`seq 1 $1`
else
	dimensions=`seq 1 4` # nb de pigeons et de pigeonniers
fi
methodes=`seq 1 5` # méthodes
executable="bin/pigeons.exe"
outputs="outputs/"; # sortie des fichiers

# Exécution de l'exécutable pour chaque couple pigeons/pigeonniers et pour chaque méthode

for methode in $methodes
do
	echo -e "Exécution de la méthode $methode"

	sortie=$outputs"method_"$methode".gpd"
	
	echo -e "#dimension\ttime" > $sortie

	for pp in $dimensions
	do
		echo -e "\tRésolution du problème pigeons = pigeonniers = $pp"
        if [ "$methode" = "4" ]
		then
			res=$({ /usr/bin/time -f "%e" mpirun -n 4 ./$executable -p $pp -c $pp -m $methode >> /dev/null; } 2>&1)
		else
			res=$({ /usr/bin/time -f "%e" ./$executable -p $pp -c $pp -m $methode >> /dev/null; } 2>&1)
		fi
		echo -e "$pp\t$res" >> $sortie
	done
done

gnuplot "comparaison.gps"
