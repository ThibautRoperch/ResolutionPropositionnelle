#ifndef MPI_REDUCER_H_TP
#define MPI_REDUCER_H_TP

#include <iostream>
#include <vector>
#include <bitset>

#include <mpi.h>

class MPI_Reducer {
	public:
		MPI_Reducer() { MPI::Init(); };
		~MPI_Reducer() { MPI::Finalize(); };

		/**
		 * Le processus maître compte le nombres de matrices à générer pour chaque processus
		 * Chaque processus génère ses matrices -> possibilités
		 * Chaque processus filtre ses matrices avec les contraintes -> solutions
		 * Les matrices restantes sont rassemblées par le processus maître
		 * Données communes : cabanes, pigeons
		 */
		void solver_brut(std::vector<bool*> &solutions, int cabanes, int pigeons) {
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

				// Calcul des sous-solutions du processus maître
				local_solver_brut(solutions, cabanes, pigeons, indices_locaux[0], indices_locaux[1]);
				// std::cout << solutions.size() << " solution(s) calculées par le master" << std::endl;

				// Récupération des solutions de chaque processus esclave, chaque esclave envoyant ses solutions une par une
				for (int i = 1; i < nb_processus; ++i) {
					MPI::Status etat;
					int nb_solutions;

					// Récupération du nombre de solutions du processus esclave
					MPI::COMM_WORLD.Recv(&nb_solutions, 1, MPI_INT, i, MPI::ANY_TAG, etat);

					// Récupération de chaque solution du processus esclave et ajout dans le vecteur solutions
					for (int j = 0; j < nb_solutions; ++j) {
						bool *sol = new bool[pigeons*cabanes];
						MPI::COMM_WORLD.Recv(&sol[0], pigeons*cabanes, MPI_INT, i, MPI::ANY_TAG, etat);
						solutions.push_back(sol);
					}

					// std::cout << nb_solutions << " solution(s) récupérée(s) du slave " << i << " par le master" << std::endl;
				}
			} else {
				// SLAVE

				MPI::Status etat;
				int debut, fin;

				// Récupération des indices
				MPI::COMM_WORLD.Recv(&debut, 1, MPI_INT, 0, MPI::ANY_TAG, etat);
				MPI::COMM_WORLD.Recv(&fin, 1, MPI_INT, 0, MPI::ANY_TAG, etat);

				// Calcul des sous-solutions
				std::vector<bool*> sous_solutions;
				local_solver_brut(sous_solutions, cabanes, pigeons, debut, fin);

				// Envoi du nombre de solutions au processus maître
				int nb_solutions = sous_solutions.size();
				MPI::COMM_WORLD.Send(&nb_solutions, 1, MPI_INT, 0, id_processus);

				// Envoi des solutions au processus maître
				for (int i = 0; i < sous_solutions.size(); ++i) {
					MPI::COMM_WORLD.Send(&sous_solutions[i][0], pigeons*cabanes, MPI_INT, 0, id_processus);
				}

				// std::cout << nb_solutions << " solution(s) envoyée(s) du slave " << id_processus << " au master" << std::endl;
			}
		}

		void local_solver_brut(std::vector<bool*> &solutions, const int cabanes, const int pigeons, unsigned long long debut, unsigned long long fin) {
			int nb_cases = cabanes * pigeons;
			const int const_bitset_size = 9999999; // en remplacement de nb_cases pour l'initialisation du bitset

			// Génération de toutes les matrices possibles
			std::vector<bool*> possibilites;

			for (unsigned long long i = debut; i <= fin; ++i) {
				std::bitset<const_bitset_size> possibilite_bit = std::bitset<const_bitset_size>(i);
				bool *possibilite_bool = new bool[nb_cases];
				for (int j = 0; j < nb_cases; ++j) possibilite_bool[j] = (possibilite_bit[j] == 1) ? true : false;
				possibilites.push_back(possibilite_bool);
			}
			
			// Nettoyage pour ne garder que les solutions (application des contraintes)
			for (auto poss : possibilites) {
				bool continuer_analyse = true;
				// Contrainte 1 : un pigeon est dans un et un seul pigeonnier
				for (int i = 0; i < pigeons; ++i) {
					int nb_cabanes = 0;
					for (int j = 0; j < cabanes; ++j)
						nb_cabanes += (poss[i*cabanes + j]) ? 1 : 0;
					if (nb_cabanes != 1) {
						continuer_analyse = false;
						break;
					}
				}
				// Contrainte 2 : un pigeonnier accueille au plus un pigeon
				if (continuer_analyse) {
					for (int i = 0; i < cabanes; ++i) {
						int nb_pigeons = 0;
						for (int j = 0; j < pigeons; ++j)
							nb_pigeons += (poss[i + j*cabanes]) ? 1 : 0;
						if (nb_pigeons > 1) {
							continuer_analyse = false;
							break;
						}
					}
				}
				// Conserver la solution si c'en est une
				if (continuer_analyse) {
					solutions.push_back(poss);
				}
					// Rendre la mémoire sinon
					else {
					delete[] poss;
				}
			}
		}
		
};

#endif
