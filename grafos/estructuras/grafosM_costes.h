#include <vector>
#include <cstddef>
#include <limits>

using std::vector;

template <typename T> 
class GrafoP { // Grafo ponderado
public:
    // Declaraciones adelantadas de la implementacion
    typedef T tCoste;
    typedef size_t vertice;
    static const tCoste INFINITO;

    explicit GrafoP(size_t n) : costes(n, vector<tCoste>(n, INFINITO)) {}
    size_t numVert() const {return costes.size();}
    const vector<tCoste>& operator [](vertice v) const {return costes[v];}
    vector<tCoste>& operator [](vertice v) {return costes[v];}

    // Operaciones implementadas por mi cuenta
    void rellenar(vertice n, vertice m, tCoste x);
    bool esDirigido() const;


private:
    vector< vector<tCoste> > costes;
};

// Adelantamos las declaraciones delantadas para luego implementar al definir nuestra estructura de datos
template <typename T>
const GrafoP<T>::tCoste GrafoP<T>::INFINITO = std::numeric_limits<T>::max();

template <typename T>
void GrafoP<T>::rellenar(GrafoP<T>::vertice n,GrafoP<T>::vertice m, GrafoP<T>::tCoste x){
    // introducimos en nuestra matriz de costes donde apunta nuestro vertice + coste de la arista
    costes[n][m] = x;
    if (!esDirigido()){
        costes[m][n] = x;
    }
}

template <typename T>
inline bool GrafoP<T>::esDirigido() const {
    for (GrafoP<T>::vertice i = 0; i < numVert(); i++){
        for (GrafoP<T>::vertice j = 0; j < numVert(); j++){
            if (costes[i][j] == costes[j][i]){
                return false;
            }
        }   
    }
    return true;
}


// implementacion dijkstra