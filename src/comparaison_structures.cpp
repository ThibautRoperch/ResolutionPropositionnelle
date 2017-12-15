#include <string>
#include <vector>
#include <bitset>
#include <iostream>
#include <cmath>

using namespace std;

/*
g++ -std=c++11 src/comparaison_structures.cpp -o bin/comparaison_structures.out

Tests avec taille = 20 :

time ./bin/comparaison_structures.out 1
0.730s
time ./bin/comparaison_structures.out 2
0.133s
time ./bin/comparaison_structures.out 3
28.072s
time ./bin/comparaison_structures.out 4
30.525s
*/

void simple_vector(int taille, long nombre, vector<bool> &res) {
	unsigned int i = 0;
	while (nombre != 0) {
		if (nombre % 2)
			res[i] = true;
		else
			res[i] = false;
		nombre /= 2;
		++i;
	}
}

void simple_array(int taille, long nombre, bool *res) {
	unsigned int i = 0;
	while (nombre != 0) {
		if (nombre % 2)
			res[i] = true;
		else
			res[i] = false;
		nombre /= 2;
		++i;
	}
}

void bitset_vector(int taille, long nombre, vector<bool> &res) {
	bitset<10000000> nombre_bits = bitset<10000000>(nombre);
	for (int j = 0; j < taille; ++j) res[j] = (nombre_bits[j] == 1) ? true : false;
}

void bitset_array(int taille, long nombre, bool *res) {
	bitset<10000000> nombre_bits = bitset<10000000>(nombre);
	for (int j = 0; j < taille; ++j) res[j] = (nombre_bits[j] == 1) ? true : false;
}

void print_vector(const vector<bool> &res) {
	for (auto bit : res) {
		cout << bit << " ";
	}
	cout << endl;
}

void print_array(bool *res, int taille) {
	for (int i = 0; i < taille; ++i) {
		cout << res[i] << " ";
	}
	cout << endl;
}


int main(int argc, char *argv[]) {
	int taille = 20;
	long nombre_max = pow(2, taille);

	int methode = atoi(argv[1]);

	for (long nombre = nombre; nombre < nombre_max; ++nombre) {
		switch (methode) {
			case 1:
			{
				vector<bool> res(taille);
				simple_vector(taille, nombre, res);
				// print_vector(res);
			}
			break;
			case 2:
			{
				bool *res = new bool[taille];
				simple_array(taille, nombre, res);
				// print_array(res, taille);
			}
			break;
			case 3:
			{
				vector<bool> res(taille);
				bitset_vector(taille, nombre, res);
				// print_vector(res);
			}
			break;
			case 4:
			{
				bool *res = new bool[taille];
				bitset_array(taille, nombre, res);
				// print_array(res, taille);
			}
			break;
		}
	}

	return 0;
}
