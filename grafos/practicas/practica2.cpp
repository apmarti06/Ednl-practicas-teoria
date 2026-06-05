#include "grafoPLA.h"
#include "grafoPMC.h"
#include "alg_grafoPMC.h"
#include "listaenla.h"
#include <vector>
#include <utility>

/*1. Tu agencia de viajes “OTRAVEZUNGRAFO S.A.” se enfrenta a un curioso cliente.
Es un personaje sorprendente, no le importa el dinero y quiere hacer el viaje más caro
posible entre las ciudades que ofertas. Su objetivo es gastarse la mayor cantidad de
dinero posible (ojalá todos los clientes fueran así), no le importa el origen ni el destino
del viaje.

Sabiendo que es imposible pasar dos veces por la misma ciudad, ya que casualmente
el grafo de tu agencia de viajes resultó ser acíclico, devolver el coste, origen y destino
de tan curioso viaje. Se parte de la matriz de costes directos entre las ciudades del grafo.*/

// EL grafo inicial es acíclico, devolvemos un struct para devolver el coste, origen y destino del viaje

/*
1. Implementacion usando floyd cambiando los costes del grafo con un menos para usar floyd normal

template <typename tCoste>
Viaje<tCoste> pedro_sanchez(const GrafoP<tCoste>& G){
    Viaje<tCoste> resultado;

    // modificar grafo (donde encuentro peso != INF y distinto a 0, lo pasamos a negativo)
    for (size_t i = 0; i < G.numVert(); i++){
        for (size_t j = 0; i < G.numVert(); j++){
            if (G[i][j] != 0 && G[i][j] != Grafop<tCoste>::INFINITO){
                G[i][j] = -G[i][j];
            }
        }
    }

    // Hacemos floyd cuando hayamos cambiado el orden
    matriz<GrafoP<tCoste>::vertice> P;
    matriz<tCoste> A = Floyd(G, P);

    // Guardamos el origen y destino del viaje
    for (size_t i = 0; i < G.numVert(); i++){
        for (size_t j = 0; i < G.numVert(); j++){
            if (A[i][j] < resultado.c_total)
            resultado.c_total = A[i][j];
            resultado.origen = i;
            resultado.destino = j;
        }
    }

    // actualizamos al valor real del viaje
    resultado.c_total = -resultado.c_total;

    return resultado;
}*/


// Recorreremos el grafo, ya que no pasaremos más de una vez por una ciudad

template <typename tCoste>
struct Viaje {
    tCoste c_total;
    size_t origen, destino;
};

template <typename tCoste>
matriz<tCoste> FloydMax(const GrafoP<tCoste>& G, matriz<typename GrafoP<tCoste>::vertice>& P){
    // hacemos floyd
    typedef typename GrafoP<tCoste>::vertice vertice;
    const size_t n = G.numVert();
    matriz<tCoste> A(n);

    for (vertice v = 0; v < n; v++){
        A[v] = G[i]; // guardamos los costes del grafo
        A[i][i] = 0;
        P[i] = std::vector<vertice>(n, i);
    }

    // Calculamos costes minimos y caminos correspondientes entre pares
    for (vertice k = 0; k < n; k++){
        for (vertice i = 0; i < n; i++){
            for (vertice j = 0; j < n; j++){
                tCoste ijk = suma(A[i][k], A[k][j]);
                if (ijk != GrafoP<tCoste>::INFINITO && (ijk > A[i][j] || A[i][j] == GrafoP<tCoste>::INFINITO)){
                    A[i][j] = ijk;
                    P[i][j] = k;
                }
            }
        }
    }

    return A;
}

// Recorreremos el grafo, ya que no pasaremos más de una vez por una ciudad
template <typename tCoste>
Viaje<tCoste> perro_sanchez(const GrafoP<tCoste>& G){
    // llamamos al metodo creado que dice que el grafo no sea acíclico
    Viaje<tCoste> resultado;

    // Hacemos floydMax en este caso 
    matriz<GrafoP<tCoste>::vertice> P;
    matriz<tCoste> D = FloydMax(G, P);

    // Guardamos el origen y destino del viaje
    for (size_t i = 0; i < G.numVert(); i++){
        for (size_t j = 0; i < G.numVert(); j++){
            if (A[i][j] > resultado.c_total)
            resultado.c_total = A[i][j];
            resultado.origen = i;
            resultado.destino = j;
        }
    }
    
    return resultado;
}

/*
2. Se dispone de un laberinto de NxN casillas del que se conocen las casillas de entrada
y salida del mismo. Si te encuentras en una casilla sólo puedes moverte en las siguientes
cuatro direcciones (arriba, abajo, derecha, izquierda). Por otra parte, entre algunas de las
casillas hay una pared que impide moverse entre las dos casillas que separa dicha pared
(en caso contrario no sería un verdadero laberinto).
Implementa un subprograma que dados
 N (dimensión del laberinto),
 la lista de paredes del laberinto,
 la casilla de entrada, y
 la casilla de salida,
calcule el camino más corto para ir de la entrada a la salida y su longitud. 
*/

// pos del laberinto
struct casilla {
    size_t fil;
    size_t col;
};

// es un camino que impide moverse entre dos casillas
struct pared {
    casilla c1, c2;
};

// funciones conversoras necesarias
size_t casillas_to_nodo(casilla C, size_t N){
    return C.fil * N + C.col;
}

template <typename tCoste>
casilla nodo_to_casillas(typename GrafoP<tCoste>::vertice v, size_t N){
    casilla c;
        c.fil = v / N;
        c.col = v % N;

    return c;
}

// Funciones auxiliares a recorrer (que el movimiento sea adyacente)
bool casillasAdyacentes(casilla& c1, casilla& c2){
    // solo son adyacentes si se mueven por una columna, o fila
    size_t df = c1.fil - c2.fil;
    size_t dc = c1.col - c2.col;

    if ((df == 0 && (dc == 1 || dc == -1)) || (dc == 0 && (df == 1 || df == -1))){
        return true;
    } else {
        return false;
    }
}

// devolvemos en la lista el camino para dar el resultado de la ruta, no usamos grafo, lo creamos aquí con las paredes pasadas

