#include "tablero.h"

using namespace std;

//	Constructora por defecto
tTablero::tTablero() {
	dim = MAX_DIM;
}

//	Constructora con parametros
tTablero::tTablero(int d) {
	if (d >= 0 && d <= MAX_DIM) {
		dim = d;
	}
	else dim = MAX_DIM;
}

const int  tTablero::dame_dim()
{
	return dim;
}

tCelda tTablero:: dame_celda(int f, int c) {
	return mCelda[f][c];
}


void tTablero::set_celda(int f, int c, const tCelda& celda) {
	if (f >= 0 && f < dim && c >= 0 && c < dim) {
		mCelda[f][c] = celda;
	}
}
