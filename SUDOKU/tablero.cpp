
#include <string>
#include <fstream>

#include "celda.h"
#include "tablero.h"

using namespace std;

//	Constructora por defecto
tTablero::tTablero() {
	dim = MAX_DIM;
}

//	Constructora con parametros
tTablero::tTablero(int d, tCelda m[MAX_DIM][MAX_DIM]) {
	if (d > 0 && d < MAX_DIM) {
		dim = d;
	}
	else dim = MAX_DIM;

	for (int i = 0; i < dim; ++i) {
		for (int j = 0; j < dim; ++j) {
			mCelda[i][j] = m[i][j];
		}
	}
}

int  tTablero::dame_dim() 
{
	return dim;
}

tCelda tTablero::dame_celda(int f, int c) {
	return mCelda[f][c];
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

void tTablero::set_celda(int f, int c, int celda) {
	mCelda[f][c].set_valor(celda);

}