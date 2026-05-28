/* Comentarios del algoritmo en clase
#include “matriz.h”

using std::vector;

template <typename tCoste>
matriz<tCoste> Floyd(const GrafoP<tCoste>& G, matriz<typename GrafoP<tCoste>::vertice>& P){
    assert(G.numVert() > 0);
    typedef typename GrafoP<tCoste>::vertice vertice;
    const size_t n = G.numVert();
    matriz<tCoste> A(n); // matriz de costes mínimos
    // Iniciar A y P con caminos directos entre cada par de vértices.P = matriz<vertice>(n);
    
    for (vertice i = 0; i <= n-1; i++) {
        A[i] = G[i]; // copia costes del grafo
        A[i][i] = 0; // diagonal a 0, pues es el coste de ir a si mismo
        P[i] = vector<vertice>(n, i); // caminos directos, inicializamos todos los vertices a si mismo, osea a su origen P[origen] = su vertice;
    }

    // Calcular costes mínimos y caminos correspondientes
    // entre cualquier par de vértices i, j
    for (vertice k = 0; k <= n-1; k++)
        for (vertice i = 0; i <= n-1; i++)
            for (vertice j = 0; j <= n-1; j++) {
            tCoste ikj = suma(A[i][k], A[k][j]); // caminos i->j que queremos minimizar
            if (ikj < A[i][j]) { // vemos si el camino ikj es mejor que el de i->j
                A[i][j] = ikj;
                P[i][j] = k;
            }
        }
    return A;
}

#include "listaenla.h"

template <typename T> class GrafoP {
public:
    typedef Lista<vertice> tCamino;
    // ...
};

template <typename tCoste> typename GrafoP<tCoste>::tCaminocaminoAux(typename GrafoP<tCoste>::vertice v,
typename GrafoP<tCoste>::vertice w, const matriz<typename GrafoP<tCoste>::vertice>& P){ 
// Devuelve el camino de coste mínimo entre v y w, exluidos estos,
// a partir de una matriz P obtenida mediante la función Floyd().

typename GrafoP<tCoste>::tCamino C1, C2;
typename GrafoP<tCoste>::vertice u;
u = P[v][w]; // es el nodo último usado para optimizar el camino v->w
    if (u != v) {
        // Hacemos un monton de llamadas recursivas donde se guarda el vértice de coste mínimo
        C1 = caminoAux<tCoste>(v, u, P);
        C1.insertar(u, c1.fin());
        C2 = caminoAux<tCoste>(u, w, P);
        C1 += C2; // Lista<vertice>::operator +=(), concatena C1 y C2
        }
    return C1;
}


#include “matriz.h”
matriz<bool> Warshall(const Grafo& G)
{
    assert(G.numVert() > 0);
    typedef Grafo::vertice vertice;
    const size_t n = G.numVert();
    matriz<bool> A(n);

    // Inicializar A con la matriz de adyacencia de G
    for (vertice i = 0; i <= n-1; i++) {
        A[i] = G[i];
        A[i][i] = true;
    }
    // Comprobar camino entre cada par de vértices i, j
    // a través de cada vértice k
    for (vertice k = 0; k <= n-1; k++)
        for (vertice i = 0; i <= n-1; i++)
            for (vertice j = 0; j <= n-1; j++)
                if (!A[i][j])
                    A[i][j] = A[i][k] && A[k][j];
    return A;
}


*/