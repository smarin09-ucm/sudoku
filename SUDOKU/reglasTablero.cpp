#include "checkML.h"
#include "reglasTablero.h"
#include <fstream>
#include <iostream>
using namespace std;

//GESTION INTERNA DE LISTA BLOQUEADAS (MEMORIA DINAMICA)
void tReglas::copia_lista_bloq(const tListaBloq& origen)
{
    lista.capacidad = origen.capacidad;
    lista.contBloq = origen.contBloq;

    //reservar array de punteros
    lista.lista = new tPos * [lista.capacidad];

    for (int i = 0; i < lista.contBloq; i++)
    {
        if (origen.lista[i] != nullptr)
        {
            lista.lista[i] = new tPos;
            *lista.lista[i] = *origen.lista[i];  
        }
        else
        {
            lista.lista[i] = nullptr;
        }
    }
}

void tReglas::libera_lista_bloq()
{
    if (lista.lista != nullptr)
    {
        for (int i = 0; i < lista.contBloq; i++)
        {
            delete lista.lista[i];   // liberar cada tPos
            lista.lista[i] = nullptr;
        }
        delete[] lista.lista;        // liberar array de punteros
    }

    lista.lista = nullptr;
    lista.contBloq = 0;
    lista.capacidad = 0;
}

void tReglas::inicializa_lista_bloq()
{
    lista.contBloq = 0;
    lista.capacidad = MAX_DIM * MAX_DIM;
    lista.lista = new tPos*[lista.capacidad];
}
//CONSTRUCTORA, DESTRUCTORA Y OPERADOR
tReglas::tReglas()
{
    cont = 0;
    lista.lista = nullptr;
    lista.contBloq = 0;
    lista.capacidad = 0;
}

//constructora con parametros
tReglas::tReglas(const tReglas& r)
{
    tablero = r.tablero;
    tableroOriginal = r.tableroOriginal;
    cont = r.cont;
    valores_celda = r.valores_celda;

    lista.lista = nullptr;
    lista.contBloq = 0;
    lista.capacidad = 0;

    copia_lista_bloq(r.lista);
}

//operador =
tReglas& tReglas::operator=(const tReglas& r)
{
    if (this != &r)
    {
        libera_lista_bloq();  // libera lo que habia

        tablero = r.tablero;
        tableroOriginal = r.tableroOriginal;
        cont = r.cont;
        valores_celda = r.valores_celda;

        copia_lista_bloq(r.lista);
    }
    return *this;
}

//destructora
tReglas::~tReglas()
{
    libera_lista_bloq();
}

//CONSULTORAS
//devuelve la dimension
const int tReglas::dame_dimension()
{
    return tablero.dame_dim();
}

//devuelve la celda
tCelda tReglas::dame_celda(int f, int c)
{
    return tablero.dame_celda(f, c);
}

//devuelve true si esta terminado
const bool tReglas::terminado()
{
    int dim = tablero.dame_dim();
    return (cont == dim * dim) && !bloqueo();
}

//devuelve true si hay algún bloqueo
const bool tReglas::bloqueo()
{
    return lista.contBloq > 0;
}

//devuelve el nuemro de celdas bloqueadas
const int tReglas::dame_num_celdas_bloqueadas()
{
    return lista.contBloq;
}

//devuelve el numero de celdas vacias
const int tReglas::dame_num_celdas_vacias()
{
    int dim = tablero.dame_dim();
    return dim * dim - cont;
}

//devuelve la celda bloqueada
const tCelda tReglas::dame_celda_bloqueada(int p, int& f, int& c)
{
    if (p >= 0 && p < lista.contBloq)
    {
        f = lista.lista[p]->f;
        c = lista.lista[p]->c;
    }
    else f = c = -1;
    return tablero.dame_celda(f, c);
}

//devuelve true si el valor se puede colocar
const bool tReglas::es_valor_posible(int f, int c, int v)
{
    int dim = tablero.dame_dim();
    if (f < 0 || f >= dim || c < 0 || c >= dim) return false;
    if (v < 1 || v > dim) return false;
    if (!tablero.dame_celda(f, c).es_vacia()) return false;
    return valores_celda.valores[f][c][v - 1].posible;
}

//devuelve el numero de posibles valores
const int tReglas::posibles_valores(int f, int c)
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

// cuantas celdas vacias tienen exactamente n valores posibles
int tReglas::dame_num_celdas_con_n_posibles(int n)
{
    int dim = tablero.dame_dim();
    int cnt = 0;
    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
            if (tablero.dame_celda(i, j).es_vacia() && posibles_valores(i, j) == n)
                cnt++;
    return cnt;
}

