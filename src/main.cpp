#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <vector>
#include <bitset>
#include <cmath>
#include <omp.h>

#include <mpi.h>

using namespace std;

void print_solutions(const vector<bool*> &solutions, const int cabanes, const int pigeons) {
  for (auto sol : solutions) {
    for (int i = 0; i < pigeons; ++i) {
      for (int j = 0; j < cabanes; ++j) {
          cout << sol[i*cabanes + j] << " ";
      }
      cout << endl;
    }
    cout << endl;
  }
  cout << solutions.size() << " solutions\n" << endl;
}

bool test_solution_final(bool* solution, const int cabanes, const int pigeons) {
  // Contrainte 1 : un pigeon est dans un et un seul pigeonnier
  for (int i = 0; i < pigeons; ++i) {
    int nb_cabanes = 0;
    for (int j = 0; j < cabanes; ++j)
      nb_cabanes += (solution[i*cabanes + j]) ? 1 : 0;
    if (nb_cabanes != 1) {
      return false;
    }
  }
  // Contrainte 2 : un pigeonnier accueille au plus un pigeon
  for (int i = 0; i < cabanes; ++i) {
    int nb_pigeons = 0;
    for (int j = 0; j < pigeons; ++j)
      nb_pigeons += (solution[i + j*cabanes]) ? 1 : 0;
    if (nb_pigeons > 1) {
      return false;
    }
  }
  return true;
}

/**
 * Calcul de toutes les matrices possibles, puis nettoyage pour ne garder que les solutions
 */
void solver_brut(vector<bool*> &solutions, const int cabanes, const int pigeons) {
  unsigned long long nb_possibilites = pow(2, cabanes*pigeons) - 1;
  int nb_cases = cabanes * pigeons;
  const int const_bitset_size = 64; // en remplacement de nb_cases pour l'initialisation du bitset

  // Génération de toutes les matrices possibles
  cout << "Génération des " << nb_possibilites << " matrices" << endl;
  vector<bool*> possibilites;

  for (unsigned long long i = 0; i <= nb_possibilites; ++i) {
    bitset<const_bitset_size> possibilite_bit = bitset<const_bitset_size>(i);
    bool *possibilite_bool = new bool[nb_cases];
    for (int j = 0; j < nb_cases; ++j) possibilite_bool[j] = (possibilite_bit[j] == 1) ? true : false;
    possibilites.push_back(possibilite_bool);
    if (i % 100 == 0 || i == nb_possibilites) cout << "\r  " << i*100/nb_possibilites << " %";
  }
  cout << endl;

  // Nettoyage pour ne garder que les solutions (application des contraintes)
  cout << "Application des contraintes sur les matrices pour ne garder que les solutions" << endl;
  unsigned int i = 0;
  while (possibilites.size() > 0) {
    bool* sol = possibilites.back();
    possibilites.pop_back();
    // Vérification des contraintes,conserver la solution si c'en est une
    if (test_solution_final(sol,cabanes,pigeons)) {
      solutions.push_back(sol);
    }
    // Rendre la mémoire sinon
    else {
      delete[] sol;
    }
    if (i % 100 == 0 || possibilites.size() == 0) cout << "\r  " << i*100/nb_possibilites << " %";
    ++i;
  }
  cout << endl;
}

void solver_brut_pragma(vector<bool*> &solutions, const int cabanes, const int pigeons) {
  unsigned long long nb_possibilites = pow(2, cabanes*pigeons) - 1;
  int nb_cases = cabanes * pigeons;
  const int const_bitset_size = 64; // en remplacement de nb_cases pour l'initialisation du bitset

  // Génération de toutes les matrices possibles
  cout << "Génération des " << nb_possibilites << " matrices" << endl;
  vector<bool*> possibilites;

  #pragma omp parallel for
  for (unsigned long long i = 0; i <= nb_possibilites; ++i) {
    bitset<const_bitset_size> possibilite_bit = bitset<const_bitset_size>(i);    
    bool *possibilite_bool = new bool[nb_cases];
    for (int j = 0; j < nb_cases; ++j) possibilite_bool[j] = (possibilite_bit[j] == 1) ? true : false;
    #pragma omp critical
    {
      possibilites.push_back(possibilite_bool);
    }
    if (i % 100 == 0 || i == nb_possibilites) cout << "\r  " << i*100/nb_possibilites << " %";
  }
  cout << endl;

  // Nettoyage pour ne garder que les solutions (application des contraints)
  cout << "Application des contraintes sur les matrices pour ne garder que les solutions" << endl;
  unsigned int i = 0;
  while (possibilites.size() > 0) {
    bool* sol = possibilites.back();
    possibilites.pop_back();
    // Vérification des contraintes,conserver la solution si c'en est une
    if (test_solution_final(sol,cabanes,pigeons)) {
      solutions.push_back(sol);
    }
    // Rendre la mémoire sinon
    else {
      delete[] sol;
    }
    if (i % 100 == 0 || possibilites.size() == 0) cout << "\r  " << i*100/nb_possibilites << " %";
    ++i;
  }
  cout << endl;
}

