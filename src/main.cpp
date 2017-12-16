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

/**
 * Affiche sur la sortie standard les solutions d'un vecteur de solutions en fonction de la taille des dimensions données
 * Une solution est affichée sur plusieurs lignes (format matriciel) lorsque le problème est à deux dimensions (comme pigeons/cabanes)
 */
void print_solutions(const vector<ull> &solutions, const vector<unsigned int> &dimensions) {
	unsigned int solutions_length = accumulate(dimensions.begin()+1, dimensions.end(), dimensions[0], multiplies<int>());

	for (auto sol : solutions) {
		bool *sol_bin = new bool[solutions_length];
		int_to_binary(sol, sol_bin, solutions_length);
		
		if (dimensions.size() == 2) {
			for (unsigned int i = 0; i < dimensions[0]; ++i) {
				for (unsigned int j = 0; j < dimensions[1]; ++j) {
					cout << sol_bin[i*dimensions[1] + j] << " ";
				}
				cout << endl;
			}
			cout << endl;
		} else {
			for (unsigned int i = 0; i < solutions_length; ++i) {
				cout << sol_bin[i] << " ";
			}
			cout << endl;
		}

		delete[] sol_bin;
	}
}

void print_help() {
    cout << "usage: exe -o 10 -c 12\n"
        "\t-p, --pigeons, nombre d'pigeons à prendre en compte\n"
        "\t-c, --cabanes, nombre des cabanes à disposition (pigeonniers)\n"
        "\t-m, --methode brute (1), efficace (2), brute OpenMP (3), brute MPI (4)\n"
        "\t-h, --help, affiche cette aide\n" << endl;
}

int main(int argc, char* argv[]) {
	// Initialisation des variables
	unsigned int methode = 1, pigeons = 2, cabanes = 2;
	vector<unsigned int> dimensions;
	bool display_solutions = false;
	vector<ull> solutions;
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
			methode = atoi(optarg);
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

	dimensions.push_back(pigeons);
	dimensions.push_back(cabanes);

	// Application de la méthode de calcul
	switch (methode) {
		case 1: // Méthode brute
			solver_brut(solutions, dimensions);    
		break;
		case 2: // Méthode efficace
			// solver_efficace(solutions, dimensions);
		break;
		case 3: // Méthode brute parallélisée avec OpenMP
		 	solver_brut_openmp(solutions, dimensions);
		break;
		case 4: // Méthode brute parallélisée avec MPI
			solver_brut_mpi(solutions, dimensions);
		break;
		default:
		break;
	}

	// Affichagedes solutions et du nombre de solutions
	if (display_solutions) print_solutions(solutions, dimensions);
	std::cout << solutions.size() << " solutions" << std::endl;

	return EXIT_SUCCESS;
}
