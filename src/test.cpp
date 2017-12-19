#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "../headers/constraints.h"

using namespace std;

int main(int argc, char *argv[]) {

    vector<string> variables;
    vector<unsigned int> dimensions;
    vector<Constraint> constraints;
    // et les autres trucs du programme

	ifstream fichier(argv[1]);

	if (fichier.is_open()) {
	    string ligne;
		while (getline(fichier, ligne)) {
            int indice = 0;

            // Esquive des espaces
            while (ligne.length() > 0 && ligne[indice] == ' ') {
                ++indice;
            }

            // Traitement des mots-clé
            if (ligne.length() > 0 && ligne[indice] != '#') {
                if (ligne[indice] == 'v') {
                    indice += 2;
                    int etape = 1;
                    string variable;
                    string valeur;
                    for (unsigned int i = indice; i < ligne.length(); ++i) {
                        if (ligne[i] == '#') break;
                        if (ligne[i] == ' ' && i > 0 && ligne[i-1] != ' ') etape++;
                        else {
                            if (etape == 1) variable += ligne[i];
                            if (etape == 2) valeur += ligne[i];
                        }
                    }
                    variables.push_back(variable);
                    dimensions.push_back(stoi(valeur));
                } else if (ligne[indice] == 'c') {
                    indice += 2;
                    int etape = 1;
                    bool signe = true;
                    string min;
                    string max;
                    string variable;
                    for (unsigned int i = indice; i < ligne.length(); ++i) {
                        if (ligne[i] == '#') break;
                        if (ligne[i] == ' ' && i > 0 && ligne[i-1] != ' ') etape++;
                        else {
                            if (etape == 2 && signe == true && min.compare("neg") == 0) {
                                    signe = false;
                                    min = "";
                                    etape = 1;
                            }
                            if (etape == 1) min += ligne[i];
                            if (etape == 2) max += ligne[i];
                            if (etape == 3) variable += ligne[i];
                        }
                    }
                    cout << min << "||" << max << endl;
                    ptrdiff_t indice_contrainte = find(variables.begin(), variables.end(), variable) - variables.begin();
                    constraints.push_back(Constraint(signe, stoi(min), stoi(max), indice_contrainte, variable));
                }
            } // Fin "si la ligne est non vide et ne commence pas par #"
        }
		fichier.close();

        constraints[0].afficher();
        constraints[1].afficher();
	} else {
		cout << "Impossible d'ouvrir le fichier " << argv[1] << endl;
		return EXIT_FAILURE;
	}

    return 0;
}