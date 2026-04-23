#include "celda.h"

using namespace std;

tCelda::tCelda() 
{
	valor = 0;
	estado = VACIA;
}

const bool tCelda::es_vacia()
{
	if (estado == VACIA)return true;
	return false;
}

const bool  tCelda::es_original()
{
	if (estado == ORIGINAL)return true;
	return false;
}

const bool  tCelda::es_ocupada()
{
	if (estado == OCUPADA)return true;
	return false;
}

const int  tCelda::dame_valor()
{
	return valor;
}

void  tCelda::set_valor(int v)
{
	valor = v;
}

void  tCelda::set_ocupada()
{
	estado = OCUPADA;
}

void  tCelda::set_original() 
{
	estado = ORIGINAL;
}

void  tCelda::set_vacia() 
{
	estado = VACIA;
}

