#include <iostream>
#include <cstdlib>
#include <vector>
#include <numeric>
#include <functional>
#include <cmath>
#include <getopt.h>
#include <omp.h>

#include "../headers/methods.h"

using namespace std;

void print_help() {
  cout << "usage: exe -o 10 -c 12\n"
    "\t-p, --pigeons, nombre de pigeons à prendre en compte\n"
    "\t-c, --cabanes, nombre de cabanes à disposition (pigeonniers)\n"
    "\t-m, --methode brute (1), efficace (2), brute OpenMP (3), brute MPI (4)\n"
    "\t-h, --help, affiche cette aide\n" << endl;
}

int main(int argc, char* argv[]) {
  // Initialisation des variables
  unsigned int method = 1, pigeons = 2, cabanes = 2;
  vector<string> variables;
  vector<unsigned int> dimensions;
  vector<Constraint> constraints;
  bool display_solutions = false;
  vector<ull> solutions;
	vector<bool*> solutions_recursif;
  int opt, indice;

  // Traitement des options
  struct option long_option[] =
  {
    {"pigeons", required_argument, NULL, 'p'},
    {"cabanes", required_argument, NULL, 'c'},
    {"method", required_argument, NULL, 'm'},
    {"display solutions", no_argument, NULL, 'd'},
    {"help", no_argument, NULL, 'h'},
    {0,0,0,0}
  };

  while ((opt = getopt_long(argc, argv, "hp:c:m:d", long_option, &indice)) != -1) {
    switch (opt) {
    case 'p':
      pigeons = atoi(optarg);
      break;
    case 'c':
      cabanes = atoi(optarg);
      break;
    case 'm':
      method = atoi(optarg);
      break;
    case 'd':
      display_solutions = true;
      break;
    case 'h':
      print_help();
      break;
    default:
      print_help();
      exit(EXIT_FAILURE);
    }
  }

  // Création et affichage des contraintes
  variables.push_back("pigeons");
  variables.push_back("pigeonniers");
  dimensions.push_back(pigeons);
  dimensions.push_back(cabanes);
  constraints.push_back(Constraint(1, 1, 1, 0));
  constraints.push_back(Constraint(1, 0, 1, 1));
  for (auto constraint : constraints) constraint.print(variables, dimensions);
  
  // Application de la méthode de calcul
  switch (method) {
    case 1: // Méthode brute
      solver_brut(solutions, dimensions, constraints);    
      break;
		case 2: // Méthode efficace
			{
				int solutions_length = accumulate(dimensions.begin()+1, dimensions.end(), dimensions[0], multiplies<int>());
				bool* matriceVide = new bool[solutions_length];	
				solutions_recursif = solver_efficace(matriceVide, 0, solutions_length, dimensions, constraints);
			}
      break;
    case 3: // Méthode brute parallélisée avec OpenMP
      solver_brut_openMP(solutions, dimensions, constraints);
      break;
    case 4: // Méthode brute parallélisée avec MPI
      solver_brut_mpi(solutions, dimensions, constraints);
      break;
		case 5: // Méthode efficace parallélisée avec OpenMP
			{
				int solutions_length = accumulate(dimensions.begin()+1, dimensions.end(), dimensions[0], multiplies<int>());
				bool* matriceVide = new bool[solutions_length];	
				solutions_recursif = solver_efficace_openMP(matriceVide, 0, solutions_length, dimensions, constraints);
			}		
		  break;
    default:
      break;
  }

	// Affichage des solutions et du nombre de solutions
	if (method == 2 || method == 5) {		
		if (display_solutions) print_solutions_recursif(solutions_recursif, dimensions);
    if (solutions.size() == 0) cout << "INSATISFIABLE" << endl;
    else cout << "SATISFIABLE" << endl;
		cout << solutions_recursif.size() << " solutions" << endl;
	} else {
		if (display_solutions) print_solutions(solutions, dimensions);
    if (solutions.size() == 0) cout << "INSATISFIABLE" << endl;
    else cout << "SATISFIABLE" << endl;
		cout << solutions.size() << " solutions" << endl;
	}

  return EXIT_SUCCESS;
}
