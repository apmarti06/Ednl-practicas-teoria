#include <iostream>
#include "../estructuras/particion_agene.h"
#include "../estructuras/apo.h"
#include "../estructuras/matriz.h"
#include "./alg_grafoPMC.h"
#include "./grafoPMC.h"
#include "./grafoMA.h"
#include "alg_grafo_E-S.h"

// ejercicio 1
typedef size_t agrupacion;

std::pair<agrupacion, agrupacion> VivaElCarnaval(Grafo& Coac, const vector<agrupacion>& agrupaciones)
{   
    // le pasamos una matriz de adyacencia, que significa si le ha dedicado un cuple o no a otra agrupacion,
    // y vemos quien recibe mas menciones y quien hace mas cuple
    size_t n = Coac.numVert();
    agrupacion ganador1 = 0, ganador2 = 0;
    vector<int> menciones(n, 0);
    
    size_t mejor_cuenta = 0;
    for (size_t i = 0; i < n; i++){
        // solo tomamos desde el siguiente vertice, pues no se va a dedicar un cuple a si mismo
        size_t actual = 0;
        size_t j = i + 1;

        do {   
            if (Coac[i][j]){
                actual++;
                menciones[j]++;
            }
            j = (j + 1) % n;
        } while (j != i);

        if (actual > mejor_cuenta){
            mejor_cuenta = actual;
            ganador1 = i;
        }
    }

    int max_menciones = 0;

    for (size_t i = 0; i < agrupaciones.size(); i++){
        if (menciones[i] > max_menciones){
            max_menciones = menciones[i];
            ganador2 = i;
        }
    }

    return std::make_pair(ganador1, ganador2);
}

// ejercicio 2

template <typename tCoste>
struct Robo {
    bool posible;
    tCoste tiempo;
    typename GrafoP<tCoste>::tCamino camino;
};

template <typename tCoste>
Robo<tCoste> calcular_ruta_posible 
(const GrafoP<tCoste>& G, size_t entrada, size_t salida
, size_t joya1, size_t joya2)
{   
    const tCoste INF = GrafoP<tCoste>::INFINITO;
    Robo<tCoste> res;
    res.tiempo = 0, res.posible = false;

    // Calculamos si nuestro gallo puede pasar por las dos joyas e irse ileso
    vector<size_t> entrada_joya1, joya1_joya2, joya2_salida;
    vector<tCoste> coste_e_j1 = Dijkstra(G, entrada, entrada_joya1);
    vector<tCoste> coste_j1_j2 = Dijkstra(G, joya1, joya1_joya2);
    vector<tCoste> coste_j2_s = Dijkstra(G, joya2, joya2_salida);

    // si no es viable posible = false;
    if (coste_ej1[joya1] != INF && coste_ej2[joya2] != INF && coste_j2_s[salida] != INF){
        return res;
    }
    
    typename GrafoP<tCoste>::tCamino cam1, cam2, cam3;
    cam1 = camino<tCoste>(entrada, joya1, entrada_joya1);
    cam2 = camino<tCoste>(joya1, joya2, joya1_joya2);
    cam3 = camino<tCoste>(joya2, salida, joya2_salida);

    // 1. Copiar cam1 completo
    res.camino = cam1;

    // 2. Añadir cam2 saltando el primer elemento (joya1)
    if (!cam2.vacia()) {
        auto it = cam2.primera();  // cabecera
        it = cam2.siguiente(it);    // primer elemento real (joya1)
        it = cam2.siguiente(it);    // segundo elemento real (saltamos joya1)
        
        // Recorrer el resto de cam2 hasta fin()
        while (it != cam2.fin()) {
            res.camino.insertar(cam2.elemento(it), res.camino.fin());
            it = cam2.siguiente(it);
        }
    }

    // 3. Añadir cam3 saltando el primer elemento (joya2)
    if (!cam3.vacia()) {
        auto it = cam3.primera();  // cabecera
        it = cam3.siguiente(it);    // primer elemento real (joya2)
        it = cam3.siguiente(it);    // segundo elemento real (saltamos joya2)
        
        // Recorrer el resto de cam3 hasta fin()
        while (it != cam3.fin()) {
            res.camino.insertar(cam3.elemento(it), res.camino.fin());
            it = cam3.siguiente(it);
        }
    }

    return res;
}