void local_solver_brut(vector<bool*> &solutions, const int cabanes, const int pigeons, unsigned long long debut, unsigned long long fin) {
  int nb_cases = cabanes * pigeons;
  const int const_bitset_size = 9999999; // en remplacement de nb_cases pour l'initialisation du bitset

  // Génération de toutes les matrices possibles
  vector<bool*> possibilites;

  for (unsigned long long i = debut; i <= fin; ++i) {
    bitset<const_bitset_size> possibilite_bit = bitset<const_bitset_size>(i);
    bool *possibilite_bool = new bool[nb_cases];
    for (int j = 0; j < nb_cases; ++j) possibilite_bool[j] = (possibilite_bit[j] == 1) ? true : false;
    possibilites.push_back(possibilite_bool);
  }
  
  // Nettoyage pour ne garder que les solutions (application des contraintes)
  for (unsigned long long p = 0; p < possibilites.size(); ++p) {
    bool* poss = possibilites[p];
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

void solver_brut_mpi(vector<bool*> &solutions, const int cabanes, const int pigeons) {
  // Le processus maître compte le nombres de matrices à générer pour chaque processus
  // Chaque processus génère ses matrices -> possibilités
  // Chaque processus filtre ses matrices avec les contraintes -> solutions
  // Les matrices restantes sont rassemblées par le processus maître
  // Données communes : cabanes, pigeons

  MPI::Init();

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
    // cout << solutions.size() << " solution(s) calculées par le master" << endl;

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

      // cout << nb_solutions << " solution(s) récupérée(s) du slave " << i << " par le master" << endl;
    }
  } else {
    // SLAVE

    MPI::Status etat;
    int debut, fin;

    // Récupération des indices
    MPI::COMM_WORLD.Recv(&debut, 1, MPI_INT, 0, MPI::ANY_TAG, etat);
    MPI::COMM_WORLD.Recv(&fin, 1, MPI_INT, 0, MPI::ANY_TAG, etat);

    // Calcul des sous-solutions
    vector<bool*> sous_solutions;
    local_solver_brut(sous_solutions, cabanes, pigeons, debut, fin);

    // Envoi du nombre de solutions au processus maître
    int nb_solutions = sous_solutions.size();
    MPI::COMM_WORLD.Send(&nb_solutions, 1, MPI_INT, 0, id_processus);

    // Envoi des solutions au processus maître
    for (int i = 0; i < sous_solutions.size(); ++i) {
      MPI::COMM_WORLD.Send(&sous_solutions[i][0], pigeons*cabanes, MPI_INT, 0, id_processus);
    }

    // cout << nb_solutions << " solution(s) envoyée(s) du slave " << id_processus << " au master" << endl;
  }

  MPI::Finalize();
}

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

void print_help() {
    cout << "usage: exe -o 10 -c 12\n"
        "\t-p, --pigeons, nombre d'pigeons à prendre en compte\n"
        "\t-c, --cabanes, nombre des cabanes à dispotion\n"
        "\t-h, --help, affiche cette aide\n" << endl;
}

int main(int argc, char* argv[]) {
  // Initialisation des variables
  int methode = 1, pigeons = 2, cabanes = 2;
  vector<bool*> solutions; // une matrice est dans un seul tableau, lignes (pigeons) à la suite : [p1c1, p1c2, p1c3, p2c1, p2c2, p2c3, p3c1, p3c2, p3c3]
  int opt, indice;

  // Traitement des options
  struct option long_option[] =
  {
    {"pigeons", required_argument, NULL, 'p'},
    {"cabanes", required_argument, NULL, 'c'},
    {"methode", required_argument, NULL, 'm'},
    {"help", no_argument, NULL, 'h'},
    {0,0,0,0}
  };
  while ((opt = getopt_long(argc, argv, "hp:c:m:", long_option, &indice)) != -1) {
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
      case 'h':
        print_help();
        break;
      default:
        print_help();
        exit(EXIT_FAILURE);
    }
  }

  switch (methode) {
    case 1: // Méthode brute
      solver_brut(solutions, cabanes, pigeons);    
      break;
    case 2: // Méthode efficace
      // solver_efficace(solutions, cabanes, pigeons);
      break;
    case 3: // Méthode brute parallélisée avec pragma
      solver_brut_pragma(solutions, cabanes, pigeons);
      break;
    case 4: // Méthode brute parallélisée avec MPI
      solver_brut_mpi(solutions, cabanes, pigeons);
      break;
    default:
      break;
  }

  std::cout << "Nombre de solutions : " << solutions.size() << std::endl;
  //print_solutions(solutions, cabanes, pigeons);

  // Rendre la mémoire des solutions
  for (auto sol : solutions) delete[] sol;

  return EXIT_SUCCESS;
}