template <typename tCoste>
typename GrafoP<tCoste>::tCamino laberinto1 (size_t N, vector<pared>& paredes, size_t origen, size_t salida){
    // guardamos vectores a usar en dijkstra, vertices y de coste
    typedef GrafoP<tCoste>::vertice vertice;
    GrafoP<size_t> G(N * N); // tamaño n^2 recordamos implementacion, de un laberinto nxn casillas
    
    // rellenamos primero el grafo indicando caminos adyacentes coste 1, (caminos de vertices a vertices)
    for (size_t i = 0; i < G.numVert(); i++){
        for (size_t j = 0; j < G.numVert(); j++){
            if (i != j){
                casilla c1 = nodo_to_casillas(i, N);
                casilla c2 = nodo_to_casillas(j, N);

                // verificamos si son adyacentes, y un laberinto es no dirigido
                if (casillasAdyacentes(c1, c2)){
                    G[i][j] = 1;
                    G[j][i] = 1;
                }

            } else { // los caminos directos a si mismos valen 0
                G[i][j] = 0;
            } 
        }
    }

    // rellenamos ahora las paredes
    for (const pared& p : paredes){
        Casilla c1 = p.c1;
        Casilla c2 = p.c2;

        // convertimos a infinito dicho camino, convirtiendo las paredes en vertices, para modificar el grafo
        Vertice v = casillas_to_nodo(c1, N);
        Vertice w = casillas_to_nodo(c2, N);

        G[v][w] = GrafoP<tCoste>::INFINITO;
        G[w][v] = GrafoP<tCoste>::INFINITO;
    }

    // Aplicamos dijkstra y devolvemos el camino construido
    vector<vertice> P;
    vector<tCoste> D = Dijkstra(G, origen, P);

    // reconstruimos el camino con el coste x (el numero de casillas por las que pasa)
    typename GrafoP<tCoste>::tCamino camino = camino(origen, salida, P);

    return camino;
}

/*3. Eres el orgulloso dueño de una empresa de distribución. Tu misión radica en
distribuir todo tu stock entre las diferentes ciudades en las que tu empresa dispone de
almacén. Tienes un grafo representado mediante la matriz de costes, en el que aparece el coste
(por unidad de producto) de transportar los productos entre las diferentes ciudades del
grafo.

Pero además resulta que los Ayuntamientos de las diferentes ciudades en las que
tienes almacén están muy interesados en que almacenes tus productos en ellas, por lo
que están dispuestos a subvencionarte con un porcentaje de los gastos mínimos de
transporte hasta la ciudad. Para facilitar el problema, consideraremos despreciables los
costes de volver el camión a su base (centro de producción).

He aquí tu problema. Dispones de:
 el centro de producción, nodo origen en el que tienes tu producto (no tiene
almacén),
 una cantidad de unidades de producto (cantidad),
 la matriz de costes del grafo de distribución con N ciudades,
 la capacidad de almacenamiento de cada una de ellas,
 el porcentaje de subvención (sobre los gastos mínimos) que te ofrece cada
Ayuntamiento.

Las diferentes ciudades (almacenes) pueden tener distinta capacidad, y además la
capacidad total puede ser superior a la cantidad disponible de producto, por lo que
debes decidir cuántas unidades de producto almacenas en cada una de las ciudades.

Debes tener en cuenta además las subvenciones que recibirás de los diferentes
Ayuntamientos, las cuales pueden ser distintas en cada uno y estarán entre el 0% y el
100% de los costes mínimos.

La solución del problema debe incluir las cantidades a almacenar en cada ciudad bajo
estas condiciones y el coste mínimo total de la operación de distribución para tu
empresa. 

*/

// Guardamos aquí el coste mínimo total de la operación de distribución
template <typename tCoste>
struct Reparto
{
    tCoste coste_total;
    std::vector<size_t> ud_ciudad;
};

// las subvenciones son porcentajes del 0% al 100%

template <typename tCoste>
Reparto<tCoste> distribucion(size_t origen, size_t cantidad, const GrafoP<tCoste>& G,
std::vector<size_t> almacen_cap, std::vector<float> subvenciones) // los vectores guardan la capacidad de cada ciudad del grafo max, y el porcentaje de subvención
{   
    // obtenemos primero el caminos de coste minimo, para determinar la prioridad de visitas, y luego las subvenciones del ayuntamiento
    Reparto<tCoste> resultado;
    const size_t n = G.numVert();
    std::vector<size_t> P;
    std::vector<tCoste> camino_min = Dijkstra(G, origen, P);

    // alteramos los caminos con las subvenciones para realizar ya los repartos, no tiene sentido repartirse a si mismo
    for (size_t i = 0; i < n; i++){
        if (i != origen && camino_min[i] != GrafoP<tCoste>::INFINITO){ // se puede repartir desde la empresa a otras ciudades
            camino_min[i] *= (1 - subvenciones[i]);
        }
    } 

    // comenzamos a repartir productos que necesiten las ciudades proximas, hasta que nos vaciemos, del minimo al maximo
    while (cantidad > 0){
        size_t destino;
        tCoste minimo = GrafoP<tCoste>::INFINITO;

        for (size_t i = 0; i < n; i++){
            if (camino_min[i] < minimo && camino_min[i] > 0 && almacen_cap[i] > 0){ // tiene que poderse meter productos y que sea el camino mínimo
                destino = i;
                minimo = camino_min[i];
            }
        }
        // la cantidad sera la suficiente a llenar al destino x, o el stock disponibles de la carga
        size_t cant = std::min(almacen_cap[destino], cantidad);

        // actualizamos al haber visitado la ciudad todos los datos
        cantidad -= cant;
        resultado.coste_total += minimo;
        resultado.ud_ciudad[destino] = cant;
        almacen_cap[destino] -= cant;
    }

    return resultado;
}

/*4. Eres el orgulloso dueño de la empresa “Cementos de Zuelandia S.A”. Empresa
dedicada a la fabricación y distribución de cemento, sita en la capital de Zuelandia. Para
la distribución del cemento entre tus diferentes clientes (ciudades de Zuelandia)
dispones de una flota de camiones y de una plantilla de conductores zuelandeses.

El problema a resolver tiene que ver con el carácter del zuelandés. El zuelandés es
una persona que se toma demasiadas “libertades” en su trabajo, de hecho, tienes
fundadas sospechas de que tus conductores utilizan los camiones de la empresa para
usos particulares (es decir indebidos, y a tu costa) por lo que quieres controlar los
kilómetros que recorren tus camiones.

Todos los días se genera el parte de trabajo, en el que se incluyen el número de
cargas de cemento (1 carga = 1 camión lleno de cemento) que debes enviar a cada
cliente (cliente = ciudad de Zuelandia). Es innecesario indicar que no todos los días hay
que enviar cargas a todos los clientes, y además, puedes suponer razonablemente que tu
flota de camiones es capaz de hacer el trabajo diario.

Para la resolución del problema quizá sea interesante recordar que Zuelandia es un
país cuya especial orografía sólo permite que las carreteras tengan un sentido de
circulación.

Implementa una función que dado el grafo con las distancias directas entre las
diferentes ciudades zuelandesas, el parte de trabajo diario, y la capital de Zuelandia,
devuelva la distancia total en kilómetros que deben recorrer tus camiones en el día, para
que puedas descubrir si es cierto o no que usan tus camiones en actividades ajenas a la
empresa. */