// faltaria simular el robo aqui esta el main, usando <TADs/alg_grafoPMC.h>
/*int main() {
    // Leer el grafo
    GrafoP<unsigned int> G(std::cin);
    size_t n = G.numVert();
    
    // Leer número de cámaras
    int c;
    std::cin >> c;
    
    // Leer salas con cámara
    set<size_t> salas_camara;
    for (int i = 0; i < c; i++) {
        size_t sala;
        std::cin >> sala;
        salas_camara.insert(sala);
    }
    
    // Leer joyas, entrada y salida
    size_t j1, j2, entrada, salida;
    std::cin >> j1 >> j2 >> entrada >> salida;
    
    // Construir conjunto de salas restringidas (adyacentes a cámaras)
    set<size_t> salas_restringidas;
    for (size_t sala_cam : salas_camara) {
        for (size_t i = 0; i < n; i++) {
            if (G[sala_cam][i] != GrafoP<unsigned int>::INFINITO) {
                salas_restringidas.insert(i);
            }
            if (G[i][sala_cam] != GrafoP<unsigned int>::INFINITO) {
                salas_restringidas.insert(i);
            }
        }
    }
    
    // Construir nuevo grafo sin las salas prohibidas
    // Primero, mapeamos las salas permitidas a índices compactos
    vector<size_t> mapeo_a_original;
    vector<int> mapeo_a_compacto(n, -1);
    
    for (size_t i = 0; i < n; i++) {
        if (!prohibida(i, salas_camara, salas_restringidas, entrada, salida, j1, j2)) {
            mapeo_a_compacto[i] = mapeo_a_original.size();
            mapeo_a_original.push_back(i);
        }
    }
    
    // Verificar que las salas obligatorias están permitidas
    if (mapeo_a_compacto[entrada] == -1 || 
        mapeo_a_compacto[salida] == -1 ||
        mapeo_a_compacto[j1] == -1 ||
        mapeo_a_compacto[j2] == -1) {
        std::cout << "No es posible realizar el robo" << std::endl;
        return 0;
    }
    
    size_t m = mapeo_a_original.size();
    
    // Crear grafo compacto
    GrafoP<unsigned int> G_compacto(m);
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            if (mapeo_a_compacto[i] != -1 && 
                mapeo_a_compacto[j] != -1 &&
                G[i][j] != GrafoP<unsigned int>::INFINITO) {
                G_compacto[mapeo_a_compacto[i]][mapeo_a_compacto[j]] = G[i][j];
            }
        }
    }
    
    // Mapear las salas importantes
    size_t c_entrada = mapeo_a_compacto[entrada];
    size_t c_salida = mapeo_a_compacto[salida];
    size_t c_j1 = mapeo_a_compacto[j1];
    size_t c_j2 = mapeo_a_compacto[j2];
    
    // Calcular las dos posibles rutas
    Resultado ruta1 = calcular_ruta_orden(G_compacto, c_entrada, c_salida, c_j1, c_j2);
    Resultado ruta2 = calcular_ruta_orden(G_compacto, c_entrada, c_salida, c_j2, c_j1);
    
    // Elegir la mejor
    Resultado mejor;
    
    if (!ruta1.posible && !ruta2.posible) {
        std::cout << "No es posible realizar el robo" << std::endl;
        return 0;
    } else if (!ruta1.posible) {
        mejor = ruta2;
    } else if (!ruta2.posible) {
        mejor = ruta1;
    } else {
        if (ruta1.tiempo < ruta2.tiempo) {
            mejor = ruta1;
        } else {
            mejor = ruta2;
        }
    }
    
    // Mostrar resultado
    std::cout << "Tiempo: " << mejor.tiempo << std::endl;
    std::cout << "Camino: ";
    for (size_t i = 0; i < mejor.camino.size(); i++) {
        std::cout << mapeo_a_original[mejor.camino[i]];
        if (i < mejor.camino.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;
    
    return 0;
}*/

