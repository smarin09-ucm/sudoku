#include <string>
#include <fstream>

#include "celda.h"
#include "tablero.h"
#include "reglasTablero.h"


using namespace std;

int tReglas::dame_dimension()
{
    return tablero.dame_dim();
}

tCelda tReglas::dame_celda(int f, int c)
{
    //return tablero.dame_celda(f, c);
}

bool tReglas::terminado()
{
    int dim = tablero.dame_dim();

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            if (/*tablero.dame_celda(i, j) == VACIA*/) {
                return false; // Hay al menos una celda vacía
            }
        }
    }
    return true; // No hay celdas vacías
}

bool tReglas::bloqueo()
{
    int dim = tablero.dame_dim();

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            if (/**/tablero.celda_bloqueada(i, j)) {
                return true; // Hay al menos una celda bloqueada
            }
        }
    }
    return false;
}

int tReglas::dame_num_celdas_bloqueadas()
{
    int contador = 0;
    int dim = tablero.dame_dim();

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            if (tablero.celda_bloqueada(i, j)) {
                contador++;
            }
        }
    }
    return contador;
}

int tReglas::dame_num_celdas_vacias()
{
    int contador = 0;
    int dim = tablero.dame_dimension();

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            if (tablero.dame_celda(i, j) == VACIA) {
                contador++;
            }
        }
    }
    return contador;
}

tCelda tReglas::dame_celda_bloqueada(int p, int& f, int& c)
{
    int contador = 0;
    int dim = tablero.dame_dimension();

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            if (tablero.celda_bloqueada(i, j)) {
                if (contador == p) {
                    f = i;
                    c = j;
                    return tablero.dame_celda(i, j);
                }
                contador++;
            }
        }
    }

    // Si no se encuentra la posición p
    f = -1;
    c = -1;
    return VACIA;
}

bool tReglas::es_valor_posible(int f, int c, int v)
{
    int dim = tablero.dame_dim();

    // Verificar si la celda está bloqueada
    if (tablero.celda_bloqueada(f, c)) {
        return false;
    }

    // Verificar fila
    for (int j = 0; j < dim; j++) {
        if (j != c && tablero.dame_celda(f, j) == v) {
            return false;
        }
    }

    // Verificar columna
    for (int i = 0; i < dim; i++) {
        if (i != f && tablero.dame_celda(i, c) == v) {
            return false;
        }
    }

    // Verificar región 3x3 (para Sudoku estándar de 9x9)
    if (dim == 9) {
        int inicioFila = (f / 3) * 3;
        int inicioCol = (c / 3) * 3;

        for (int i = inicioFila; i < inicioFila + 3; i++) {
            for (int j = inicioCol; j < inicioCol + 3; j++) {
                if (i != f && j != c && tablero.dame_celda(i, j) == v) {
                    return false;
                }
            }
        }
    }

    return true;
}

int tReglas::posibles_valores(int f, int c)
{
    int dim = tablero.dame_dim();
    int contador = 0;

    // Si la celda está bloqueada o no está vacía, no tiene valores posibles
    if (tablero.celda_bloqueada(f, c) || tablero.dame_celda(f, c) != VACIA) {
        return 0;
    }

    // Probar todos los valores posibles del 1 al 9 (para Sudoku estándar)
    for (int v = 1; v <= dim; v++) {
        if (es_valor_posible(f, c, v)) {
            contador++;
        }
    }

    return contador;
}

void tReglas::pon_valor(int f, int c, int v)
{
    if (!tablero.celda_bloqueada(f, c) && v >= 1 && v <= tablero.dame_dimension()) {
        tablero.pon_celda(f, c, v);
    }
}

void tReglas::quita_valor(int f, int c)
{
    if (!tablero.celda_bloqueada(f, c)) {
        tablero.pon_celda(f, c, VACIA);
    }
}

void tReglas::reset()
{
    tablero = tableroOriginal;
}

void tReglas::autocompletar()
{
    int dim = tablero.dame_dimension();
    bool cambios = true;

    while (cambios) {
        cambios = false;

        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                // Solo procesar celdas vacías y no bloqueadas
                if (tablero.dame_celda(i, j) == VACIA && !tablero.celda_bloqueada(i, j)) {
                    int numPosibles = 0;
                    int valorUnico = -1;

                    // Encontrar el único valor posible
                    for (int v = 1; v <= dim; v++) {
                        if (es_valor_posible(i, j, v)) {
                            numPosibles++;
                            valorUnico = v;
                        }
                    }

                    // Si solo hay un valor posible, colocarlo
                    if (numPosibles == 1) {
                        pon_valor(i, j, valorUnico);
                        cambios = true;
                    }
                }
            }
        }
    }
}

void tReglas::carga_sudoku(ifstream& arch)
{
    int dim;
    arch >> dim;

    // Crear tableros con la dimensión leída
    tablero = tTablero(dim);
    tableroOriginal = tTablero(dim);

    // Cargar los valores del archivo
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            int valor;
            arch >> valor;

            if (valor >= 1 && valor <= dim) {
                tablero.pon_celda(i, j, valor);
                tablero.bloquea_celda(i, j, true);
            }
            else {
                tablero.pon_celda(i, j, VACIA);
                tablero.bloquea_celda(i, j, false);
            }
        }
    }

    // Guardar una copia del tablero original
    tableroOriginal = tablero;
}