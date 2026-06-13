#include "../estructuras/particion_agene.h"
#include "../estructuras/apo.h"
#include "../estructuras/matriz.h"
#include "./alg_grafoPMC.h"
#include "./grafoPMC.h"
#include "./grafoMA.h"

#include <cmath>
#include <limits>
/* El archipiélago de Tombuctú3 está formado por un número desconocido de islas, cada una de las cuales tiene
, a su vez, un número desconocido de ciudades, las cuales tienen en común que todas y cada una de ellas dispone de un aeropuerto.

Sí que se conoce el número total de ciudades del archipiélago (podemos llamarlo N, por ejemplo). Dentro de cada una de las islas existen 
carreteras que permiten viajar entre todas las ciudades de la isla. No existen puentes que unan las islas y se ha decidido 
que la opción de comunicación más económica de implantar será el avión. Se dispone de las coordenadas cartesianas (x, y) 
de todas y cada una de las ciudades del archipiélago. Se dispone de un grafo (matriz de adyacencia) en el que se indica si existe
carretera directa entre cualesquiera dos ciudades del archipiélago. 

El objetivo de nuestro problema es encontrar qué líneas aéreas debemos implantar para poder viajar entre todas las ciudades del archipiélago,
siguiendo los siguientes criterios:

1) Se implantará el mínimo número de líneas aéreas entre islas. 
2) La línea aérea escogida entre cada par de islas será la más corta entre todas las posibles. 

Así pues, dados los siguientes datos: 
• Lista de ciudades de Tombuctú3 representada cada una de ellas por sus coordenadas cartesianas.
 • Matriz de adyacencia de Tombuctú que indica las carreteras existentes en dicho archipiélago,
implementen un subprograma que calcule y devuelva las líneas aéreas necesarias para comunicar adecuadamente el archipiélago siguiendo los 
criterios anteriormente expuestos.*/

struct LineaArea{
    size_t cOrigen, cDestino;
    double coste;
    bool operator < (const LineaArea& a, const LineaArea& b){
        return a.coste < b.coste;
    }
    LineaArea(size_t c1, size_t c2, double c) : cOrigen{c1}, cDestino{c2}, coste{c} {}
};

struct coordenada {
    double x, y;
    coordenada(double a, double b) : x{a}, y{b} {}
};

double DistanciaEuclidea(coordenada a, coordenada b){
    return std::sqrt(std::pow(std::abs(a.x - b.x), 2) + std::pow(std::abs(a.y - b.x), 2));
}

vector<LineaArea> tombuctu3(const vector<coordenada>& ciudades_tombuctu, const Grafo& ady)
{
    size_t n_ciudades = ciudades_tombuctu.size();

    // Creamos una particion para crear el número de islas
    size_t n_islas = n_ciudades;
    Particion distribucion(n_islas);
    
    // como esta representado por ciudades cartesianas hacemos particion tal
    for (size_t i = 0; i < n_ciudades; i++){
        for (size_t j = i + 1; j < n_ciudades; j++){
            int rep_i = distribucion.encontrar(i);
            int rep_j = distribucion.encontrar(j);
            if (ady[i][j] && rep_i != rep_j){
                // significa que pertenecen a la misma isla y lo unimos, actualizando el número de islas
                distribucion.unir(i, j);
                n_islas--;
            }
        }
    }

    // Una vez representado las islas insertamos todas las posibles aristas en el grafo, que seran aquellas islas que sean distintas (grafo no dirigido)
    Apo<LineaArea> A(n_ciudades * (n_ciudades - 1) / 2);
    for (size_t i = 0; i < n_ciudades; i++){
        for (size_t j = i + 1; j < n_ciudades; j++){
            // Como introducimos en el apo las candidatas (las ciudades que pertenecen a distintas islas)
            if (distribucion.encontrar(i) != distribucion(j)){
                A.insertar({i, j, DistanciaEuclidea(ciudades_tombuctu[i], ciudades_tombuctu[j])});
            }
        }
    }

    // Guardamos todas las lineas areas posibles
    size_t i = 0;
    vector<LineaArea> lineas_areas;
    while (i < n_islas - 1){
        LineaArea a = A.cima(); A.suprimir();
        if (distribucion.encontrar(a.cOrigen) != distribucion.encontrar(a.cDestino)){
            distribucion.unir(a.cOrigen, a.cDestino);
            lineas_areas.push_back(a);
            i++;
        }
    }
    return lineas_areas;
}


