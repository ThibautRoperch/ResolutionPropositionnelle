#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <vector>
#include <bitset>
#include <cmath>

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

void solver_brut(vector<bool*> &solutions, const int cabanes, const int pigeons) {
  unsigned long long nb_possibilites = pow(2, cabanes*pigeons) - 1;
  int nb_cases = cabanes * pigeons;
  const int const_bitset_size = 9999999; // en remplacement de nb_cases pour l'initialisation du bitset

  // Génération de toutes les matrices possibles
  cout << "Génération des " << nb_possibilites << " matrices" << endl;
  vector<bool*> possibilites;

  bool *possibilite_bool;
  for (unsigned int i = 0; i <= nb_possibilites; ++i) {
    bitset<const_bitset_size> possibilite_bit = bitset<const_bitset_size>(i);
    possibilite_bool = new bool[nb_cases];
    for (int j = 0; j < nb_cases; ++j) possibilite_bool[j] = (possibilite_bit[j] == 1) ? true : false;
    possibilites.push_back(possibilite_bool);
    if (i % 100 == 0 || i == nb_possibilites) cout << "\r  " << i*100/nb_possibilites << " %";
  }
  cout << endl;

  // Nettoyage pour ne garder que les solutions (application des contraints)
  cout << "Application des contraintes sur les matrices pour ne garder que les solutions" << endl;
  unsigned int i = 0;
  while (possibilites.size() > 0) {
    bool* sol = possibilites.back();
    possibilites.pop_back();
    bool continuer_analyse = true;
    // Contrainte 1 : un pigeon est dans un et un seul pigeonnier
    for (int i = 0; i < pigeons; ++i) {
      int nb_cabanes = 0;
      for (int j = 0; j < cabanes; ++j)
        nb_cabanes += (sol[i*cabanes + j]) ? 1 : 0;
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
          nb_pigeons += (sol[i + j*cabanes]) ? 1 : 0;
        if (nb_pigeons > 1) {
          continuer_analyse = false;
          break;
        }
      }
    }
    // Conserver la solution si c'en est une
    if (continuer_analyse) {
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

void solver_efficace(vector<bool*> &solutions, const int cabanes, const int pigeons) {

}

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

  // Méthode brute
  if (methode == 1) {
    // Calcul de toutes les matrices possibles, et nettoyage des solutions
    solver_brut(solutions, cabanes, pigeons);
  }
  // Méthode efficace
  else if (methode == 2) {
    // Calcul des solutions
    solver_efficace(solutions, cabanes, pigeons);
  }

  // print_solutions(solutions, cabanes, pigeons);

  for (auto sol : solutions) delete[] sol;

  return EXIT_SUCCESS;
}