// Calculamos la distancia de ida a una ciudad destino y su vuelta, lo que usaremos dijkstra inverso
// las cargas pueden ser 1 o 0
template <typename tCoste>
tCoste CementosZuelandia(const GrafoP<tCoste>& G, size_t capital, std::vector<size_t> cargas)
{
    const size_t n = G.numVert();
    typedef GrafoP<tCoste>::INFINITO INFINITO;
    std::vector<size_t> P_ida, P_vuelta;

    // Camino dijkstra directo-inversa
    std::vector<tCoste> D_ida = Dijkstra(G, capital, P_ida);
    std::vector<tCoste> D_vuelta = Dijkstra_inv(G, capital, P_vuelta);
    
    tCoste distancia_total = 0.0;
    // calculamos todas las distancias de ir y volver a x ciudad
    for (size_t ciudad = 0; ciudad < n; ciudad++){
        if (cargas[ciudad] > 0 && ciudad != capital && (D_ida[ciudad] != INFINITO && 
            D_vuelta[ciudad] != INFINITO)){ // solo calculamos la distancia si se tiene una carga para dicha ciudad + no es ella misma
                tCoste ida_vuelta = suma(D_ida[ciudad], D_ida[ciudad]);
                distancia_total += ida_vuelta;
        }
    }
    return distancia_total;
}

/*Nota Importante:

A partir del problema 5 (el viajero alérgico), empiezan a aparecer en los enunciados
el uso de diferentes medios de transporte a la hora de realizar un viaje. En nuestros
problemas (tanto en prácticas como en exámenes) asumiremos que
a) Definición de trasbordo : En el contexto de los problemas de la asignatura,
consideraremos trasbordo el cambio de medio de transporte.
b) Trasbordos libres y gratuitos por defecto: Si el enunciado del problema no
indica lo contrario los trasbordos en nuestros problemas son libres y gratuitos. 
*/

/*5. Se dispone de tres grafos que representan la matriz de costes para viajes en un
determinado país pero por diferentes medios de transporte, por supuesto todos los grafos
tendrán el mismo número de nodos. El primer grafo representa los costes de ir por
carretera, el segundo en tren y el tercero en avión. Dado un viajero que dispone de una
determinada cantidad de dinero, que es alérgico a uno de los tres medios de transporte, y
que sale de una ciudad determinada, implementar un subprograma que determine las
ciudades a las que podría llegar nuestro infatigable viajero. */

typedef enum {CARRETERA, TREN, AVION} alergia;

// todos los grafos tienen el mismo numero de vertices con distintos costes
template <typename tCoste>
std::vector<size_t> viaje_por_pais (const GrafoP<tCoste>& G_carretera, const GrafoP<tCoste>& G_tren,
     const GrafoP<tCoste>& G_avion, alergia guerrero, tCoste plata_disp, typename GrafoP<tCoste>::vertice origen)
{       
    typedef GrafoP<tCoste>::vertice vertice;
    std::vector<vertice> P1, P2, res;

    // Buscamos los caminos que son aptos para mi guerrero
    switch (guerrero){
        case CARRETERA: // alergico a la carretera
            std::vector<tCoste> D1 = Dijkstra(G_tren, origen, P1);
            std::vector<tCoste> D2 = Dijkstra(G_avion, origen, P2);


            // insertamos todos los vertices que mi niño pueda pagarse 
            for (size_t i = 0; i < G_tren.numVert; i++){
                if (D1[i] < plata_disp && ){
                    res.push_back(i);
                }
            }

            for (size_t i = 0; i < G_avion.numVert; i++){
                if (D1[i] < plata_disp && i != origen){
                    res.push_back(i);
                }
            }

        break;

        case TREN: // alergico al tren
            std::vector<tCoste> D1 = Dijkstra(G_carretera, origen, P1);
            std::vector<tCoste> D2 = Dijkstra(G_avion, origen, P2);

            // insertamos todos los vertices que mi niño pueda pagarse 
            for (size_t i = 0; i < G_carretera.numVert; i++){
                if (D1[i] < plata_disp && ){
                    res.push_back(i);
                }
            }

            for (size_t i = 0; i < G_avion.numVert; i++){
                if (D1[i] < plata_disp && i != origen){
                    res.push_back(i);
                }
            }
            
        break;

        case AVION: // alergico al avion
            std::vector<tCoste> D1 = Dijkstra(G_carretera, origen, P1);
            std::vector<tCoste> D2 = Dijkstra(G_tren, origen, P2);


            // insertamos todos los vertices que mi niño pueda pagarse 
            for (size_t i = 0; i < G_carretera.numVert; i++){
                if (D1[i] < plata_disp && ){
                    res.push_back(i);
                }
            }

            for (size_t i = 0; i < G_tren.numVert; i++){
                if (D1[i] < plata_disp && i != origen){
                    res.push_back(i);
                }
            }
            
        break;
        
        default:
            break;
    }

    return res;
}

/*6. Al dueño de una agencia de transportes se le plantea la siguiente situación. La
agencia de viajes ofrece distintas trayectorias combinadas entre N ciudades españolas
utilizando tren y autobús. Se dispone de dos grafos que representan los costes (matriz de
costes) de viajar entre diferentes ciudades, por un lado en tren, y por otro en autobús
(por supuesto entre las ciudades que tengan línea directa entre ellas). Además coincide
que los taxis de toda España se encuentran en estos momentos en huelga general, lo que
implica que sólo se podrá cambiar de transporte en una ciudad determinada en la que,
por casualidad, las estaciones de tren y autobús están unidas.

Implementa una función que calcule la tarifa mínima (matriz de costes mínimos) de
viajar entre cualesquiera de las N ciudades disponiendo del grafo de costes en autobús,
del grafo de costes en tren, y de la ciudad que tiene las estaciones unidas. */

// Usamos floyd, para tener todo el mapa de costes minimo de todos los vertices, y disponemos de cambio,
//para obtener el grafo definitivo para obtener las rutas con menores costes usando en la ciudad, los transbordos

template<typename tCoste>
matriz<tCoste> transbordos1(const GrafoP<tCoste>& bus, const GrafoP<tCoste>& tren, size_t cambio){
    // creamos todas las matrices y hacemos floyd
    matriz<tCoste> resultado(bus.numVert());
    matriz<size_t> P1, P2;
    const tCoste INF = GrafoP<tCoste>::INFINITO;

    matriz<tCoste> A1 = Floyd(bus, P1);
    matriz<tCoste> A2 = Floyd(tren, P2);

    // creamos nuestro grafo definitivo
    for (size_t i = 0; i  < resultado.dimension(); i++){
        for (size_t j = 0; j < resultado.dimension(); j++){
            if (i != j){ // siempre sera coste 0 para A[i][i]
                tCoste coste_min = INF;

                // buscamos el  costemin, de los dos grafos
                if (A1[i][j] < coste_min){
                    coste_min = A1[i][j];
                }

                if (A2[i][j] < coste_min){
                    coste_min = A2[i][j];
                }
                // si estamos en la ciudad donde se hace transbordo (viendo ambos casos), donde podemos hacer uso de un bus a tren
                if (suma(A1[i][cambio], A2[cambio][j]) != INFINITO && 
                suma(A1[i][cambio], A2[cambio][j]) < coste_min){
                    coste_min = suma(A1[i][cambio], A2[cambio][j]);
                }

                if (suma(A2[i][cambio], A1[cambio][j]) != INFINITO && 
                suma(A2[i][cambio], A1[cambio][j]) < coste_min){
                    coste_min = suma(A2[i][cambio], A1[cambio][j]);
                }

                // si no se modifica el coste minimo al no haber camino ha de ser infinito, 
                //donde es una ciudad invisitable desde el vertice i
                resultado[i][j] = coste_min;

            } else {
                resultado[i][j] = 0;
            }
        }
    }
    return resultado;
}

