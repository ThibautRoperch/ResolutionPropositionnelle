#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <numeric>
#include <functional>

typedef unsigned long long ull; // 0 à 2^(64)-1

/**********************************
 * Fonctions communes aux méthodes
 */

/**
 * Écrit dans le tableau de booléen donné en paramètre la représentation binaire d'un nombre décimal
 */
void int_to_binary(ull nombre, bool *res, unsigned int longueur) {
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
 * Affiche sur la sortie standard les solutions d'un vecteur de solutions en fonction de la taille des dimensions données
 * Une solution est affichée sur plusieurs lignes (format matriciel) lorsque le problème est à deux dimensions (comme pigeons/cabanes)
 */
void print_solutions(const std::vector<ull> &solutions, const std::vector<unsigned int> &dimensions) {
  unsigned int solutions_length = std::accumulate(dimensions.begin()+1, dimensions.end(), dimensions[0], std::multiplies<int>());

  for (auto sol : solutions) {
    bool *sol_bin = new bool[solutions_length];
    int_to_binary(sol, sol_bin, solutions_length);
    
    if (dimensions.size() == 2) {
      for (unsigned int i = 0; i < dimensions[0]; ++i) {
        for (unsigned int j = 0; j < dimensions[1]; ++j) {
          std::cout << sol_bin[i*dimensions[1] + j] << " ";
        }
        std::cout << std::endl;
      }
      std::cout << std::endl;
    } else {
      for (unsigned int i = 0; i < solutions_length; ++i) {
        std::cout << sol_bin[i] << " ";
      }
      std::cout << std::endl;
    }

    delete[] sol_bin;
  }
}

#endif
