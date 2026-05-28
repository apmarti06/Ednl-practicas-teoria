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
                if (ijk > A[i][j] && ijk != GrafoP<tCoste>::INFINITO && (ijk > A[i][j] || A[i][j] == GrafoP<tCoste>::INFINITO)){
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
Viaje<tCoste> pedro_sanchez(const GrafoP<tCoste>& G){
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
    return C.fila * N + C.col;
}

template <typename tCoste>
casilla nodo_to_casillas(GrafoP<tCoste>::vertice v, size_t N){
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
GrafoP<size_t>::tCamino laberinto1 (size_t N, vector<pared>& paredes, size_t origen, size_t salida){
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
    GrafoP<vertice>::tCamino camino = camino(origen, salida, P);

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
            camino_min[i] *= (1 - subvenciones[i]) / 100.0;
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
     const GrafoP<tCoste>& G_avion, alergia guerrero, tCoste plata_disp, GrafoP<tCoste>::vertice origen)
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
                    res.insert(v.end(), i);
                }
            }

            for (size_t i = 0; i < G_avion.numVert; i++){
                if (D1[i] < plata_disp && i != origen){
                    res.insert(v.end(), i);
                }
            }

        break;

        case TREN: // alergico al tren
            std::vector<tCoste> D1 = Dijkstra(G_carretera, origen, P1);
            std::vector<tCoste> D2 = Dijkstra(G_avion, origen, P2);

            // insertamos todos los vertices que mi niño pueda pagarse 
            for (size_t i = 0; i < G_carretera.numVert; i++){
                if (D1[i] < plata_disp && ){
                    res.insert(v.end(), i);
                }
            }

            for (size_t i = 0; i < G_avion.numVert; i++){
                if (D1[i] < plata_disp && i != origen){
                    res.insert(v.end(), i);
                }
            }
            
        break;

        case AVION: // alergico al avion
            std::vector<tCoste> D1 = Dijkstra(G_carretera, origen, P1);
            std::vector<tCoste> D2 = Dijkstra(G_tren, origen, P2);


            // insertamos todos los vertices que mi niño pueda pagarse 
            for (size_t i = 0; i < G_carretera.numVert; i++){
                if (D1[i] < plata_disp && ){
                    res.insert(v.end(), i);
                }
            }

            for (size_t i = 0; i < G_tren.numVert; i++){
                if (D1[i] < plata_disp && i != origen){
                    res.insert(v.end(), i);
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
    typedef GrafoP<tCoste>::INFINITO INFINITO;

    matriz<tCoste> A1 = Floyd(bus, P1);
    matriz<tCoste> A2 = Floyd(bus, P2);

    // creamos nuestro grafo definitivo
    for (size_t i = 0; i  < resultado.dimension(); i++){
        for (size_t j = 0; j < resultado.dimension(); j++){
            if (i != j){ // siempre sera coste 0 para A[i][i]
                tCoste coste_min = INFINITO;

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

// devolvemos un par de valores donde pasamos la ruta y el coste_minimo

// Implementamos la reconstruccion de caminos para usar dijkstra

template <typename tCoste>
typename GrafoP<tCoste>::tCamino reconstruirCamino(
    size_t origen,
    size_t destino,
    const vector<size_t>& P)
{
    typename GrafoP<tCoste>::tCamino camino;
    
    // Si no hay camino al destino
    if (P[destino] == destino && destino != origen) {
        return camino;
    }
    
    // Si el camino es directo al destino (coste 0)
    if (origen == destino) {
        camino.insertar(origen, camino.primera());
        return camino;
    }
    
    // Reconstruir camino (iterativo)
    vector<size_t> temp;
    size_t actual = destino;
    
    // vamos recorriendo por los vertices, hasta llegar al destino
    while (actual != origen) {
        temp.push_back(actual);
        actual = P[actual];
    }

    // vamos empujando en nuestro contenedor
    temp.push_back(origen);

    // Invertir pues va de destino---origen
    for (int i = temp.size() - 1; i >= 0; i--) {
        camino.insertar(temp[i], camino.fin());
    }   

    return camino;
}


template <typename tCoste>
std::pair<typename GrafoP<tCoste>::tCamino, tCoste> transbordos2(const GrafoP<tCoste>& bus, const GrafoP<tCoste>& tren,
size_t origen, size_t destino, size_t cambio1, size_t cambio2)
{   
    // usando pair devolveremos el resultado
    vector<size_t> bus_vuelta_P, tren_ida_P;
    GrafoP<tCoste>::tCamino cam1, cam2, ruta_final; // son lista de vertices

    // hacemos la busqueda de caminos desde el origen para su destino para trazar su camino, respetando precondiciones
    vector<tCoste> tren_ida_D = Dijkstra(tren, origen, tren_ida_P);
    vector<tCoste> bus_vuelta_D = Dijkstra_inv(bus, destino, bus_vuelta_P);

    // trazamos el camino guardando dicha ruta en un vertice y su coste
    tCoste minimo = GrafoP<tCoste>::INFINITO;
    size_t mejor_cambio;

    // vemos las 2 rutas disponibles, no podemos aplicar en mas de una ciudad el cambio

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

    // reconstruimos los caminos con el mejor cambio, cam1 del tren, y cam2 del bus??
    cam1 = camino(origen, mejor_cambio, tren_ida_P);
    cam2 = camino(destino, mejor_cambio, bus_vuelta_P);

    // invertimos el camino de la vuelta 
    cam2 = InvertirLista(cam2);

    // copiamos la primera, y añadimos al final la nueva ruta, desde el mejor cambio
    ruta_final = cam1;

    // copiamos la segunda tras haber invertido la lista
    if (!cam2.vacio()){ // operaciones de lista
        // saltamos el mejor cambio
        Lista<size_t>::posicion p = cam2.primera();
        p = cam2.siguiente();

        while (p != cam2.fin()){
            ruta_final.insertar(cam2.elemento(p), ruta_final.fin());
            p = cam2.siguiente(p);
        }
    }

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

        // Vemos cual es el coste menor de ir al destino en bus o tren
        tCoste minimo = std::min(c_salBus[destino], c_salTren[destino]);

        // Elegimos el viaje con coste minimo, que nos lleve al destino deseado
        for (size_t cambio = 0; cambio < n_tren; cambio++){
            minimo = std::min(suma(c_salBus[cambio], c_lleTren[cambio]), suma(c_salTren[cambio], c_lleBus[cambio]), minimo);
        }

        return minimo;
    }




