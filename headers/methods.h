#ifndef METHODS_H
#define METHODS_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <numeric>
#include <functional>
#include <cmath>

#include "common.h"
#include "constraints.h"
#include "MPI_Reducer.h"

using namespace std;

/**********************************
 * Solver brut (approche naïve)
 */

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

		// if (i % 100 == 0 || i == nb_possibilites) cout << "\r  Progression : " << i*100/nb_possibilites << " %";
	}

	cout << endl;
}


/**********************************
 * Solveur efficace
 */

/*bool* initSolutionVide(const int col, const int row) {  
  bool firstMatriceVide = new bool[col*row];
  for (int i=0; i<(col*row); ++i) {
    firstMatriceVide[i] = 0;
  }
  return firstMatriceVide;
}

bool test_solution_max(bool * solution, const int cabanes, const int pigeons) {
  // Contrainte 1 : un pigeon est dans un et un seul pigeonnier
  for (int i = 0; i < pigeons; ++i) {
    int nb_cabanes = 0;
    for (int j = 0; j < cabanes; ++j)
      nb_cabanes += (solution[i*cabanes + j]);
    if (nb_cabanes > 1) {
      return false;
    }
  }
  // Contrainte 2 : un pigeonnier accueille au plus un pigeon
  for (int i = 0; i < cabanes; ++i) {
    int nb_pigeons = 0;
    for (int j = 0; j < pigeons; ++j)
      nb_pigeons += (solution[i + j*cabanes]);
    if (nb_pigeons > 1) {
      return false;
    }
  }
  return true;
}

vector<bool*> solver_intelligent(vector<bool*> solutionsPrecedentes, int indexPigeon, const int cabanes, const int pigeons) {
  int indexStart = indexPigeon*cabanes;
  int size = cabanes*pigeon;

  vector<bool*> solutionsfinal;
  vector<bool*> solutionTmp;
  vector<bool*> solutions;
  for (auto solution : solutionsPrecedentes) {
    solutionTmp.clear();
    bool maSolutionTmp = new bool[size];
    std::copy(std::begin(solution), std::end(solution), std::begin(maSolutionTmp));
    solutionTmp.push_back(maSolutionTmp);
    for (int i=indexStart; i<(indexStart*cabanes); ++i) {
      solutions.clear();
      for (auto tmp : solutionTmp) {
        bool sol1 = new bool[size];
        std::copy(std::begin(tmp), std::end(tmp), std::begin(sol1));
        sol1[i] = 1;
        if (test_solution_max(sol1,cabanes,pigeons)) {
          solutions.push_back(sol1);
        }
        bool sol2 = new bool[size];
        std::copy(std::begin(tmp), std::end(tmp), std::begin(sol2));
        sol2[i] = 0;
        if (test_solution_max(sol2,cabanes,pigeons)) {
          solutions.push_back(sol2);
        }
      } 
      solutionTmp.clear();
      solutionTmp = solutions;      
    }
    solutionsfinal.insert(solutionsfinal.end(),solutionTmp.begin(),solutionTmp.end());
  }

  solutions.clear();
  solutionTmp.clear();
  return solutionsfinal;
}

void solver_efficace(vector<bool*> &solutions, const int cabanes, const int pigeons) {
  int index = 0;
  vector<bool*> solutionPartiel;
  bool* firstMatriceVide = initSolutionVide(cabanes,pigeons);
  print_solutions(firstMatriceVide,cabanes,pigeons);
  solutionPartiel.push_back(firstMatriceVide);
  while (index < pigeons) {
    solutionPartiel = solver_intelligent(solutionPartiel, index, cabanes, pigeons);
    index++;
  }

  // Test les solutions de nouveaux suivant les contraintes
  //

  solutions = solutionPartiel;  
}*/

/**********************************
 * Solver brut (approche naïve) parallélisé avec MPI
 */

/**
 * Calcul de toutes les possibilités et nettoyage pour ne garder que les solutions
 */
void solver_brut_mpi(vector<ulong> &solutions, vector<unsigned int> &dimensions) {
	MPI_Reducer reducer;
	reducer.solver_brut(solutions, dimensions);
}

#endif
