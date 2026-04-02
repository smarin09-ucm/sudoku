#include "reglasTablero.h"
#include <fstream>
#include <iostream>
using namespace std;

tReglas::tReglas() 
{
    cont = 0;
    lista.contBloq = 0;
}

//devuelve la dimension
int tReglas::dame_dimension() 
{
    return tablero.dame_dim();
}

//devuelve la celda
tCelda tReglas::dame_celda(int f, int c) 
{
    return tablero.dame_celda(f, c);
}

//devuelve true si esta terminado
bool tReglas::terminado() 
{
    int dim = tablero.dame_dim();
    return (cont == dim * dim) && !bloqueo();
}

//devuelve true si hay algún bloqueo
bool tReglas::bloqueo() 
{
    return lista.contBloq > 0;
}

//devuelve el nuemro de celdas bloqueadas
int tReglas::dame_num_celdas_bloqueadas() 
{
    return lista.contBloq;
}

//devuelve el numero de celdas vacias
int tReglas::dame_num_celdas_vacias() 
{
    int dim = tablero.dame_dim();
    return dim * dim - cont;
}

//devuelve la celda bloqueada
tCelda tReglas::dame_celda_bloqueada(int p, int& f, int& c) 
{
    if (p >= 0 && p < lista.contBloq) 
    {
        f = lista.lista[p].f;
        c = lista.lista[p].c;
    }
    return tablero.dame_celda(f, c);
}

//devuelve true si el valor se puede colocar
bool tReglas::es_valor_posible(int f, int c, int v) 
{
    int dim = tablero.dame_dim();

    if (f < 0 || f >= dim || c < 0 || c >= dim) return false;
    if (v < 1 || v > dim) return false;
    if (!tablero.dame_celda(f, c).es_vacia()) return false;

    return valores_celda.valores[f][c][v - 1].posible;
}

//devuelve el numero de posibles valores
int tReglas::posibles_valores(int f, int c) 
{
    int dim = tablero.dame_dim();

    if (!tablero.dame_celda(f, c).es_vacia()) return 0;

    int cnt = 0;

    for (int v = 0; v < dim; v++) 
    {
        if (valores_celda.valores[f][c][v].posible) cnt++;
    }
    return cnt;
}

//devuelve la lista real de valores posibles para mostrar al usuario
void tReglas::dame_valores_posibles(int f, int c, int vals[], int& n) {
    int dim = tablero.dame_dim();
    n = 0;
    if (!tablero.dame_celda(f, c).es_vacia()) return;
    for (int v = 0; v < dim; v++) {
        if (valores_celda.valores[f][c][v].posible) {
            vals[n++] = v + 1;
        }
    }
}

//inicializa todos los valores
void tReglas::inicializa_valores() 
{
    int dim = tablero.dame_dim();
    valores_celda.nFilas = dim;
    valores_celda.nColumnas = dim;

    //todo posible y sin celdas que afectan
    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
            for (int v = 0; v < dim; v++) 
            {
                valores_celda.valores[i][j][v].posible = true;
                valores_celda.valores[i][j][v].celdas_que_afectan = 0;
            }

    //procesar celdas originales y ocupadas
    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++) 
        {
            tCelda cel = tablero.dame_celda(i, j);
            if (!cel.es_vacia()) 
            {
                actualiza_valores_pon(i, j, cel.dame_valor());
            }
        }
}

//cuando se pone v en (f,c) marca como no posible en fila y col
void tReglas::actualiza_valores_pon(int f, int c, int v) {
    int dim = tablero.dame_dim();
    int idx = v - 1;

    //en al propia celda ningún valor es posible
    for (int k = 0; k < dim; k++)
    {
        valores_celda.valores[f][c][k].posible = false;
        valores_celda.valores[f][c][k].celdas_que_afectan++;
    }

    int n = 1;
    while (n * n < dim) n++;
    int iF = (f / n) * n;
    int iC = (c / n) * n;

    // para la fila
    for (int j = 0; j < dim; j++) 
    {
        if (j != c && valores_celda.valores[f][j][idx].posible) 
        {
            valores_celda.valores[f][j][idx].posible = false;
            valores_celda.valores[f][j][idx].celdas_que_afectan = 1;
        }
        else if (j != c && !valores_celda.valores[f][j][idx].posible) 
        {
            valores_celda.valores[f][j][idx].celdas_que_afectan++;
        }
    }
    //para la columna
    for (int i = 0; i < dim; i++) 
    {
        if (i != f && valores_celda.valores[i][c][idx].posible) 
        {
            valores_celda.valores[i][c][idx].posible = false;
            valores_celda.valores[i][c][idx].celdas_que_afectan = 1;
        }
        else if (i != f && !valores_celda.valores[i][c][idx].posible) 
        {
            valores_celda.valores[i][c][idx].celdas_que_afectan++;
        }
    }
    //submatriz
    for (int i = iF; i < iF + n; i++) 
    {
        for (int j = iC; j < iC + n; j++) 
        {
            if (i != f || j != c) {
                if (valores_celda.valores[i][j][idx].posible) 
                {
                    valores_celda.valores[i][j][idx].posible = false;
                    valores_celda.valores[i][j][idx].celdas_que_afectan = 1;
                }
                else 
                {
                    valores_celda.valores[i][j][idx].celdas_que_afectan++;
                }
            }
        }
    }
}

