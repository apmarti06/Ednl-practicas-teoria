#include <vector>
#include "./lista_enla.h"


class Grafo {
    public:
        typedef size_t vertice; // un valor entre 0 y Grafo::numVert()-1
        explicit Grafo(size_t n): ady(n) {}
        size_t numVert() const {return ady.size(); }
        const Lista<vertice>& adyacentes(vertice v) const {return ady[v];}
        Lista<vertice>& adyacentes(vertice v) {return ady[v];}
    private:
        vector< Lista<vertice> > ady;
};

