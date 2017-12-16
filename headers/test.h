#ifndef TEST_____
#define TEST_____

#include <iostream>
#include <vector>
#include <bitset>

#include <mpi.h>

class MPI_TEST {
	public:
		MPI_TEST() { MPI::Init(); };
		~MPI_TEST() { MPI::Finalize(); };

		/**
		 * Le processus maître compte le nombres de matrices à générer pour chaque processus
		 * Chaque processus génère ses matrices -> possibilités
		 * Chaque processus filtre ses matrices avec les contraintes -> solutions
		 * Les matrices restantes sont rassemblées par le processus maître
		 * Données communes : cabanes, pigeons
		 */
		void solver_brut(std::vector<std::vector<bool>> &solutions, int cabanes, int pigeons) {
			int id_processus = MPI::COMM_WORLD.Get_rank();
			int nb_processus = MPI::COMM_WORLD.Get_size();

			unsigned long long nb_possibilites = pow(2, cabanes*pigeons) - 1;
			int nb_cases = cabanes * pigeons;

			if (id_processus == 0) {
				// MASTER

				int *tailles_locales = new int[nb_processus]; // une taille de tableau par processus
				int *indices_locaux = new int[nb_processus * 2]; // indice début, indice fin pour chaque processus

				// Calcul du nombre de possibilités à calculer par chaque processus, avec répartition du reste si nb_possibilites%nb_processus != 0
				for (int i = 0; i < nb_processus; ++i) {
					tailles_locales[i] = nb_possibilites / nb_processus;
				}
				int modulo = nb_possibilites % nb_processus;
				int i = 0;
				while (modulo > 0) {
					tailles_locales[i++] += 1;
					modulo--;
				}

				// Calcul des indices de possibilités de chaque processus (chaque tableau doit générer les matrices des possibilités a à b, a et b étant des entiers de 0 à nb_possibilites)
				int indice_fin_precedent = -1;
				for (int i = 0; i < nb_processus; ++i) {
					indices_locaux[i*2] = indice_fin_precedent + 1;
					indice_fin_precedent = indice_fin_precedent + tailles_locales[i];
					indices_locaux[i*2+1] = indice_fin_precedent;
				}
				indices_locaux[(nb_processus-1)*2+1]++;

				// Envoi des indices à chaque processus esclave
				for (int i = 1; i < nb_processus; ++i) {
					MPI::COMM_WORLD.Send(&indices_locaux[i*2], 1, MPI_INT, i, 0);
					MPI::COMM_WORLD.Send(&indices_locaux[i*2+1], 1, MPI_INT, i, 0);
				}

				std::vector<bool> sol(pigeons*cabanes);
				// Récupération et ajout des solutions de chaque processus esclave, chaque esclave envoyant ses solutions une par une
				for (int i = 1; i < nb_processus; ++i) {
					MPI::Status etat;
					// MPI::COMM_WORLD.Recv(&sol[0], pigeons*cabanes, MPI_INT, i, MPI::ANY_TAG, etat);
				}
				solutions.push_back(sol);
			} else {
				// SLAVE

				MPI::Status etat;
				int debut, fin;

				// Récupération des indices
				MPI::COMM_WORLD.Recv(&debut, 1, MPI_INT, 0, MPI::ANY_TAG, etat);
				MPI::COMM_WORLD.Recv(&fin, 1, MPI_INT, 0, MPI::ANY_TAG, etat);

				// Calcul des sous-solutions
				std::vector<std::vector<bool>> sous_solutions;
				local_solver_brut(sous_solutions, cabanes, pigeons, debut, fin);
				
				// Envoi des solutions au processus maître
				// MPI::COMM_WORLD.Send(&sous_solutions[0][0], pigeons*cabanes, MPI_INT, 0, id_processus);
				std::vector<bool> sol = sous_solutions[0];
				// MPI::COMM_WORLD.Send(&sol[0], pigeons*cabanes, MPI_INT, 0, id_processus);
			}
		}

		void local_solver_brut(std::vector<std::vector<bool>> &solutions, const int cabanes, const int pigeons, unsigned long long debut, unsigned long long fin) {
			std::vector<bool> sol(pigeons*cabanes);
			for (int i = 0; i < pigeons*cabanes; ++i) {
				sol[i] = 1;
			}
			solutions.push_back(sol);
		}
		
};

#endif
