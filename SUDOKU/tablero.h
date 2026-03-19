#pragma once

#include <string>
#include <fstream>

#include "celda.h"

using namespace std;
const int MAX_DIM = 9;

class tTablero {
private:
	int dim;
	tCelda mCelda[MAX_DIM][MAX_DIM];
public:
	tTablero();
	tTablero(int d,tCelda m[MAX_DIM][MAX_DIM]);
	int dame_dim();
	tCelda dame_celda(int f, int c);
	void set_celda(int f, int c, int celda);
	void set_valor(int f, int c, int v);
};

