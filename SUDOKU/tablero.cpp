
#include <string>
#include <fstream>

#include "celda.h"
#include "tablero.h"

using namespace std;

//	Constructora por defecto
tTablero tTablero::Tablero() {
	dim = 9;
}

//	Constructora con parametros
tTablero tTablero::Tablero(int d, tCelda m[MAX][MAX]) {
	dim = d;
	for (int i = 0; i < MAX; ++i) {
		for (int j = 0; j < MAX; ++j) {
			mCelda[i][j] = m[i][j];
		}
	}
}

int  tTablero::dame_dim() 
{
	return dim;
}

int tTablero::dame_valor(int f, int c) {
	return mCelda[f][c].dame_valor();
}

void tTablero::set_celda(int f, int c, tCelda celda) {
	mCelda[f][c].set_valor(celda.dame_valor());

	if (celda.es_vacia()) {
		mCelda[f][c].set_vacia();
	}

	if (celda.es_original()) {
		mCelda[f][c].set_original();
	}

	if (celda.es_ocupada()) {
		mCelda[f][c].set_ocupada();
	}
}