/*
Ejercicio 7: Se dispone de dos grafos (matriz de costes) que representan los costes de viajar entre
N ciudades españolas utilizando el tren (primer grafo) y el autobús (segundo grafo).
Ambos grafos representan viajes entre las mismas N ciudades.
Nuestro objetivo es hallar el camino de coste mínimo para viajar entre dos ciudades
concretas del grafo, origen y destino, en las siguientes condiciones:

- La ciudad origen sólo dispone de transporte por tren.
- La ciudad destino sólo dispone de transporte por autobús.
- El sector del taxi, bastante conflictivo en nuestros problemas, sigue en huelga,
por lo que únicamente es posible cambiar de transporte en dos ciudades del
grafo, cambio1 y cambio2, donde las estaciones de tren y autobús están
unidas. 

Cambios son ciudades!!

Implementa un subprograma que calcule la ruta y el coste mínimo para viajar entre
las ciudades Origen y Destino en estas condiciones. */

// Implementamos la reconstruccion de caminos para usar dijkstra
template <typename tCoste>
typename GrafoP<tCoste>::tCamino reconstruirCamino(
    size_t origen,
    size_t destino,
    const vector<size_t>& P)
{
    typename GrafoP<tCoste>::tCamino C;
    C.insertar(destino, C.primera());

    do {
        C.insertar(P[destino], C.primera());
        destino = P[destino];
    } while (v != orig);

    return C;
}

template <typename tCoste>
void reconstruirCaminoInvertida(
    typename GrafoP<tCoste>::tCamino& C,
    size_t origen,
    size_t destino,
    const vector<size_t>& P)
{
    C.insertar(destino, C.fin());

    do {
        C.insertar(P[destino], C.fin());
        destino = P[destino];
    } while (v != orig);

    return C;
}

template <typename tCoste>
std::pair<typename GrafoP<tCoste>::tCamino, tCoste> transbordos2(const GrafoP<tCoste>& bus, const GrafoP<tCoste>& tren,
size_t origen, size_t destino, size_t cambio1, size_t cambio2)
{   
    // usando pair devolveremos el resultado
    vector<size_t> bus_vuelta_P, tren_ida_P;

    // hacemos la busqueda de caminos desde el origen para su destino para trazar su camino, respetando precondiciones
    vector<tCoste> tren_ida_D = Dijkstra(tren, origen, tren_ida_P);
    vector<tCoste> bus_vuelta_D = Dijkstra_inv(bus, destino, bus_vuelta_P);

    // trazamos el camino guardando dicha ruta en un vertice y su coste
    tCoste minimo = GrafoP<tCoste>::INFINITO;
    size_t mejor_cambio;

    // vemos las 2 rutas disponibles, no podemos aplicar en mas de una ciudad el cambio
    // tren-tren->bus (cada flecha un cambio)
    // tren->bus-bus (cada flecha un cambio)

    // Buscamos cual es el mejor cambio de todas las posibilidades
    if (suma(tren_ida_D[cambio1], bus_vuelta_D[cambio1]) < minimo){
        minimo = suma(tren_ida_D[cambio1], bus_vuelta_D[cambio1]);
        mejor_cambio = cambio1;
    }

    if (suma(tren_ida_D[cambio2], bus_vuelta_D[cambio2]) < minimo){
        minimo = suma(tren_ida_D[cambio2], bus_vuelta_D[cambio2]);
        mejor_cambio = cambio2;
    }

    // Si no hay camino posible
    if (minimo == GrafoP<tCoste>::INFINITO) {
        return {ruta_final, minimo};
    }

    // construimos la ruta
    typename GrafoP<tCoste>::tCamino ruta_final = reconstruirCamino(origen, mejor_cambio, tren_ida_P); // son lista de vertices
    reconstruirCaminoInvertida(ruta_final, mejor_cambio, destino, bus_vuelta_P);
    ruta_final.insertar(origen, ruta_final.primera());

    return {ruta_final, minimo};
}

/*Ejercicio 8: “UN SOLO TRANSBORDO, POR FAVOR”. Este es el título que reza en tu
flamante compañía de viajes. Tu publicidad explica, por supuesto, que ofreces viajes
combinados de TREN y/o AUTOBÚS (es decir, viajes en tren, en autobús, o usando
ambos), entre N ciudades del país, que ofreces un servicio inmejorable, precios muy
competitivos, y que garantizas ante notario algo que no ofrece ninguno de tus
competidores: que en todos tus viajes COMO MÁXIMO se hará un solo transbordo
(cambio de medio de transporte).

Bien, hoy es 1 de Julio y comienza la temporada de viajes.
¡Qué suerte! Acaba de aparecer un cliente en tu oficina. Te explica que quiere viajar
entre dos ciudades, Origen y Destino, y quiere saber cuánto le costará.
Para responder a esa pregunta dispones de dos grafos de costes directos (matriz de
costes) de viajar entre las N ciudades del país, un grafo con los costes de viajar en tren y
otro en autobús.

Implementa un subprograma que calcule la tarifa mínima en estas condiciones.
Mucha suerte en el negocio, que la competencia es dura*/

template <typename tCoste>
tCoste ejercicio8 (const GrafoP<tCoste>& tren,
    const GrafoP<tCoste>& bus, 
    size_t origen, size_t destino)
    {
        const size_t n_tren = tren.numVert();
        const size_t n_bus = bus.numVert();

        // Creamos los vectores de los costes minimos y obtenemos caminos
        vector<size_t> salBus, lleBus, salTren, lleTren;
        vector<tCoste> c_salBus, c_lleBus, c_salTren ,c_lleTren;

        // Caminos desde el origen
        c_salBus = Dijkstra(bus, origen, salBus);
        c_salTren = Dijkstra(bus, origen, salTren);

        c_lleBus = Dijkstra_inv(bus, destino, lleBus);
        c_lleTren = Dijkstra_inv(bus, destino, lleTren);

        // Vemos cual es el coste menor de ir al destino en bus o tren, de primeras
        tCoste minimo = std::min(c_salBus[destino], c_salTren[destino]);

        // Viendo los transbordos vemos si haciendo transbordo en una ciudad, se supera al coste minimo anterior
        for (size_t cambio = 0; cambio < n_tren; cambio++){
            minimo = std::min(suma(c_salBus[cambio], c_lleTren[cambio]), suma(c_salTren[cambio], c_lleBus[cambio]), minimo);
        }

        return minimo;
    }


