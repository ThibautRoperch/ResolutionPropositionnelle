#ifndef METHODS_H
#define METHODS_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <numeric>
#include <algorithm>
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

void solver_brut_openMP(vector<ull> &solutions, const vector<unsigned int> &dimensions, const vector<Constraint> &constraints) {
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

/**********************************
 * Solveur efficace (approche récursive)
 */

vector<bool*> solver_efficace(bool* tab, unsigned int i, unsigned int solutions_length, const vector<unsigned int> &dimensions, const vector<Constraint> &constraints) {
  vector<bool*> solutions;
  vector<bool*> solutions_enfants;

  // Si on est à la fin, on stock la solution
  if (i == solutions_length) {
    if (valid_constraints(tab, dimensions, constraints)) {
      solutions.push_back(tab);
    }
    return solutions;
  }
  
  for (int j = 0; j <= 1; ++j) {
    tab[i] += j;
    // On test les contraintes en cours de construction, si la solution est possible, 
    // on passe à la case suivant sinon on ne fait rien et évite ainsi le parcours inutile de la branche
    if (partially_valid_constraints(tab, dimensions, constraints)) {
      bool* newTab = new bool[solutions_length];
      for (unsigned int k=0; k < solutions_length; ++k) {
        newTab[k] = tab[k];
      }
      solutions_enfants = solver_efficace(newTab, i+1, solutions_length, dimensions, constraints);
      solutions.insert(solutions.end(), solutions_enfants.begin(), solutions_enfants.end());
    }
    tab[i] -= j;
  }

  return solutions;
}

/*******************************************************
 * Solver efficace (approche récursive) parallélisé avec OpenMP
 */

vector<bool*> solver_efficace_openMP(bool* tab, unsigned int i, unsigned int solutions_length, const vector<unsigned int> &dimensions, const vector<Constraint> &constraints) {
  vector<bool*> solutions;
  vector<bool*> solutions_enfants;

  // Si on est à la fin, on stock la solution
  if (i == solutions_length) {
    if (valid_constraints(tab, dimensions, constraints)) {
      solutions.push_back(tab);
    }
    return solutions;
  }

  for (int j = 0; j <= 1; ++j) {
    tab[i] += j;
    // On test les contraintes en cours de construction, si la solution est possible, 
    // on passe à la case suivant sinon on ne fait rien et évite ainsi le parcours inutile de la branche
    if (partially_valid_constraints(tab, dimensions, constraints)) {
      bool* newTab = new bool[solutions_length];
      #pragma omp parallel for
      for (unsigned int k=0; k < solutions_length; ++k) {
        newTab[k] = tab[k];
      }
      solutions_enfants = solver_efficace_openMP(newTab, i+1, solutions_length, dimensions, constraints);
      solutions.insert(solutions.end(), solutions_enfants.begin(), solutions_enfants.end());
    }
    tab[i] -= j;
  }

  return solutions;
}

#endif
