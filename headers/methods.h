#ifndef METHODS_H
#define METHODS_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <numeric>
#include <functional>
#include <cmath>

#include "constraints.h"

using namespace std;

/**
 * Écrit dans le tableau de booléen donné en paramètre la représentation binaire d'un nombre décimal
 */
void int_to_binary(ulong nombre, bool *res, unsigned int longueur) {
	unsigned int i = 0;
	while (nombre != 0 || i < longueur) {
		if (nombre % 2)
			res[i] = true;
		else
			res[i] = false;
		nombre /= 2;

		++i;
	}
}

/**
 * Calcul de toutes les possibilités et nettoyage pour ne garder que les solutions
 */
void solver_brut(vector<ulong> &solutions, const vector<unsigned int> &dimensions) {
	unsigned int longueur_solutions = accumulate(dimensions.begin()+1, dimensions.end(), dimensions[0], multiplies<int>());
	ulong nb_possibilites = pow(2, longueur_solutions) - 1;

	// Génération de toutes les possibilités et nettoyage pour ne garder que les solutions (application des contraintes)
	cout << "Génération des " << nb_possibilites << " possibilités et conservation des solutions" << endl;

	for (ulong i = 0; i <= nb_possibilites; ++i) {
		bool *poss = new bool[longueur_solutions];
		int_to_binary(i, poss, longueur_solutions);

		// Vérification des contraintes, conserver la solution au format décimal si c'en est une
		if (test_solution_final(poss, dimensions)) {
			solutions.push_back(i);
		}

		// Rendre la mémoire de la représentation binaire de la possibilité
		delete[] poss;

		if (i % 100 == 0 || i == nb_possibilites) cout << "\r  Progression : " << i*100/nb_possibilites << " %";
	}

	cout << endl;
}

#endif
