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

//te dice si has terminado el sudoku o no
bool tReglas::terminado()
{
    int dim = tablero.dame_dim();

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            tCelda celda = tablero.dame_celda(i, j);
            if (celda.es_vacia()){
                return false; // Hay al menos una celda vacía
            }
        }
    }
    return true; // No hay celdas vacías
}


//funcion en proceso
bool tReglas::bloqueo()
{
    int dim = tablero.dame_dim();

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            tCelda celda = tablero.dame_celda(i, j);
            if (celda.es_ocupada()) {
                return true; // Hay al menos una celda bloqueada
            }
        }
    }
    return false;
}

//no se a que se refiere con celda bloqueada
int tReglas::dame_num_celdas_bloqueadas()
{
    int contador = 0;
    int dim = tablero.dame_dim();

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            tCelda celda = tablero.dame_celda(i, j);
            if (celda.es_ocupada()) {
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
            tCelda celda = tablero.dame_celda(i, j);
            if (celda.es_vacia()) {
                contador++;
            }
        }
    }
    return contador;
}

tCelda tReglas::dame_celda_bloqueada(int p, int& f, int& c)
{
    int contador = 0;
    int dim = tablero.dame_dim();

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            tCelda celda = tablero.dame_celda(i, j);
            if (celda.es_ocupada()) {
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
    tCelda celda;
    celda.set_vacia();
    return celda;
}

bool tReglas::es_valor_posible(int f, int c, int v)
{
    int dim = tablero.dame_dim();

    // Verificar si la celda está bloqueada
    tCelda celda = tablero.dame_celda(f, c);
    if (celda.es_ocupada()) {
        return false;
    }

    // Verificar fila
    for (int j = 0; j < dim; j++) {
        if (j != c && celda.dame_valor() == v) {
            return false;
        }
    }

    // Verificar columna
    for (int i = 0; i < dim; i++) {
        if (i != f &&celda.dame_valor() == v) {
            return false;
        }
    }

    // Verificar región 3x3 (para Sudoku estándar de 9x9)
    if (dim == 9) {
        int inicioFila = (f / 3) * 3;
        int inicioCol = (c / 3) * 3;

        for (int i = inicioFila; i < inicioFila + 3; i++) {
            for (int j = inicioCol; j < inicioCol + 3; j++) {
                if (i != f && j != c && celda.dame_valor() == v) {
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
    tCelda celda = tablero.dame_celda(f, c);
    if (celda.es_ocupada() || !celda.es_vacia()) {
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
    tCelda celda = tablero.dame_celda(f, c);

    if (!celda.es_ocupada() && v >= 1 && v <= tablero.dame_dim()) {
        tablero.set_valor(f, c, v);
    }
}

void tReglas::quita_valor(int f, int c)
{
    tCelda celda;
    if (!celda.es_ocupada()) {
        celda.set_vacia();
        tablero.set_celda(f, c, celda);
    }
}

void tReglas::reset()
{
   // tablero = tableroOriginal;
}

void tReglas::autocompletar()
{
    int dim = tablero.dame_dim();
    bool cambios = true;

    while (cambios) {
        cambios = false;

        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                // Solo procesar celdas vacías y no bloqueadas
                tCelda celda = tablero.dame_celda(i, j);
                if (celda.es_vacia() && !celda.es_ocupada()) {
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
    //tablero = tTablero(dim);
    //tableroOriginal = tTablero(dim);

    // Cargar los valores del archivo
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            int valor;
            arch >> valor;

            if (valor >= 1 && valor <= dim) {
                //tablero.pon_celda(i, j, valor);
                //tablero.bloquea_celda(i, j, true);
            }
            else {
               // tablero.pon_celda(i, j, VACIA);
               // tablero.bloquea_celda(i, j, false);
            }
        }
    }

    // Guardar una copia del tablero original
    tableroOriginal = tablero;
}