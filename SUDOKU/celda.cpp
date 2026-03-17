
#include <string>
#include <fstream>

#include "celda.h"

using namespace std;

tCelda tCelda::Celda() 
{

}

bool tCelda::es_vacia()
{
	if (estado == VACIA)return true;
	return false;
}

bool  tCelda::es_original()
{
	if (estado == ORIGINAL)return true;
	return false;
}

bool  tCelda::es_ocupada()
{
	if (estado == OCUPADA)return true;
	return false;
}

int  tCelda::dame_valor() 
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

