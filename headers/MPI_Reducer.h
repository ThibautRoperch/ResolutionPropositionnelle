#ifndef MPI_REDUCER_H_TP
#define MPI_REDUCER_H_TP

#include <iostream>
#include <vector>
#include <numeric>
#include <functional>

#include <mpi.h>

#include "common.h"
#include "constraints.h"

class MPI_Reducer {
  private:
    int id_processus;
    int nb_processus;

  public:
    MPI_Reducer() {
      MPI::Init();
      id_processus = MPI::COMM_WORLD.Get_rank();
      nb_processus = MPI::COMM_WORLD.Get_size();
    };
    ~MPI_Reducer() { MPI::Finalize(); };

    /**
     * Le processus maître compte le nombres de matrices à générer pour chaque processus
     * Chaque processus génère ses possibilités
     * Chaque processus filtre ses possibilités avec les contraintes -> solutions
     * Les solutions réparties entre les processus sont rassemblées par le processus maître
     * Données communes : dimensions du problème
     */
    void solver_brut(std::vector<ull> &solutions, const std::vector<unsigned int> &dimensions, const std::vector<Constraint> &constraints) {
      unsigned int solutions_length = std::accumulate(dimensions.begin()+1, dimensions.end(), dimensions[0], std::multiplies<int>());
      ull nb_possibilites = pow(2, solutions_length) - 1;

      if (id_processus == 0) {
        // MASTER

        ull *tailles_locales = new ull[nb_processus]; // une taille de tableau par processus
        ull *indices_locaux = new ull[nb_processus * 2]; // indice début, indice fin pour chaque processus
        std::vector<ull> *solutions_locales = new std::vector<ull>[nb_processus * 2]; // des solutions pour chaque processus

        // Calcul du nombre de possibilités à calculer par chaque processus, avec répartition du reste si nb_possibilites%nb_processus != 0
        for (int i = 0; i < nb_processus; ++i) {
          tailles_locales[i] = nb_possibilites / (ull) nb_processus;
        }
        int modulo = nb_possibilites % nb_processus;
        int i = 0;
        while (modulo > 0) {
          tailles_locales[i++] += 1;
          modulo--;
        }

        // Calcul des indices de possibilités de chaque processus (chaque tableau doit générer les possibilités a à b, a et b étant des entiers de 0 à nb_possibilites)
        ull indice_fin_precedent = -1;
        for (int i = 0; i < nb_processus; ++i) {
          indices_locaux[i*2] = indice_fin_precedent + 1;
          indice_fin_precedent = indice_fin_precedent + tailles_locales[i];
          indices_locaux[i*2+1] = indice_fin_precedent;
        }
        indices_locaux[(nb_processus-1)*2+1]++;

        std::cout << "Génération des " << nb_possibilites << " possibilités et conservation des solutions" << std::endl;

        // Envoi des indices à chaque processus esclave
        for (int i = 1; i < nb_processus; ++i) {
          MPI::COMM_WORLD.Send(&indices_locaux[i*2], 1, MPI_UNSIGNED_LONG_LONG , i, 0);
          MPI::COMM_WORLD.Send(&indices_locaux[i*2+1], 1, MPI_UNSIGNED_LONG_LONG , i, 0);
        }

        // Calcul des sous-solutions du processus maître
        local_solver_brut(solutions_locales[0], dimensions, constraints, indices_locaux[0], indices_locaux[1]);
        // std::cout << solutions_locales[0].size() << " solution(s) calculées par le master" << std::endl;

        // Récupération des solutions de chaque processus esclave, chaque esclave envoyant ses solutions une par une
        for (int i = 1; i < nb_processus; ++i) {
          MPI::Status etat;
          int nb_solutions;

          // Récupération du nombre de solutions du processus esclave
          MPI::COMM_WORLD.Recv(&nb_solutions, 1, MPI_INT, i, MPI::ANY_TAG, etat);
          
          // Redimensionnement du vecteur de solutions receveur
          solutions_locales[i].resize(nb_solutions);

          // Récupération des solutions du processus esclave
          MPI::COMM_WORLD.Recv(&solutions_locales[i][0], nb_solutions, MPI_UNSIGNED_LONG_LONG , i, MPI::ANY_TAG, etat);

          // std::cout << nb_solutions << " solution(s) récupérée(s) du slave " << i << " par le master" << std::endl;
        }

        std::cout << "Rassemblement des solutions réparties entre les processus par Master" << std::endl;

        // Ajout des solutions de chaque processus dans le vecteur de solutions
        for (int i = 0; i < nb_processus; ++i) {
          for (auto sol : solutions_locales[i]) {
            solutions.push_back(sol);
          }
        }

        std::cout << std::endl;
      } else {
        // SLAVE

        MPI::Status etat;
        ull debut, fin;

        // Récupération des indices
        MPI::COMM_WORLD.Recv(&debut, 1, MPI_UNSIGNED_LONG_LONG , 0, MPI::ANY_TAG, etat);
        MPI::COMM_WORLD.Recv(&fin, 1, MPI_UNSIGNED_LONG_LONG , 0, MPI::ANY_TAG, etat);

        // Calcul des sous-solutions
        std::vector<ull> sous_solutions;
        local_solver_brut(sous_solutions, dimensions, constraints, debut, fin);

        // Envoi du nombre de solutions au processus maître
        int nb_solutions = sous_solutions.size();
        MPI::COMM_WORLD.Send(&nb_solutions, 1, MPI_INT, 0, id_processus);

        // Envoi des solutions au processus maître
        MPI::COMM_WORLD.Send(&sous_solutions[0], nb_solutions, MPI_UNSIGNED_LONG_LONG , 0, id_processus);

        // std::cout << nb_solutions << " solution(s) envoyée(s) du slave " << id_processus << " au master" << std::endl;
      }
    }

    void local_solver_brut(std::vector<ull> &solutions, const std::vector<unsigned int> &dimensions, const std::vector<Constraint> &constraints, ull debut, ull fin) {
      unsigned int solutions_length = std::accumulate(dimensions.begin()+1, dimensions.end(), dimensions[0], std::multiplies<int>());
      ull nb_possibilites = fin - debut;

      // Génération de toutes les possibilités et nettoyage pour ne garder que les solutions (application des contraintes)
      // std::cout << "Génération des " << nb_possibilites << " possibilités et conservation des solutions" << std::endl;

      for (ull i = debut; i <= fin; ++i) {
        bool *poss = new bool[solutions_length];
        int_to_binary(i, poss, solutions_length);

        // Vérification des contraintes, conserver la solution au format décimal si c'en est une
        if (valid_constraints(poss, dimensions, constraints)) {
          solutions.push_back(i);
        }

        // Rendre la mémoire de la représentation binaire de la possibilité
        delete[] poss;

        if (id_processus == 0) if (i % 20000 == 0 || i == nb_possibilites) std::cout << "\r  Progression générale : " << i*100/(double)nb_possibilites << " %    ";
      }

      if (id_processus == 0) std::cout << std::endl;
      std::cout << "  Fin du processus " << id_processus+1 << "/" << nb_processus << std::endl;
    }
    
};

#endif