/*9. Se dispone de dos grafos que representan la matriz de costes para viajes en un
determinado país, pero por diferentes medios de transporte (tren y autobús, por
ejemplo). Por supuesto ambos grafos tendrán el mismo número de nodos, N. Dados
ambos grafos, una ciudad de origen, una ciudad de destino y el coste del taxi para
cambiar de una estación a otra dentro de cualquier ciudad (se supone constante e igual
para todas las ciudades), implementa un subprograma que calcule el camino y el coste
mínimo para ir de la ciudad origen a la ciudad destino. */

/* bus_bus(1er)             bus_tren(2do)
   --------------/---------------
   /             /              /
   /             /              /
   --------------/---------------
   /             /              /
   /             /              /
   --------------/---------------
tren_bus(3ro)                tren_tren(4to)                   
*/

// Definimos los 4 cuadrantes que sera el coste de usar dichos medios
enum Cuadrante {primer, segundo, tercer, cuarto};

// Viendo en que coordenada de la ciudad estoy(matriz de costes), donde N es el numero de ciudades
Cuadrante Cuadrante_actual(size_t i, size_t j, size_t N){
    if (i <= N - 1){

        if (j <= N -1){
            return Cuadrante::primer;
        } else {
            return Cuadrante::segundo;
        }

    } else {

        if (j > N - 1){
            return Cuadrante::cuarto;
        } else {
            return Cuadrante::tercer;
        }

    }
}

template <typename tCoste>
void rellenarGrafo(GrafoP<tCoste>& fusion, const GrafoP<tCoste>& bus, 
const GrafoP<tCoste> &tren, tCoste coste_taxi, size_t N)
{
    for (size_t i = 0; i < 2*N; i++){  
        for (size_t j = 0; j < 2*N; j++){   
            // recordamos que solo podemos hacer transbordos en aquellas ciudades que posean ambos servicios
            Cuadrante actual = Cuadrante_actual(i, j, N);
            
            switch (actual) {
                case primer:   // Bus → Bus
                    fusion[i][j] = bus[i][j];
                    break;
                    
                case segundo:  // Bus → Tren (transbordo con taxi)
                    if ((i % N) == (j % N)) {  // Misma ciudad real
                        fusion[i][j] = coste_taxi;
                    } else {
                        fusion[i][j] = GrafoP<tCoste>::INFINITO;  // No se puede
                    }
                    break;
                    
                case tercer:   // Tren → Tren
                    fusion[i][j] = tren[i - N][j - N];  // Ajustar índices, que correspondan al grafo tren
                    break;
                    
                case cuarto:   // Tren → Bus (transbordo con taxi)
                    if ((i % N) == (j % N)) {  // Misma ciudad real
                        fusion[i][j] = coste_taxi;
                    } else {
                        fusion[i][j] = GrafoP<tCoste>::INFINITO;
                    }
                    break;
            }
        }
    }
}


template <typename tCoste>
std::pair<tCoste, typename GrafoP<tCoste>::tCamino> transbordos3 
(const GrafoP<tCoste>& bus, const GrafoP<tCoste>& tren,
size_t origen_bus, size_t destino_bus, tCoste coste_taxi)
{   
    // Actualizamos el grafo fusionado, y añadimos teniendo en cuenta la sintaxi del grafo, los origen-destino
    size_t n = bus.numVert();
    size_t origen_tren = origen_bus + n;
    size_t destino_tren = destino_bus + n;
    GrafoP<tCoste> res(2*n);
    rellenarGrafo(res, bus, tren, coste_taxi, res.numVert());

    // Calculamos los costes minimos de dichos grafos
    std::vector<size_t> P_bus, P_tren;
    std::vector<tCoste> desde_bus = Dijkstra(res, origen_bus, P_bus), 
    std::vector<tCoste> desde_tren = Dijkstra(res, origen_tren, P_tren);

    // Calculamos el coste minimo en ir desde bus y desde tren, incluyendo el haber hecho transbordo desde_transporte_!=[destino_en_transporte_!=]
    tCoste coste_min = std::min({desde_bus[destino_bus], desde_bus[destino_tren], 
        desde_tren[destino_bus], desde_tren[destino_tren]});
    
    // Guardamos el camino deseado, que es lo que nos queda
    typename GrafoP<tCoste>::tCamino camino_min;

        // caso solo vayamos en bus
    if (coste_min == desde_bus[destino_bus]){
        camino_min = camino<tCoste>(origen_bus, destino_bus, P_bus);

        // caso de que hayamos usado un transbordo para llegar al destino en tren
    } else if (coste_min == desde_bus[destino_tren]){
        camino_min = camino<tCoste>(origen_bus, destino_tren, P_bus);

        // caso solo vayamos en tren
    } else if (coste_min == desde_tren[destino_bus]){
        camino_min = camino<tCoste>(origen_tren, destino_tren, P_tren);

        // caso de que hayamos usado un transbordo para llegar al destino en tren
    } else {
        camino_min = camino<tCoste>(origen_tren, destino_bus, P_tren);
    }

    return {coste_min, camino_min};
}

/*10. Se dispone de tres grafos que representan la matriz de costes para viajes en un
determinado país, pero por diferentes medios de transporte (tren, autobús y avión). Por
supuesto los tres grafos tendrán el mismo número de nodos, N.

Dados los siguientes datos:
 los tres grafos,
 una ciudad de origen,
 una ciudad de destino,
 el coste del taxi para cambiar, dentro de una ciudad, de la estación de tren a la
de autobús o viceversa (taxi-tren-bus) y el coste del taxi desde el aeropuerto a la estación de 
tren o la de autobús, o viceversa (taxi-aeropuerto-tren/bus).
Asumiendo que ambos costes de taxi (distintos entre sí, son dos costes diferentes) son
constantes e iguales para todas las ciudades, implementa un subprograma que calcule el
camino y el coste mínimo para ir de la ciudad origen a la ciudad destino. */

/*            BUS           TREN            AVION
        --------------/--------------/---------------/
        /             /              /               /
    B   /             /              /               /
        --------------/--------------/---------------/
        /             /              /               /
    T   /             /              /               /
        --------------/--------------/---------------/
        /             /              /               /
    A   /             /              /               /   
        --------------/--------------/---------------/
   
   
           
*/

// Definimos los 9, ya que poseemos 3 grafos y calculamos el coste de usar cada medio 
enum Cuadrante_2 {primer, segundo, tercer, cuarto, quinto, sexto, septimo, octavo, noveno};

