#pragma once

#include "celda.h"

const int MAX_DIM = 9;

class tTablero {
private:
	int dim;
	tCelda mCelda[MAX_DIM][MAX_DIM];
public:
	tTablero();
	tTablero(int d);
	const int dame_dim();
	tCelda dame_celda(int f, int c);
	void set_celda(int f, int c, const tCelda& celda);
};

