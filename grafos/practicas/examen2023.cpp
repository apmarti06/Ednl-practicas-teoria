#include "../estructuras/matriz.h"
#include "./alg_grafoPMC.h"
#include "./grafoPMC.h"

#include <cmath>
#include <limits>

typedef typename GrafoP<size_t>::vertice nodo;

// pos del laberinto
struct casilla {
    size_t fil;
    size_t col;
};

// funciones conversoras necesarias
nodo casillas_to_nodo(casilla C, size_t N){
    return C.fil * N + C.col;
}

casilla nodo_to_casillas(nodo v, size_t N){
    casilla c;
        c.fil = v / N;
        c.col = v % N;
    return c;
}

// Recordamos como funciona un laberinto (n*n)
template <typename tCoste>
void cortar_caminos (nodo n, GrafoP<tCoste>& Narnia){
    // actualizamos el laberinto de forma real
    for (size_t i = 0; i < Narnia.numVert(); i++){
        Narnia[n][i] = Narnia[i][n] = GrafoP<tCoste>::INFINITO;
    }
}

// abs valor absoluto de dos números
bool casillas_mov_caballo (casilla a, casilla b){
    // comprobamos si es movimiento en L, viendo su valor absoluto (sin distinguir que sea negativo/positivo)
    // para que no tener comprobar los casos espejos
    return ((std::abs(a.fil - b.fil) == 1 && std::abs(a.col - b.col) == 2) 
    || (std::abs(a.fil - b.fil) == 2 && std::abs(a.col - b.col) == 1));
}

// devolvemos el coste si hay una salida desde la casilla inicial
template <typename tCoste>
std::pair<bool, tCoste> laberinto_Narnia (size_t N, size_t M, 
    std::vector<casilla> casillas_trampas, std::vector<casilla> casillas_caballeros)
{      
    // Creamos datos iniciales
    casilla inicial(0, 0), final(N-1, M-1);
    GrafoP<tCoste> Narnia(N*M); 
    size_t n = Narnia.numVert();

    // comenzamos a actualizar todo el laberinto, primero con los caminos accesibles e inaccesibles
    for (size_t i = 0; i < n; i++){
        for (size_t j = 0; j < n; j++){

            if (i != j){
                casilla c1 = nodo_to_casillas(i, N);
                casilla c2 = nodo_to_casillas(j, N);

                // comprobamos las casillas accesibles del laberinto N, M
                if (casillas_mov_caballo(c1, c2)){
                    Narnia[i][j] = Narnia[j][i] = 1;
                } else {
                    Narnia[i][j] = Narnia[j][i] = GrafoP<tCoste>::INFINITO;
                }

            } else {
                // el coste directo de ir a si mismo es cero
                Narnia[i][j] = Narnia[j][i] = 0;
            }
        }
    }

    // Colocamos ahora las casillas trampas 
    for (const casilla& trampa : casillas_trampas){
        nodo n = casillas_to_nodo(trampa, N);
        cortar_caminos(n, Narnia);
    }

    // Colocamos las casillas que son cortadas por el caballero
    for (const casilla& trampa : casillas_caballeros){
        // actualizamos todas las casillas a alrededor 
        for (size_t df = -1; df <= 1; df++){
            for (size_t dc = -1; dc <= 1; dc++){
                // solo podemos no pisar las que marque el caballero, la suya se puede pasar
                if (!(df == 0 && dc == 0)){
                    // Escogemos las pos de las casilla
                    size_t nf = df + trampa.fil;
                    size_t nc = dc + trampa.col;

                    // Vemos si se encuentra en el rango (dentro del laberinto) en la fila o columna
                    if (nf >= 0 && nf < N && nc >= 0 && nc < M){
                        nodo n = casillas_to_nodo((nf,nc), N);
                        cortar_caminos(n, Narnia);
                    }
                }
            }
        }
    }

    std::vector<nodo> camino_recorrido;
    std::vector<tCoste> costes_min = Dijkstra(Narnia, casillas_to_nodo(inicial, N), camino_recorrido);

    // dijkstra debe actualizar mi camino si existe forma de llegar a esa casilla, desde el origen
    if (costes_min[casillas_to_nodo(final, N)] == GrafoP<tCoste>::INFINITO){
        return std::make_pair(false, 0);
    } else {
        // si se quisiera devolver el camino 
        // typename GrafoP<tCoste>::tcamino camino_laberinto = camino<tCoste>(inicial, final, P);
        return std::make_pair(true, costes_min[casillas_to_nodo(final, N)]);
    }
}