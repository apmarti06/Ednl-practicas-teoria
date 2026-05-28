#include "../estructuras/grafosM_costes.h" 
#include "../estructuras/grafosLISTP.h"

// habra que añadir dos rutas para que puedan usarse para ambas implementaciones (vector de listas y matriz)

#ifndef Dijkstra_H
#define Dijkstra_H


// Devuelve el camino de orig a v a partir de un vector
// P obtenido mediante la función Dijkstra().
template <typename tCoste>
typename GrafoP<tCoste>::tCamino camino(typename GrafoP<tCoste>::vertice orig, 
        typename GrafoP<tCoste>::vertice v,
        const vector<typename GrafoP<tCoste>::vertice>& P);

// tras llamar a camino, se puede llamar a su otra funcion

// Incluimos el algoritmo para calcular el coste minimo del camino, declaracion adelantada
template <typename tCoste>
vector<tCoste> Dijkstra(const GrafoP<tCoste>& G, typename GrafoP<tCoste>::vertice origen, vector<typename GrafoP<tCoste>::vertice>& P);

#endif