// ejercicio tierra media

typedef GrafoP<unsigned int> GP;
typedef GP::vertice vertice;

bool estanConectados(const GP& G){
    size_t nfuegos = G.numVert();
    size_t ncomponentes = nfuegos;
    Particion P (nfuegos);
    for (size_t i = 0; i < nfuegos; i++) {
        for (size_t j = i + 1; j < nfuegos; j++) {
            if(G[i][j]!=GP::INFINITO){
                size_t ri = P.encontrar(i);
                size_t rj = P.encontrar(j);
                if(ri!=rj){
                    P.unir(ri,rj);
                ncomponentes--;}
            }
        }
    }
    return ncomponentes == 1;
}

unsigned int costeFuegos(GP& Optimo){
    size_t nfuegos = Optimo.numVert();
    unsigned int coste = 0;
    if(estanConectados(Optimo)){
        Optimo = Kruskall(Optimo);
         for (size_t i = 0; i < nfuegos; i++) {
            for (size_t j = i + 1; j < nfuegos; j++) {
               if(Optimo[i][j]!=GP::INFINITO) 
               coste = suma(coste, Optimo[i][j]);   
            }
        }
        return coste;
    }
    return GP::INFINITO;
}

void prueba(){
    GP Oro(std::cin);            // lee N + matriz NxN de oro
    GP Minutos(std::cin);        // lee N (repetido) + matriz NxN de minutos
    GP Optimo = Oro;
    size_t nfuegos = Oro.numVert();
    unsigned int costes = costeFuegos(Optimo);

    if(costes == GP::INFINITO){
        std::cout<< "Gondor ha caido: la red de fuegos no puede unirse"<<std::endl;
        return 0;

    }

    std::cout<<"Coste de construccion: "<<costes<<std::endl;

    for (size_t i = 0; i < nfuegos; i++) {
        for (size_t j = i + 1; j < nfuegos; j++) {
            if(Optimo[i][j] == GP::INFINITO) {
                Minutos[i][j] = GP::INFINITO;
                Minutos[j][i] = GP::INFINITO;  
            }
        }
    }

    vector<vertice> P(nfuegos);
    vector<unsigned int> tiempo = Dijkstra(Minutos, 0, P );
    unsigned int tiempo_aviso = *std::max_element(tiempo.begin(), tiempo.end());
    std::cout<<"Tiempo de aviso: "<<tiempo_aviso<<std::endl;   
}

// Ejercicio 4 la caida de troya
typedef GrafoP<int> GP;
typedef GP::vertice vertice;


int tiempoDeOdisea(GP G, const vector<int>& hospitalidades, vertice origen, vertice destino){
    size_t n = G.numVert();
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            if(j!=origen && j!= destino){
                G[i][j] = suma(G[i][j], hospitalidades[j]);
            }
        }
    }
    vector<vertice> P;
    vector<int> coste = Dijkstra(G, origen, P);
    return coste[destino];
}

void troya2() {
    size_t nislas;
    std::cin >> nislas;
    vector<int> hospitalidades(nislas, 0);
    for(size_t i = 0; i<nislas; i++){
        std::cin >> hospitalidades[i];
        
    }

    GP G(std::cin);              // lee N + matriz N×N
    size_t nviajes;
    std::cin >> nviajes;
    vertice origen, destino;
    for(size_t i = 0; i<nviajes; i++){
        std::cin >> origen>>destino;
        int coste = tiempoDeOdisea(G,hospitalidades,origen,destino);

        if(coste == GP::INFINITO){
            std::cout << "imposible" << std::endl;
        }else{
            std::cout << coste << std::endl;
        }
        
    }
    
}