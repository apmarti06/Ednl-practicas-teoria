#ifndef MATRIZ_ADYACENTE_H
#define MATRIZ_ADYACENTE_H

#include <vector>
#include <cstddef>

class Grafo {
    public: 
        typedef size_t vertice;
        explicit Grafo(size_t n) : ady(n, vector<bool>(n, false)) {}
        size_t numVert() const {return ady.size();}

        // Nos devuelve la aritsta deseado, consultando el vertice x
        const vector<bool>& operator [](vertice v) const {return ady[v];}
        vector<bool>& operator [](vertice v) {return ady[v];}

        /*resto de operaciones*/

    private: 
        vector< vector<bool> > ady;
};

#endif