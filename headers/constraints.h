#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

#include <vector>

/**
 * Les contraintes appliquées à une solution représentée par en binaire
 */
bool test_solution_final(bool* solution, const std::vector<unsigned int> &dimensions) {
	unsigned int pigeons = dimensions[0];
	unsigned int cabanes = dimensions[1];

	// Contrainte 1 : un pigeon est dans un et un seul pigeonnier
	for (unsigned int i = 0; i < pigeons; ++i) { // parcours des pigeons
		int nb_cabanes = 0;
		for (unsigned int j = 0; j < cabanes; ++j) { // comptage des cabanes du pigeon i
			nb_cabanes += (solution[i * cabanes + j]) ? 1 : 0;
		}
		if (nb_cabanes != 1) { // vérification du min (1) et du max (1)
			return false;
		}
	}

	// Contrainte 2 : un pigeonnier accueille au plus un pigeon
	for (unsigned int i = 0; i < cabanes; ++i) { // parcours des cabanes
		int nb_pigeons = 0;
		for (unsigned int j = 0; j < pigeons; ++j) { // comptage des pigeons de la cabane i
			nb_pigeons += (solution[i + j * cabanes]) ? 1 : 0;
		}
		if (nb_pigeons > 1) { // vérification du min (0) et du max (1)
			return false;
		}
	}

	return true;
}

#endif
