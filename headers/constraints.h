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
    unsigned int indice; // indice dans le vecteur de variables et de dimensions
    std::string nom_dimension;

  public:
    Constraint(bool signe, int alpha, int beta, unsigned int V) : signe(signe), min(alpha), max(beta), indice(V) {};
    ~Constraint() {};

    /**
     * Test de la contrainte sur une potentielle solution représentée en binaire
     */
    bool test(bool* solution, const std::vector<unsigned int> &dimensions) const {
      unsigned int prev_dim_size = 0;
      for (unsigned int dim = 0; dim < dimensions.size(); ++dim) { // pour chaque autre dimension du problème
        if (dim != indice) { // les autres dimensions, celle-ci exclue
          for (unsigned int i = 0; i < dimensions[indice]; ++i) { // parcours de la dimension de la contrainte
            int res = 0;
            for (unsigned int j = 0; j < dimensions[dim]; ++j) { // comptage des true sur les autres dimensions
                if (indice == 0) // première dimension
                  res += (solution[prev_dim_size + i * dimensions[dim] + j]) ? 1 : 0;
                else // autres dimensions
                  res += (solution[prev_dim_size + i + dimensions[indice] * j]) ? 1 : 0;
            }
            if (signe == true) {
              if (res < min || res > max) // vérification du min et du max
                return false;
            } else {
              if (res >= min || res <= max) // vérification du min et du max
                return false;
            }
          }
          prev_dim_size += dimensions[dim];
        }
      }

      return true;
    }

    void print(const std::vector<std::string> &variables, const std::vector<unsigned int> &dimensions) const {
      std::cout << signe << " (" << min << ", " << max << ", " << variables[indice] << " (" << dimensions[indice] << "))" << std::endl;
    }

};

/**
 * Test des contraintes appliquées à une solution représentée en binaire
 */
/*bool test_solution_final(bool* solution, const std::vector<unsigned int> &dimensions) {
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
}*/

bool valid_constraints(bool* solution, const std::vector<unsigned int> &dimensions, const std::vector<Constraint> &constraints) {
  for (auto constraint : constraints) {
    if (constraint.test(solution, dimensions) == false) {
      return false;
    }
  }
  return true;
}

/**
 * Les contraintes appliquées à une solution représentée en binaire suivant le max
 */
bool test_solution_partiel(bool* solution, const std::vector<unsigned int> &dimensions) {
	unsigned int pigeons = dimensions[0];
	unsigned int cabanes = dimensions[1];

	// Contrainte 1 : un pigeon est dans un et un seul pigeonnier
	for (unsigned int i = 0; i < pigeons; ++i) { // parcours des pigeons
		int nb_cabanes = 0;
		for (unsigned int j = 0; j < cabanes; ++j) { // comptage des cabanes du pigeon i
			nb_cabanes += (solution[i * cabanes + j]) ? 1 : 0;
		}
		if (nb_cabanes > 1) { // vérification du min (1) et du max (1)
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
