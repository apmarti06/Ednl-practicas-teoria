#include <vector>
#include "listaenla.h"

#ifndef GRAFOSLISTA_PONDERADA_H
#define GRAFOSLISTA_PONDERADA_H

using std::vector;

template <typename T> 
class GrafoP { // Grafo ponderado
    public:
        typedef T tCoste; // el coste es el de tipo del grafo creado
        typedef size_t vertice; // un valor entre 0 y GrafoP::numVert()-1
        static const tCoste INFINITO; // peso de arista inexistente

        // incluimos tipo necesario para el algoritmo de Dijkstra
        typedef Lista<vertice> tCamino;

        struct vertice_coste { // vértice adyacente y coste, lo declaramos en la parte pública, para usarlo en funciones sin restricciones
            vertice v;
            tCoste c;
            // requerido por Lista<vertice_coste>::buscar()
            bool operator ==(const vertice_coste& vc) const {return v == vc.v;}
        };

        GrafoP(size_t n): ady(n) {}
        size_t numVert() const {return ady.size();}
        const Lista<vertice_coste>& adyacentes(vertice v) const {return ady[v];}
        Lista<vertice_coste>& adyacentes(vertice v) {return ady[v];}

    private:
        vector<Lista<vertice_coste> > ady; // vector de listas de vértice-coste
};

// Adelantamos las declaraciones delantadas para luego implementar al definir nuestra estructura de datos
template <typename T>
const GrafoP<T>::tCoste GrafoP<T>::INFINITO = std::numeric_limits<T>::max();



#endif
