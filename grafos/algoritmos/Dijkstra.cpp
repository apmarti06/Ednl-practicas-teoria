#include "./Dijkstra.h"

// Devuelve el camino de orig a v a partir de un vector
// P obtenido mediante la función Dijkstra().
template <typename tCoste>
typename GrafoP<tCoste>::tCamino camino(typename GrafoP<tCoste>::vertice orig, 
        typename GrafoP<tCoste>::vertice v,
        const vector<typename GrafoP<tCoste>::vertice>& P)
{
    typename GrafoP<tCoste>::tCamino C;

    C.insertar(v, C.primera());
    
    do {
        C.insertar(P[v], C.primera());
        v = P[v];
    } while (v != orig);

    return C;
}

// Suma de costes
template <typename tCoste>
tCoste suma(tCoste x, tCoste y)
{
    const tCoste INF = GrafoP<tCoste>::INFINITO; // asi no declaramos dos veces lo mismo

    return (x == INF || y == INF) ? INF : x + y;
}

template <typename tCoste>
vector<tCoste> Dijkstra(const GrafoP<tCoste>& G, 
                        typename GrafoP<tCoste>::vertice origen,
                        vector<typename GrafoP<tCoste>::vertice>& P)
{   
    assert(G.numVert() > 2);

    typedef typename GrafoP<tCoste>::vertice vertice;
    vertice v, w;
    const size_t n = G.numVert(); 

    vector<bool> S(n, false);                 // vértices ya seleccionados, donde guardaremos los vertices deseados
    vector<tCoste> D(n);                      // costes mínimos desde origen

    // Inicialización
    D = G[origen];                            // costes directos desde origen
    D[origen] = 0;                            // de origen a origen vale 0, importante para cualquier caso!!!!
    P = vector<vertice>(n, origen);           // predecesores iniciales, de todos los nodos es el origen (aunque no sea verdad)

    S[origen] = true; // sabemos que el coste de origen a si mismo es el minimo, por lo que ponemos que ya ha sido encontrado
   
    for (size_t i = 1; i <= n - 2; i++)
    {
        // Buscar vértice no marcado con menor coste
        tCoste costeMin = GrafoP<tCoste>::INFINITO;
        for (v = 0; v < n; v++) // guardamos para g[i][v] el coste minimo desde origen
        {
            if (!S[v] && D[v] <= costeMin) // que o este en s y tenga el menor coste provisional de d
            {
                costeMin = D[v];
                w = v;
            }
        }

        S[w] = true; // meter w en el conjunto de definitivos

        // Relajar caminos pasando por w
        for (v = 0; v < n; v++)
        {
            if (!S[v])
            {   
                // vemos cual es el recorrido actual de menor coste
                tCoste Owv = suma(D[w], G[w][v]); 
                if (Owv < D[v])
                {   
                    //En D lo que cuesta desde el vertice actual hasta v, y P que pasa por w para llegar a v
                    D[v] = Owv;
                    P[v] = w;
                }
            }
        }
    }
    // retorna el coste minimo, junto al camino del coste mínimo en P
    return D;
}

