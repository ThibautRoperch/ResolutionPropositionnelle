#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

#include <iostream>
#include <string>
#include <vector>

class Constraint {
	private:
		bool signe;
		int min;
		int max;
		unsigned int indice_dimension;
		std::string nom_dimension;

	public:
		Constraint(bool signe, int alpha, int beta, unsigned int V, std::string contrainte) : signe(signe), min(alpha), max(beta), indice_dimension(V), nom_dimension(contrainte) {};
		~Constraint() {};

		/**
		 * Test de la contrainte sur une potentielle solution représentée en binaire
		 */
		bool test(bool* solution, const std::vector<unsigned int> &dimensions) {
			for (unsigned int i = 0; i < dimensions[indice_dimension]; ++i) { // parcours de la dimension
				for (unsigned int j = 0; j < dimensions.size() && j != indice_dimension; ++j) { // pour chaque autre dimension
					int res = 0;
					for (unsigned int k = 0; k < dimensions[j]; ++k) { // comptage des true sur les autres dimensions
						res += (solution[i * dimensions[j] + k]) ? 1 : 0;
					}
					if (signe == true) {
						if (res < min || res > max) { // vérification du min et du max
							return false;
						}
					} else {
						if (res >= min || res <= max) { // vérification du min et du max
							return false;
						}
					}
				}
			}

			return true;
		}

		void afficher() {
			std::cout << signe << " (" << min << ", " << max << ", " << nom_dimension << ")" << std::endl;
		}

};

/**
 * Les contraintes appliquées à une solution représentée en binaire
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