// Viendo en que coordenada de la ciudad estoy(matriz de costes), donde N es el numero de ciudades
Cuadrante_2 Cuadrante_actual2(size_t i, size_t j, size_t N){
    if (i <= N - 1){

        if (j <= N - 1){
            return Cuadrante_2::primer;
        } else if ( N <= j && j <= 2*N - 1){
            return Cuadrante_2::segundo;
        } else {
            return Cuadrante_2::tercer;
        }

    } else if (i <= 2*N - 1){

        if (j <= N - 1){
            return Cuadrante_2::cuarto;
        } else if ( N <= j && j <= 2*N - 1){
            return Cuadrante_2::quinto;
        } else {
            return Cuadrante_2::sexto;
        }

    } else {
        
        if (j <= N - 1){
            return Cuadrante_2::septimo;
        } else if ( N <= j && j <= 2*N - 1){
            return Cuadrante_2::octavo;
        } else {
            return Cuadrante_2::noveno;
        }
    }
}

// Los costes entre tren_bus y avion a bus o tren son distintos por lo que creamos distintos costes
template <typename tCoste>
void rellenar_grafo2(GrafoP<tCoste>& fusion, const GrafoP<tCoste>& bus,
const GrafoP<tCoste>& tren, const GrafoP<tCoste>& avion, size_t N, 
tCoste taxi_tren_bus, tCoste taxi_avion)
{
    for (size_t i = 0; i < 3 * N; i++){
        for (size_t j = 0; j < 3 * N; j++){
            switch (Cuadrante_actual2(i, j, N))
            {
                case primer:
                    fusion[i][j] = bus[i][j];
                    break;

                // el segundo como el 4to tiene el mismo coste de hacer el transbordo taxi-> bus_tren
                case segundo:
                    case cuarto:
                        // solo ponemos casillas transbordo las diagonales
                        if (i % N == j % N){
                            fusion[i][j] = taxi_tren_bus;
                        } else {
                        fusion[i][j] = GrafoP<tCoste>::INFINITO;  // No se puede
                        }

                        break;
                    
                // Vemos los transbordo que son el mismo coste de ir entre el areopuerto y un bus o tren
                case tercer:
                case sexto:
                case septimo:
                case octavo:
                        // solo ponemos casillas transbordo las diagonales
                        if (i % N == j % N){
                            fusion[i][j] = taxi_avion;
                        } else {
                        fusion[i][j] = GrafoP<tCoste>::INFINITO;  // No se puede
                        }
                        
                
                // Costes reales del grafo del tren y del avion
                case quinto:
                    fusion[i][j] = tren[i][j];
                    break;

                case novena:
                    fusion[i][j] = avion[i][j];
                    break;
            }
        }
    }
}

template <typename tCoste>
std::pair<tCoste, typename GrafoP<tCoste>::tcamino> transbordo4 
(const GrafoP<tCoste>& bus,const GrafoP<tCoste>& tren, const GrafoP<tCoste>& avion,
size_t ciudad_O, size_t ciudad_D, tCoste taxi_tren_bus, tCoste taxi_avion)
{   
    // Recordamos que los 3 grafos son iguales, y montamos el fusion
    size_t n =  bus.numVert();
    GrafoP<tCoste> resultado(3*n);
    rellenar_grafo2(resultado, bus, tren, avion, n, taxi_tren_bus, taxi_avion);

    // Montamos las ciudades para el grafo creado
    typedef size_t ciudad;
    ciudad bus_origen = ciudad_O, tren_origen = ciudad_O + N, avion_origen = ciudad_O + 2*N;
    ciudad bus_destino = ciudad_D, tren_destino = ciudad_D + N, avion_destino = ciudad_D + 2*N;    

    // Calculamos los caminos de buses, tren y aviones
    std::vector<ciudad> ciudades_desde_bus, ciudades_desde_tren, ciudades_desde_avion;
    std::vector<tCoste> desde_bus = Dijkstra(fusion, bus_origen, ciudades_desde_bus);
    std::vector<tCoste> desde_tren = Dijkstra(fusion, tren_origen, ciudades_desde_tren);
    std::vector<tCoste> desde_avion = Dijkstra(fusion, avion_origen, ciudades_desde_avion);

    // Estructura para guardar la mejor opcion
    struct Opcion {
        tCoste coste;
        ciudad origen;
        ciudad destino;
        std::vector<ciudad> camino;
    };
    
    // Vector con todas las combinaciones posibles, 
    std::vector<Opcion> opciones = {
        {desde_bus[bus_destino], bus_origen, bus_destino, ciudades_desde_bus},
        {desde_bus[tren_destino], bus_origen, tren_destino, ciudades_desde_bus},
        {desde_bus[avion_destino], bus_origen, avion_destino, ciudades_desde_bus},
        {desde_tren[bus_destino], tren_origen, bus_destino, ciudades_desde_tren},
        {desde_tren[tren_destino], tren_origen, tren_destino, ciudades_desde_tren},
        {desde_tren[avion_destino], tren_origen, avion_destino, ciudades_desde_tren},
        {desde_avion[bus_destino], avion_origen, bus_destino, ciudades_desde_avion},
        {desde_avion[tren_destino], avion_origen, tren_destino, ciudades_desde_avion},
        {desde_avion[avion_destino], avion_origen, avion_destino, ciudades_desde_avion}
    };
    
    // Buscar la opción con menor coste
    tCoste coste_min = typename GrafoP<tCoste>::INFINITO;
    size_t indice_mejor;
    
    // Elegimos de forma mas elegante
    for (size_t i = 0; i < opciones.size(); ++i) {
        if (opciones[i].coste < coste_min) {
            coste_min = opciones[i].coste;
            indice_mejor = i;
        }
    }
    
    // Construir el camino mínimo
    typename GrafoP<tCoste>::tcamino camino_min = camino<tCoste>(
        opciones[indice_mejor].origen, 
        opciones[indice_mejor].destino, 
        opciones[indice_mejor].camino
    );
    
    // retornamos el par de valores
    return sdt::make_pair(coste_min, camino_min);
}

/*11. Disponemos de tres grafos (matriz de costes) que representan los costes directos de
viajar entre las ciudades de tres de las islas del archipiélago de las Huríes (Zuelandia).
Para poder viajar de una isla a otra se dispone de una serie de puentes que conectan
ciudades de las diferentes islas a un precio francamente asequible (por decisión del
Prefecto de las Huríes, el uso de los puentes es absolutamente gratuito).
Si el alumno desea simplificar el problema, puede numerar las N1 ciudades de la isla
1, del 0 al N1-1, las N2 ciudades de la isla 2, del N1 al N1+N2-1, y las N3 de la última, del
N1+ N2 al N1+N2+ N3-1.

Disponiendo de las tres matrices de costes directos de viajar dentro de cada una de
las islas, y la lista de puentes entre ciudades de las mismas, calculad los costes mínimos
de viajar entre cualesquiera dos ciudades de estas tres islas.
¡¡¡ QUE DISFRUTÉIS EL VIAJE !!! */