//LOGICA INTERNA DE VALORES POSIBLES
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
                actualiza_valores(i, j, cel.dame_valor(), true);
            }
        }
}

void tReglas::actualiza_una_celda(int i, int j, int idx, bool poniendo)
{
    int delta = poniendo ? 1 : -1;
    valores_celda.valores[i][j][idx].celdas_que_afectan += delta;
    if (valores_celda.valores[i][j][idx].celdas_que_afectan > 0)
        valores_celda.valores[i][j][idx].posible = false;
    else
        valores_celda.valores[i][j][idx].posible = true;
}

void tReglas::actualiza_valores(int f, int c, int v, bool poniendo)
{
    int dim = tablero.dame_dim();
    int idx = v - 1;
    int delta = poniendo ? 1 : -1;

    // la propia celda
    for (int k = 0; k < dim; k++)
    {
        valores_celda.valores[f][c][k].celdas_que_afectan += delta;
        if (valores_celda.valores[f][c][k].celdas_que_afectan > 0)
            valores_celda.valores[f][c][k].posible = false;
        else
            valores_celda.valores[f][c][k].posible = true;
    }

    int n = 1;
    while (n * n < dim) n++;
    int iF = (f / n) * n;
    int iC = (c / n) * n;

    for (int j = 0; j < dim; j++)
        if (j != c) actualiza_una_celda(f, j, idx, poniendo);

    for (int i = 0; i < dim; i++)
        if (i != f) actualiza_una_celda(i, c, idx, poniendo);

    for (int i = iF; i < iF + n; i++)
        for (int j = iC; j < iC + n; j++)
            if (i != f || j != c) actualiza_una_celda(i, j, idx, poniendo);
}

void tReglas::actualiza_bloqueos()
{
    // liberar los anteriores
    for (int i = 0; i < lista.contBloq; i++)
        delete lista.lista[i];

    lista.contBloq = 0;

    int dim = tablero.dame_dim();

    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            if (tablero.dame_celda(i, j).es_vacia() && posibles_valores(i, j) == 0)
            {
                if (lista.contBloq < lista.capacidad)
                {
                    lista.lista[lista.contBloq] = new tPos;
                    lista.lista[lista.contBloq]->f = i;
                    lista.lista[lista.contBloq]->c = j;
                    lista.contBloq++;
                }
            }
        }
    }
}

//MODIFICADORES
void tReglas::pon_valor(int f, int c, int v) {
    if (es_valor_posible(f, c, v)) { 
        tCelda celda = tablero.dame_celda(f, c); 
        celda.set_valor(v); celda.set_ocupada(); 
        tablero.set_celda(f, c, celda); cont++; 
        actualiza_valores(f, c, v, true);
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

        actualiza_valores(f, c, v, false);
        actualiza_bloqueos();
    }
}

void tReglas::reset() 
{
    int dim = tablero.dame_dim();
    for (int i = 0; i < dim; i++) 
    {
        for (int j = 0; j < dim; j++) 
        {
            tCelda celda = tablero.dame_celda(i, j);
            if (celda.es_ocupada() && !celda.es_original())
                quita_valor(i, j);
        }
    }
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

//CARGA Y GUARDADO
void tReglas::carga_sudoku(ifstream& archivo) 
{
    libera_lista_bloq();
    inicializa_lista_bloq();

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
// guarda la partida en el formato del archivo de partidas
void tReglas::guarda_partida(ofstream& arch)
{
    int dim = tablero.dame_dim();
    // cabecera igual que un sudoku original (solo las celdas originales)
    arch << dim << "\n";
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            tCelda cel = tableroOriginal.dame_celda(i, j);
            if (j > 0) arch << " ";
            arch << cel.dame_valor();
        }
        arch << "\n";
    }
    // celdas ocupadas por el jugador
    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
        {
            tCelda cel = tablero.dame_celda(i, j);
            if (cel.es_ocupada())
                arch << i << " " << j << " " << cel.dame_valor() << "\n";
        }
    arch << "-1\n";
}

//RESTO DE OPERADORES
bool operator<(tReglas& a, tReglas& b)
{
    int vaciasA = a.dame_num_celdas_vacias();
    int vaciasB = b.dame_num_celdas_vacias();
    if (vaciasA != vaciasB) return vaciasA < vaciasB;

    int dim = a.dame_dimension();
    for (int n = 1; n <= dim; n++)
    {
        int cA = a.dame_num_celdas_con_n_posibles(n);
        int cB = b.dame_num_celdas_con_n_posibles(n);
        if (cA != cB) return cA > cB;
    }
    return false;
}

bool operator==(tReglas& a, tReglas& b)
{
    bool aMenorB = operator<(a, b);
    bool bMenorA = operator<(b, a);
    return !aMenorB && !bMenorA;
}

