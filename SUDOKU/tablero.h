#pragma once

#include <string>
#include <fstream>

#include "celda.h"

using namespace std;
const int MAX = 9;

class tTablero {
private:
	int dim;
	tCelda mCelda[MAX][MAX];
public:
	tTablero Tablero();
	tTablero Tablero(int d,tCelda m[MAX][MAX]);
	int dame_dim();
	int dame_valor(int f, int c);
	void set_celda(int f, int c, tCelda celda);
};