typedef std::pair<size_t, size_t> coordenadas; // implicitamente guarda la pos
// de un puente al colocarlo desde una ciudad v y otra ciudad w de islas distintas

template <typename tCoste>
GrafoP<tCoste> multigrafo (const GrafoP<tCoste>& isla1, const GrafoP<tCoste>& isla2,
const GrafoP<tCoste>& isla3)
{   
    // Creamos nuestro multigrafo que disponga todas las islas
    size_t n1 = isla1.numVert(), n2 = isla2.numVert(), n3 = isla3.numVert();
    size_t n_total = n1 + n2 + n3;
    GrafoP<tCoste> resultado(n_total);

    // Actualizamos todos los grafos
    for (size_t i = 0; i < n1; i++){
        for (size_t j = 0; j < n1; j++){
            resultado[i][j] = isla1[i][j];
        }
    }

    for (size_t i = n1; i < n1 + n2; i++){
        for (size_t j = n1; j < n1 + n2; j++){
            resultado[i][j] = isla2[i - n1][j - n1];
        }
    }

    for (size_t i = n1 + n2; i < n_total; i++){
        for (size_t j = n1 + n2; j < n_total; j++){
            resultado[i][j] = isla3[i - n1 - n2][j - n1 - n2];
        }
    }
    return resultado;
}

// REALMENTE LO UNICO QUE NOS INTERESA EL COSTE DE FLOYD
template <typename tCoste>
std::pair<matriz<tCoste>, matriz<size_t>> viajar_Huríes(const GrafoP<tCoste>& isla1, const GrafoP<tCoste>& isla2,
const GrafoP<tCoste>& isla3, const std::vector<coordenadas>& puentes)
{
    // Creamos el multigrafo
    GrafoP<tCoste> G = multigrafo(isla1, isla2, isla3);

    // Creamos nuestros puentes, donde el coste de usarlo es gratuito
    for (const coordenadas& puente : puentes){
        G[puente.first][puente.second] = G[puente.second][puente.first] = 0;
    }

    // Devolvemos una matriz que nos diga todo los costes de ir de un sitio a otro
    matriz<size_t> P;
    matriz<tCoste> coste_ciudades = Floyd(G, P);

    return std::make_pair(coste_ciudades, P);
}

/*12. El archipiélago de Grecoland (Zuelandia) está formado únicamente por dos islas,
Fobos y Deimos, que tienen N1 y N2 ciudades, respectivamente, de las cuales C1 y C2
ciudades son costeras (obviamente C1 ≤ N1 y C2 ≤ N2). Se desea construir un puente
que una ambas islas. Nuestro problema es elegir el puente a construir entre todos los
posibles, sabiendo que el coste de construcción del puente se considera irrelevante. Por
tanto, escogeremos aquel puente que minimice el coste global de viajar entre todas las
ciudades de las dos islas, teniendo en cuenta las siguientes premisas:

1. Se asume que el coste viajar entre las dos ciudades que una el puente es 0.
2. Para poder plantearse las mejoras en el transporte que implica la construcción de
un puente frente a cualquier otro, se asume que se realizarán exactamente el
mismo número de viajes entre cualesquiera ciudades del archipiélago. Por
ejemplo, se considerará que el número de viajes entre la ciudad P de Fobos y la
Q de Deimos será el mismo que entre las ciudades R y S de la misma isla. Dicho
de otra forma, todos los posibles trayectos a realizar dentro del archipiélago son
igual de importantes.

Dadas las matrices de costes directos de Fobos y Deimos y las listas de ciudades
costeras de ambas islas, implementa un subprograma que calcule las dos ciudades que
unirá el puente. */

template <typename tCoste>
size_t ciudad_coste_min (const GrafoP<tCoste>& isla, const std::vector<size_t>& ciudades){
    tCoste coste_minimo = typename GrafoP<tCoste>::INFINITO;
    size_t ciudad_minimizadora;

    // Hacemos dijkstra por cada ciudad costera de la isla
    for (size_t i = 0; i < ciudades.size(); i++){
        std::vector<size_t> P;
        std::vector<tCoste> distancias = Dijkstra(isla, ciudades[i], P);

        // Calculamos el coste total a todas sus ciudades cercanas a P de la isla X
        tCoste total = 0;
        for (const size_t& ciudad : ciudades){
            total = suma(total, distancias[ciudad]);
        }

        // Probamos si es el nuevo coste minimo
        if (total < coste_minimo){
            coste_minimo = total;
            ciudad_minimizadora = ciudades[i];
        }
    }
    return ciudad_minimizadora;
}

template <typename tCoste>
std::pair<size_t, size_t> ciudades_puente (const GrafoP<tCoste>& Fobos, 
    const GrafoP<tCoste>& Deimos, const std::vector<size_t>& ciudades_costeras_F,
    const std::vector<size_t>& ciudades_costeras_D)
{   
    // si no hubiera los grafos fobos y demos, tendriamos que construir el grafo G 
    // apartir de la matriz de costes y las ciudades costeras de G
    size_t mejor_opcion_F = ciudad_coste_min(Fobos, ciudades_costeras_F);
    size_t mejor_opcion_D = ciudad_coste_min(Deimos, ciudades_costeras_D);

    return std::make_pair(mejor_opcion_F, mejor_opcion_D);
}

/*13. El archipiélago de las Huríes acaba de ser devastado por un maremoto de
dimensiones desconocidas hasta la fecha. La primera consecuencia ha sido que todos y
cada uno de los puentes que unían las diferentes ciudades de las tres islas han sido
destruidos. En misión de urgencia las Naciones Unidas han decidido construir el
mínimo número de puentes que permitan unir las tres islas. Asumiendo que el coste de
construcción de los puentes implicados los pagará la ONU, por lo que se considera
irrelevante, nuestro problema es decidir qué puentes deben construirse. Las tres islas de
las Huríes tienen respectivamente N1, N2 y N3 ciudades, de las cuales C1, C2 y C3 son
costeras (obviamente C1 ≤ N1 , C2 ≤ N2 y C3 ≤ N3) . Nuestro problema es elegir los
puentes a construir entre todos los posibles. Por tanto, escogeremos aquellos puentes
que minimicen el coste global de viajar entre todas las ciudades de las tres islas,
teniendo en cuenta las siguientes premisas:

1. Se asume que el coste viajar entre las ciudades que unan los puentes es 0.
2. La ONU subvencionará únicamente el número mínimo de puentes necesario
para comunicar las tres islas.
3. Para poder plantearse las mejoras en el transporte que implica la construcción de
un puente frente a cualquier otro, se asume que se realizarán exactamente el
mismo número de viajes entre cualesquiera ciudades del archipélago. Dicho de
otra forma, todos los posibles trayectos a realizar dentro del archipiélago son
igual de importantes.

Dadas las matrices de costes directos de las tres islas y las listas de ciudades costeras
del archipiélago, implementad un subprograma que calcule los puente mejor ces a construir en las
condiciones anteriormente descritas. */


