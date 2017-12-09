#ifndef MPI_REDUCER_H_TP
#define MPI_REDUCER_H_TP

#include <iostream>
#include <typeinfo>
#include <numeric>
#include <iomanip>

#include <mpi.h>

class MPI_Reducer {
    private:
        MPI::Datatype type;

    public:
        MPI_Reducer(int argc, char* argv[]) { MPI::Init(argc, argv); };
        ~MPI_Reducer() { MPI::Finalize(); };

        void oddEvenSort(int tab[], int size) {
            int id_processus = MPI::COMM_WORLD.Get_rank();
            int nb_processus = MPI::COMM_WORLD.Get_size();

            if (id_processus == 0) {
                // MASTER

                int *tailles_locales = new int[nb_processus]; // une taille de tableau par processus
                int **tableaux_locaux = new int*[nb_processus]; // un sous-tableau par processus

                // Calcul des tailles des sous-tableaux, avec répartition du reste si size%nb_processus != 0
                for (int i = 0; i < nb_processus; ++i) {
                    tailles_locales[i] = size / nb_processus;
                }
                int modulo = size % nb_processus;
                int i = 0;
                while (modulo > 0) {
                    tailles_locales[i++] += 1;
                    modulo--;
                }

                // Attribution des valeurs aux sous-tableaux
                int tailles_cumulees = 0;
                for (int i = 0; i < nb_processus; ++i) {
                    int *sous_tab = new int[tailles_locales[i]];
                    for (int j = tailles_cumulees, h = 0; j < tailles_cumulees + tailles_locales[i]; ++j, ++h) {
                        sous_tab[h] = tab[j];
                    }
                    tailles_cumulees += tailles_locales[i];
                    tableaux_locaux[i] = sous_tab;
                }

                // Envoi des sous-tailles et sous-tableaux à chaque slave
                for (int i = 1; i < nb_processus; ++i) {
                    MPI::COMM_WORLD.Send(&tailles_locales[i], 1, MPI_INT, i, 0);
                    MPI::COMM_WORLD.Send(&tableaux_locaux[i][0], tailles_locales[i], MPI_INT, i, 0);
                }

                // Calcul du sous-tableau du master
                // std::cout << "Processus " << 0 << " : ";
                // displayArray(tableaux_locaux[0], tailles_locales[0]);
                localOddEvenSort(tableaux_locaux[0], tailles_locales[0]);
                // std::cout << " => ";
                // displayArray(tableaux_locaux[0], tailles_locales[0]);
                // std::cout << std::endl;

                // Récupération des sous-tableaux de chaque slave
                for (int i = 1; i < nb_processus; ++i) {
                    MPI::Status etat;
                    MPI::COMM_WORLD.Recv(&tableaux_locaux[i][0], tailles_locales[i], MPI_INT, i, MPI::ANY_TAG, etat);
                }

                // for (int i = 0; i < nb_processus; ++i) {
                //     displayArray(tableaux_locaux[i], tailles_locales[i]);
                //     std::cout << std::endl;
                // }

                // Merge des tableaux
                int *parcours_locaux = new int[nb_processus]; // une taille de tableau par processus
                for (int i = 0; i < nb_processus; ++i) parcours_locaux[i] = 0;
                for (int i = 0; i < size; ++i) {
                    int min;
                    int proc_qui_a_le_min;
                    // Recherche du min dans tous les tableaux
                    for (int j = 0; j < nb_processus; ++j) {
                        if (parcours_locaux[j] < tailles_locales[j] && (j == 0 || tableaux_locaux[j][parcours_locaux[j]] < min)) {
                            min = tableaux_locaux[j][parcours_locaux[j]];
                            proc_qui_a_le_min = j;
                        }
                    }
                    parcours_locaux[proc_qui_a_le_min]++;
                    tab[i] = min;
                }

                // displayArray(tab, size);
            } else {
                // SLAVE

                int taille_locale;
                MPI::Status etat;
                MPI::COMM_WORLD.Recv(&taille_locale, 1, MPI_INT, 0, MPI::ANY_TAG, etat);
                int *tab_local = new int[taille_locale];
                MPI::COMM_WORLD.Recv(&tab_local[0], taille_locale, MPI_INT, 0, MPI::ANY_TAG, etat);
                
                // std::cout << "Processus " << id_processus << " : ";
                // displayArray(tab_local, taille_locale);
                localOddEvenSort(tab_local, taille_locale);
                // std::cout << " => ";
                // displayArray(tab_local, taille_locale);
                // std::cout << std::endl;

                MPI::COMM_WORLD.Send(&tab_local[0], taille_locale, MPI_INT, 0, nb_processus);
            }
        }

        void displayArray(int tab[], int size) {
            for (int i = 0; i < size; ++i) {
                std::cout << tab[i] << " ";
            }
            // std::cout << std::endl;
        }

        void localOddEvenSort(int tab[], int size) {
            // tells whether array is sorted or not
            bool is_sorted = false;
            while (!is_sorted) {
                is_sorted = true;
                // even index sort
                for (int i = 0; i < size; i += 2) {
                    if (tab[i] > tab[i+1] && i+1 < size) {
                        int tmp = tab[i];
                        tab[i] = tab[i+1];
                        tab[i+1] = tmp;
                        is_sorted = false;
                    }
                }
                // odd index sort
                for (int i = 1; i < size - 1; i += 2) {
                    if (tab[i] > tab[i+1]) {
                        int tmp = tab[i];
                        tab[i] = tab[i+1];
                        tab[i+1] = tmp;
                        is_sorted = false;
                    }
                }
            }
        }
        
};

#endif
