#ifndef COMMON_H
#define COMMON_H

#include <cstdlib>

/**********************************
 * Fonctions communes aux méthodes
 */

/**
 * Écrit dans le tableau de booléen donné en paramètre la représentation binaire d'un nombre décimal
 */
void int_to_binary(ulong nombre, bool *res, unsigned int longueur) {
	unsigned int i = 0;
	while (nombre != 0 || i < longueur) {
		if (nombre % 2)
			res[i] = true;
		else
			res[i] = false;
		nombre /= 2;

		++i;
	}
}

#endif
