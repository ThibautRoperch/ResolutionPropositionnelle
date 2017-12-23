#ifndef METHODS_H
#define METHODS_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <numeric>
#include <functional>
#include <cmath>
#include <omp.h>

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
void solver_brut(vector<ull> &solutions, const vector<unsigned int> &dimensions, const vector<Constraint> &constraints) {
  unsigned int longueur_solutions = accumulate(dimensions.begin()+1, dimensions.end(), dimensions[0], multiplies<int>());
  ull nb_possibilites = pow(2, longueur_solutions) - 1;

  // Génération de toutes les possibilités et nettoyage pour ne garder que les solutions (application des contraintes)
  cout << "Génération des " << nb_possibilites << " possibilités et conservation des solutions" << endl;

  for (ull i = 0; i <= nb_possibilites; ++i) {
    bool *poss = new bool[longueur_solutions];
    int_to_binary(i, poss, longueur_solutions);

    // Vérification des contraintes, conserver la solution au format décimal si c'en est une
    if (valid_constraints(poss, dimensions, constraints)) {
      solutions.push_back(i);
    }

    // Rendre la mémoire de la représentation binaire de la possibilité
    delete[] poss;

    if (i % 20000 == 0 || i == nb_possibilites) cout << "\r  Progression : " << i*100/nb_possibilites << " %";
  }

  cout << endl;
}

/*********************************************************
 * Solver brut (approche naïve) parallélisé avc openMP
 */

void solver_brut_openmp(vector<ull> &solutions, const vector<unsigned int> &dimensions, const vector<Constraint> &constraints) {
  unsigned int longueur_solutions = accumulate(dimensions.begin()+1, dimensions.end(), dimensions[0], multiplies<int>());
  ull nb_possibilites = pow(2, longueur_solutions) - 1;

  // Génération de toutes les possibilités et nettoyage pour ne garder que les solutions (application des contraintes)
  cout << "Génération des " << nb_possibilites << " possibilités et conservation des solutions" << endl;

  // Parallélisation avec openMP
  // Parallélisation possible étant donné que les possibilités sont générées indépendament
  #pragma omp parallel for
  for (ull i = 0; i <= nb_possibilites; ++i) {
    bool *poss = new bool[longueur_solutions];
    int_to_binary(i, poss, longueur_solutions);

    // Vérification des contraintes, conserver la solution au format décimal si c'en est une
    if (valid_constraints(poss, dimensions, constraints)) {
      solutions.push_back(i);
    }

    // Rendre la mémoire de la représentation binaire de la possibilité
    delete[] poss;
    
    if (i == 0) {
      std::cout << "Nombre de thread : " << omp_get_num_threads() << std::endl;
    }

    if (omp_get_thread_num() == 0 && (i % 100 == 0 || i == nb_possibilites)) {
      double nbPossibiliteThread = (nb_possibilites/omp_get_num_threads());
      cout << "\r  Progression : " << i*100/nbPossibiliteThread << " % i = " << i;
    }
  }

  cout << endl;
}


/**********************************
 * Solveur efficace
 */

// raisonner en deux dimensions
// un premier pigeon a n emplacements possibles, vérifier l'emplacement 1, si les contraintes sont ok, alors les autres emplacements sont solution
// le second pigeon a n-1 emplacements possibles


/*******************************************************
 * Solver brut (approche naïve) parallélisé avec MPI
 */

/**
 * Calcul de toutes les possibilités et nettoyage pour ne garder que les solutions
 */
void solver_brut_mpi(vector<ull> &solutions, vector<unsigned int> &dimensions, const vector<Constraint> &constraints) {
  MPI_Reducer reducer;
  reducer.solver_brut(solutions, dimensions, constraints);
}

#endif