//cuando se quita v restaura los posibilidades
void tReglas::actualiza_valores_quita(int f, int c, int v) 
{
    int dim = tablero.dame_dim();
    int idx = v - 1;

    //la propia celda vuelve a estar disponible
    for (int k = 0; k < dim; k++) 
    {
        valores_celda.valores[f][c][k].celdas_que_afectan--;
        if (valores_celda.valores[f][c][k].celdas_que_afectan == 0) valores_celda.valores[f][c][k].posible = true;
    }

    int n = 1;
    while (n * n < dim) n++;
    int iF = (f / n) * n;
    int iC = (c / n) * n;

    //para la fila
    for (int j = 0; j < dim; j++) 
    {
        if (j != c) 
        {
            valores_celda.valores[f][j][idx].celdas_que_afectan--;
            if (valores_celda.valores[f][j][idx].celdas_que_afectan == 0)
                valores_celda.valores[f][j][idx].posible = true;
        }
    }
    //para columna
    for (int i = 0; i < dim; i++) 
    {
        if (i != f) 
        {
            valores_celda.valores[i][c][idx].celdas_que_afectan--;
            if (valores_celda.valores[i][c][idx].celdas_que_afectan == 0) valores_celda.valores[i][c][idx].posible = true;
        }
    }
    //submatriz
    for (int i = iF; i < iF + n; i++) 
    {
        for (int j = iC; j < iC + n; j++) 
        {
            if (i != f || j != c) 
            {
                valores_celda.valores[i][j][idx].celdas_que_afectan--;
                if (valores_celda.valores[i][j][idx].celdas_que_afectan == 0) valores_celda.valores[i][j][idx].posible = true;
            }
        }
    }
}

void tReglas::pon_valor(int f, int c, int v) 
{
    if (es_valor_posible(f, c, v)) 
    {
        tCelda celda = tablero.dame_celda(f, c);
        celda.set_valor(v);
        celda.set_ocupada();

        tablero.set_celda(f, c, celda);
        cont++;

        actualiza_valores_pon(f, c, v);
        actualiza_bloqueos();
    }
}

void tReglas::quita_valor(int f, int c) 
{
    tCelda celda = tablero.dame_celda(f, c);
    if (celda.es_ocupada() && !celda.es_original()) 
    {
        int v = celda.dame_valor();
        celda.set_vacia();
        celda.set_valor(0);

        tablero.set_celda(f, c, celda);
        cont--;

        actualiza_valores_quita(f, c, v);
        actualiza_bloqueos();
    }
}

void tReglas::actualiza_bloqueos() 
{
    lista.contBloq = 0;
    int dim = tablero.dame_dim();
    for (int i = 0; i < dim; ++i)
    {
        for (int j = 0; j < dim; ++j) 
        {
            if (tablero.dame_celda(i, j).es_vacia() && posibles_valores(i, j) == 0)
            {
                lista.lista[lista.contBloq].f = i;
                lista.lista[lista.contBloq].c = j;
                lista.contBloq++;
            }
        }
    }
}

void tReglas::reset() 
{
    tablero = tableroOriginal;
    cont = 0;

    int dim = tablero.dame_dim();
    for (int i = 0; i < dim; i++) 
    {
        for (int j = 0; j < dim; j++) 
        {
            if (tablero.dame_celda(i, j).es_original())cont++;
        }
    }
    inicializa_valores();
    actualiza_bloqueos();
}

void tReglas::autocompletar()
{
    int dim = tablero.dame_dim();
    bool cambios = true;
    while (cambios) 
    {
        cambios = false;
        for (int i = 0; i < dim; i++)
        {
            for (int j = 0; j < dim; j++) 
            {
                if (tablero.dame_celda(i, j).es_vacia())
                {
                    int numPosibles = 0, valorUnico = -1;
                    for (int v = 1; v <= dim; v++) 
                    {
                        if (es_valor_posible(i, j, v)) 
                        {
                            numPosibles++;
                            valorUnico = v;
                        }
                    }
                    if (numPosibles == 1) 
                    {
                        pon_valor(i, j, valorUnico);
                        cambios = true;
                    }
                }
            }
        }
    }
}

void tReglas::carga_sudoku(ifstream& archivo) 
{
    int dim;
    archivo >> dim;
    tablero = tTablero(dim);
    cont = 0;

    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++) 
        {
            tCelda celda;
            int valor;
            archivo >> valor;
            if (valor == 0)
            {
                celda.set_valor(0);
                celda.set_vacia();
            }
            else 
            {
                celda.set_valor(valor);
                celda.set_original();
                cont++;
            }
            tablero.set_celda(i, j, celda);
        }
    }

    tableroOriginal = tablero;
    inicializa_valores();  // inicializa la 3D
    actualiza_bloqueos();
}