// Dos opciones:
struct Puente {
    size_t c1, c2;
};

//1. Más correcta, no sabemos donde es colocar puentes isla 2, isla 1, o isla 3
template <typename tCoste>
std::pair<Puente, Puente> mejores_puentes(
    const GrafoP<tCoste>& G1, const GrafoP<tCoste>& G2,
    const GrafoP<tCoste>& G3, 
    const std::vector<size_t>& costeras_1, 
    const std::vector<size_t>& costeras_2, 
    const std::vector<size_t>& costeras_3)
{
    // 1. Precomputamos distancias mínimas dentro de cada isla
    size_t n1 = G1.numVert(), n2 = G2.numVert(), n3 = G3.numVert();
    const tCoste INF = GrafoP<tCoste>::INFINITO;
    
    // como no nos hace falta la matriz de vertices la descartamos
    matriz<size_t> caminos;
    matriz<tCoste> dist1 = Floyd(G1, caminos);  // O(n1³)
    matriz<tCoste> dist2 = Floyd(G2, caminos);  // O(n2³)
    matriz<tCoste> dist3 = Floyd(G3, caminos);  // O(n3³)
    
    // Para cada ciudad costera, calculamos la suma de distancias a TODAS las ciudades de su isla
    std::vector<tCoste> suma_costera1(costeras_1.size());
    std::vector<tCoste> suma_costera2(costeras_2.size());
    std::vector<tCoste> suma_costera3(costeras_3.size());
    
    // Isla 1
    for (size_t idx = 0; idx < costeras_1.size(); idx++) {
        size_t c = costeras_1[idx];
        tCoste suma = 0;
        for (size_t i = 0; i < n1; i++) {
            if (dist1[c][i] != INF) {
                suma = suma(suma, dist1[c][i]);
            }
        }
        suma_costera1[idx] = suma;
    }
    
    // Isla 2
    for (size_t idx = 0; idx < costeras_2.size(); idx++) {
        size_t c = costeras_2[idx];
        tCoste suma = 0;
        for (size_t i = 0; i < n2; i++) {
            if (dist2[c][i] != INF) {
                suma = suma(suma, dist2[c][i]);
            }
        }
        suma_costera2[idx] = suma;
    }
    
    // Isla 3
    for (size_t idx = 0; idx < costeras_3.size(); idx++) {
        size_t c = costeras_3[idx];
        tCoste suma = 0;
        for (size_t i = 0; i < n3; i++) {
            if (dist3[c][i] != INF) {
                suma = suma(suma, dist3[c][i]);
            }
        }
        suma_costera3[idx] = suma;
    }
    
    // Calculamos la mejor conexión para cada par de islas
    // El coste de conectar dos islas mediante puentes en ciudades a y b es:
    // coste_conexion = suma_desde_a + suma_desde_b
    
    struct Conexion {
        tCoste coste;
        size_t c1, c2;     // ciudades originales
    };
    
    // Conexión Isla1-Isla2
    Conexion mejor_12 = {INF, 0, 0};
    for (size_t i = 0; i < costeras_1.size(); i++) {
        for (size_t j = 0; j < costeras_2.size(); j++) {
            tCoste coste_conexion = suma(suma_costera1[i], suma_costera2[j]);
            if (coste_conexion < mejor_12.coste) {
                mejor_12 = {coste_conexion, costeras_1[i], costeras_2[j]};
            }
        }
    }
    
    // Conexión Isla1-Isla3
    Conexion mejor_13 = {INF, 0, 0};
    for (size_t i = 0; i < costeras_1.size(); i++) {
        for (size_t j = 0; j < costeras_3.size(); j++) {
            tCoste coste_conexion = suma(suma_costera1[i], suma_costera3[j]);
            if (coste_conexion < mejor_13.coste) {
                mejor_13 = {coste_conexion, costeras_1[i], costeras_3[j]};
            }
        }
    }
    
    // Conexión Isla2-Isla3
    Conexion mejor_23 = {INF, 0, 0};
    for (size_t i = 0; i < costeras_2.size(); i++) {
        for (size_t j = 0; j < costeras_3.size(); j++) {
            tCoste coste_conexion = suma(suma_costera2[i], suma_costera3[j]);
            if (coste_conexion < mejor_23.coste) {
                mejor_23 = {coste_conexion, costeras_2[i], costeras_3[j]};
            }
        }
    }
    
    // Evaluamos las 3 topologías posibles y elegimos la mejor, la que minimiza más distancia en sus islas
    // El coste total = suma_interna + coste_puente1 + coste_puente2
    
    tCoste coste_top1 = suma(mejor_12.coste, mejor_13.coste);
    tCoste coste_top2 = suma(mejor_12.coste, mejor_23.coste);
    tCoste coste_top3 = suma(mejor_13.coste, mejor_23.coste);
    
    // Devolvemos la mejor configuración
    Puente p1, p2;
    
    if (coste_top1 <= coste_top2 && coste_top1 <= coste_top3) {
        p1 = {mejor_12.c1, mejor_12.c2};
        p2 = {mejor_13.c1, mejor_13.c2};
    } else if (coste_top2 <= coste_top3) {
        p1 = {mejor_12.c1, mejor_12.c2};
        p2 = {mejor_23.c1, mejor_23.c2};
    } else {
        p1 = {mejor_13.c1, mejor_13.c2};
        p2 = {mejor_23.c1, mejor_23.c2};
    }
    
    return std::make_pair(p1, p2);
}

// 2da Opcion, como nos dice coste montar el puente es 0, no nos importa donde montar puentes
template <typename tCoste>
std::pair<Puente, Puente> ciudades_puente (const GrafoP<tCoste>& Isla1, 
    const GrafoP<tCoste>& Isla2, const GrafoP<tCoste>& Isla3,
    const std::vector<size_t>& ciudades_costeras_isla1,
    const std::vector<size_t>& ciudades_costeras_isla2,
    const std::vector<size_t>& ciudades_costeras_isla3
)
{   
    // si no hubiera los grafos fobos y demos, tendriamos que construir el grafo G 
    // apartir de la matriz de costes y las ciudades costeras de G
    size_t ciudad1 = ciudad_coste_min(Isla1, ciudades_costeras_isla1);
    size_t ciudad2 = ciudad_coste_min(Isla2, ciudades_costeras_isla2);
    size_t ciudad3 = ciudad_coste_min(Isla3, ciudades_costeras_isla3);

    Puente puente12 = Puente{ciudad1, ciudad2};
    Puente puente13 = Puente{ciudad1, ciudad3};

    return std::make_pair(puente12, puente13);
}


