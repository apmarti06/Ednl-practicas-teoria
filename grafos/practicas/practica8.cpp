#include "../estructuras/particion_agene.h"
#include "../estructuras/apo.h"
#include "../estructuras/matriz.h"
#include "./alg_grafoPMC.h"
#include "./grafoPMC.h"
#include "./grafoMA.h"

#include <cmath>
#include <limits>

/*1. El archipiélago de Tombuctú, está formado por un número indeterminado de islas,
cada una de las cuales tiene, a su vez, un número indeterminado de ciudades. En
cambio, sí es conocido el número total de ciudades de Tombuctú (podemos llamarlo N,
por ejemplo).

Dentro de cada una de las islas existen carreteras que permiten viajar entre todas
las ciudades de la isla. Se dispone de las coordenadas cartesianas (x, y) de todas y cada
una de las ciudades del archipiélago. Se dispone de un grafo (matriz de adyacencia) en
el que se indica si existe carretera directa entre cualesquiera dos ciudades del
archipiélago. El objetivo de nuestro problema es encontrar qué ciudades de Tombuctú
pertenecen a cada una de las islas del mismo y cuál es el coste mínimo de viajar entre
cualesquiera dos ciudades de una misma isla de Tombuctú. Así pues, dados los siguientes datos:

- Lista de ciudades de Tombuctú representada cada una de ellas por sus
coordenadas cartesianas.
- Matriz de adyacencia de Tombuctú, que indica las carreteras existentes en
dicho archipiélago.

Implementen un subprograma que calcule y devuelva la distribución en islas de las
ciudades de Tombuctú, así como el coste mínimo de viajar entre cualesquiera dos
ciudades de una misma isla del archipiélago.*/

// Ubicacion en el mapa de una ciudad del archipiélago
typedef std::pair<double, double> coordenadas;

// Calculamos la distancia entre dos coordenadas
double distanciaEuclidiana(coordenadas a, coordenadas b){
    return sqrt(pow(b.first - a.first, 2) + pow(b.second - a.second, 2));
}

// En este ejercicio recibimos un grafo no conexo (posee varias componentes), sin costes
// con un vector de coordenadas de donde se encuentran situados las ciudades

std::pair<Particion, matriz<double>> archipielagoTombuctú 
(const Grafo& Tombuctu, const std::vector<coordenadas>& ciudades)
{   
    size_t n = Tombuctu.numVert();
    Particion Distribucion_T(n);

    // Actualizamos un grafo con los costes de ir a un lugar a otro
    GrafoP<double> costes(n);

    // Creamos las islas en el tad particsion, recordando
    //que el grafo no es dirigido, olvidandonos de su inversa [i][j] == [j][i]
    // ojo  si pusieras de j=0, el infinito nos daria problemas tener en cuenta esto
    for (size_t i = 0; i < n; i++){
        for (size_t j = i + 1; j < n; j++){
            int rep_i = Distribucion_T.encontrar(i);
            int rep_j = Distribucion_T.encontrar(j);
            // si existe camino posible en el grafo (True), y no son mismo representante, los unimos
            if (Tombuctu[i][j] && rep_i != rep_j){
                // actualizamos grafo, y 
                Distribucion_T.unir(rep_i, rep_j);
                costes[i][j] = costes[j][i] = distanciaEuclidiana(ciudades[i], ciudades[j]);
            } else {
                costes[i][j] = costes[j][i] = GrafoP<double>::INFINITO;
            }
        }
    }

    // Una vez tenemos el grafo actualizado con todos los costes calculamos el coste minimo
    matriz<size_t> P;
    return std::make_pair(Distribucion_T, Floyd(costes, P));
}

/*2. El archipiélago de Tombuctú2 está formado por un número desconocido de islas,
cada una de las cuales tiene, a su vez, un número desconocido de ciudades, las cuales
tienen en común que todas y cada una de ellas dispone de un aeropuerto. Sí que se
conoce el número total de ciudades del archipiélago (podemos llamarlo N, por ejemplo).
Dentro de cada una de las islas existen carreteras que permiten viajar entre todas
las ciudades de la isla. No existen puentes que unan las islas y se ha decidido que la
opción de comunicación más económica de implantar será el avión.

Se dispone de las coordenadas cartesianas (x, y) de todas y cada una de las
ciudades del archipiélago. Se dispone de un grafo (matriz de adyacencia) en el que se
indica si existe carretera directa entre cualesquiera dos ciudades del archipiélago. El
objetivo de nuestro problema es encontrar qué líneas aéreas debemos implantar para
poder viajar entre todas las ciudades del archipiélago, siguiendo los siguientes criterios:

1) Se implantará una y sólo una línea aérea entre cada par de islas.
2) La línea aérea escogida entre cada par de islas será la más corta entre todas
las posibles.

Así pues, dados los siguientes datos:
• Lista de ciudades de Tombuctú2 representada cada una de ellas por sus
coordenadas cartesianas.
• Matriz de adyacencia de Tombuctú que indica las carreteras existentes en
dicho archipiélago.

Implementen un subprograma que calcule y devuelva las líneas aéreas necesarias para
comunicar adecuadamente el archipiélago siguiendo los criterios anteriormente
expuestos. */

// Primero hay que hallar el numero de islas, para determinar el numero de lineas a montar
// Se montaran solo una linea area por cada par de isla
// Debe de ser la distancia mas corta de todas las posibles (distancia de coordenadas cartesianas)

/*#include <TAD/grafoMA.h> */

typedef std::pair<double, double> coordenadas;

struct linea_Area {
    size_t origen, destino;
    double coste;
    // Calculamos el orden de lineas areas a meter en un apo
    bool operator <(const linea_Area& a) const
    { return coste < a.coste; }
};


std::vector<linea_Area> lineas_areas_tombuctu (const Grafo& Adyadencia, const std::vector<coordenadas>& ciudades)
{   

    // creamos datos iniciales, n el numero de ciuades, el numero de islas (determinado con particion), grafoP (matriz de costes)
    size_t n = Adyadencia.numVert();
    GrafoP<double> Tombuctu(n);
    Particion distribucion(n);

    // Creamos las islas en el tad particion, recordando y actualizando el numero de islas
    //que el grafo no es dirigido, olvidandonos de su inversa [i][j] == [j][i]
    size_t n_islas = n;
    for (size_t i = 0; i < n; i++){
        for (size_t j = i + 1; j < n; j++){
            int rep_i = distribucion.encontrar(i);
            int rep_j = distribucion.encontrar(j);
            // si existe camino posible en el grafo (True), y no son mismo representante, los unimos
            if (Adyadencia[i][j] && rep_i != rep_j){
                distribucion.unir(rep_i, rep_j);
                n_islas--;
            }
        }
    }

    // Guardamos quienes son representantes de las ciudades (sin saber de donde son),
    // para saber cuales son los que vamos a usar {-1, -1, -1, 3, -1, 5, -1, -1, -1, 9} 
    vector<int> representantes(n, -1);
    size_t isla = 0; // indice especifico de cada isla obtenida

    for (size_t i = 0; i < n; i++){
        // vemos si es representante, tomando la implementacion de arboles de altura
        if (representantes[i] == -1){
            // Buscamos todos los indices donde no esten actualizados, y sean el representante de la isla indicando que hay una isla
            for (size_t j = 0; j < n; j++){
                if (distribucion.encontrar(i) == distribucion.encontrar(j) && representantes[j] == -1){
                    representantes[j] = isla; 
                }
            }
            // cuando termine vemos cual es el representante de la ciudad x
            isla++;
        }
    }

    // Elegimos primero las mejores candidatos
    Apo<linea_Area> A(n * (n - 1) / 2);

    // Guardamos en el Apo, guardando el de una diagonal superior para evitar duplicados, solo si existen caminos (ser de la misma isla)
    for (size_t i = 0; i < n; i++){
        for (size_t j = i + 1; j < n; j++){
            if (distribucion.encontrar(i) != distribucion.encontrar(j)){
                linea_Area a = {i, j, distanciaEuclidiana(ciudades[i], ciudades[j])};
                A.insertar(a);
            }
        }
    }

    // calculamos el numero de lineas areas a montar
    std::vector<std::vector<bool>> adyIslas(n_islas, std::vector<bool>(n_islas, false));
    size_t nLineas = 0;
    vector<linea_Area> lineas(n_islas * (n_islas - 1) / 2);

    while (nLineas < (n_islas * (n_islas - 1) / 2)){
        linea_Area candidata = A.cima();
        A.suprimir();

        // escogemos del apo, los candidatosa asegurandonos que es el candidato, dandoles la ciudad x y obteniendo que isla es
        size_t isla1 = representantes[candidata.origen];
        size_t isla2 = representantes[candidata.destino];

        // si no se ha montado el minimo de pares de lineas areas es la candidata
        if (!adyIslas[isla1][isla2]){
            adyIslas[isla1][isla2] = adyIslas[isla2][isla1] = true;
            lineas.push_back(candidata);
            nLineas++;
        }
        
    }
    return lineas;
}

/*3. Implementa un subprograma para encontrar un árbol de extensión máximo. ¿Es más
difícil que encontrar un árbol de extensión mínimo? */

// Implementamos Kruskall MAX, recordando que es un grafo no dirigido
// conexo, devolviendo un árbol generador


/* // Si no queremos volver a implementarlo, sobrecargamos el operador <
bool operator <(const arista& a, const arista& b){
    return (a > b);
}
*/

template <typename tCoste>
GrafoP<tCoste> KruskallMax(const GrafoP<tCoste>&G)
{
    assert(!G.esDirigido());

    typedef typename GrafoP<tCoste>::vertice vertice;
    typedef typename GrafoP<tCoste>::arista arista;
    const tCoste INF = GrafoP<tCoste>::INFINITO;
    size_t n = G.numVert();

    // Devolvemos un grafo Max
    GrafoP<tCoste> res(n);
    Particion P(n);
    Apo<arista> A;

    // Insertamos las aristas inviertendo los costes
    for (vertice i = 0; i < n; i++){
        for (vertice j = i + 1; j < n; j++){
            if (G[i][j] != INF){
                // ponemos coste con un menos, para escoger los costes más caros
                A.insertar(arista(i, j, -G[i][j]));
            }
        }
    }

    size_t i = 1;
    while (i <= n - 1){ // seleccionamos 
        arista a = A.cima();
        A.suprimir();
        vertice u = P.encontrar(a.orig);
        vertice v = P.encontrar(a.dest);
        // si no son los mismos representantes los unimos
        if (u != v){
            P.unir(u, v);
            // incluimos la arista a en el arbol res
            res[a.orig][a.dest] = res[a.dest][a.orig] = -a.coste;
        }
    }
    return res;
}

/*4. La empresa EMASAJER S.A. tiene que unir mediante canales todas las ciudades del
valle del Jerte (Cáceres). Calcula qué canales y de qué longitud deben construirse
partiendo del grafo con las distancias entre las ciudades y asumiendo las siguientes
premisas:
− el coste de abrir cada nuevo canal es casi prohibitivo, luego la solución final
debe tener un número mínimo de canales.
− el Ministerio de Fomento nos subvenciona por Kms de canal, luego los canales
deben ser de la longitud máxima posible. */

// Tenemos que hacer un arbol generador maximo MST max, ya que nos pide que canales
template <typename tCoste>
GrafoP<tCoste> calcular_canales_maximos (const GrafoP<tCoste>& G){
    return KruskallMax(G);
}

/*5. La nueva compañía de telefonía RETEUNI3 tiene que conectar entre sí, con fibra
óptica, todas y cada una de las ciudades del país. Partiendo del grafo que representa las
distancias entre todas las ciudades del mismo, implementad un subprograma que
calcule la longitud mínima de fibra óptica necesaria para realizar dicha conexión. */

double calcular_distancia_minima (const GrafoP<double>& Pais){
    // apartir del arbolgenerador minimo, elegimos
    GrafoP<double> cables_compañia = Kruskall(Pais);
    double longitud = 0.0;

    // guardamos toda la distancia de los cables apartir del arbol generador
    for (size_t i = 0; i < cables_compañia.numVert(); i++){
        for (size_t j = i + 1; j < cables_compañia.numVert(); j++){
            longitud += cables_compañia[i][j];
        }
    }
    return longitud;
}

/*6. La empresa EMASAJER S.A. tiene que unir mediante canales todas las ciudades del
valle del Jerte (Cáceres), teniendo en cuenta las siguientes premisas:

− El coste de abrir cada nuevo canal es casi prohibitivo, luego la solución final debe
tener un número mínimo de canales. (Usar Kruskall o Prim)
− El Ministerio de Fomento nos subvenciona por m3 /sg de caudal, luego el conjunto de los canales debe admitir el mayor caudal posible,
 pero por otra parte, el coste de abrir cada canal es proporcional a su longitud, por lo que el conjunto de los
canales también debería medir lo menos posible. 

Así pues, la solución óptima debería combinar adecuadamente ambos factores.

(4 parametros)
Dada la matriz de distancias entre las diferentes ciudades del valle del Jerte, otra
matriz con los diferentes caudales máximos admisibles entre estas ciudades teniendo en cuenta su orografía
, la subvención que nos da Fomento por m3 /sg. de caudal y el coste por km. de canal, 

implementen un subprograma que calcule qué canales y de qué
longitud y caudal deben construirse para minimizar el coste total de la red de canales.*/

// datos para tener en cuenta de un canal
struct Canal {
    double caudal, longitud;
};

// Devolvemos una matriz de canales, con su caudal y longitud de ciudades
matriz<Canal> calcular_canales_caceres(const matriz<double>& distancias, 
                                        const matriz<double>& caudales,
                                        double subvencion_caudal, 
                                        double coste_km)
{   
    const double INF = GrafoP<double>::INFINITO;
    size_t n = distancias.dimension();
    
    // Matriz resultado, inicializada
    matriz<Canal> canales_minimos(n, {0.0, 0.0});
    
    // Calcular coste neto (puede ser negativo si beneficio > coste)
    GrafoP<double> coste_neto(n);
    for(size_t i = 0; i < n; i++){
        for(size_t j = i + 1; j < n; j++){
            if (distancias[i][j] != INF && caudales[i][j] != INF){
                double coste = (distancias[i][j] * coste_km) - (caudales[i][j] * subvencion_caudal);
                coste_neto[i][j] = coste_neto[j][i] = coste;
            } else {
                coste_neto[i][j] = coste_neto[j][i] = INF;
            }
        }
       
    }
    
    // Obtener MST (mínimo coste neto)
    GrafoP<double> arbol = Kruskall(coste_neto);
    
    // una vez almacenado los canales deseados, los volcamos en canales con sus costes reales
    for(size_t i = 0; i < n; i++){
        for(size_t j = i + 1; j < n; j++){
            // verificamos que sea un canal a construir (algun valor aunque no sea min)
            if(arbol[i][j] != INF && arbol[i][j] != 0){
                canales_minimos[i][j] = {caudales[i][j], distancias[i][j]};
                canales_minimos[j][i] = {caudales[i][j], distancias[i][j]};
            }
        }
    }

    return canales_minimos;
}


/*7. El archipiélago de Grecoland (Zuelandia) está formado únicamente por dos islas,
Fobos y Deimos, que tienen N1 y N2 ciudades, respectivamente, de las cuales C1 y C2
ciudades son costeras (obviamente C1 ≤ N1 y C2 ≤ N2 ). Se dispone de las coordenadas
cartesianas (x, y) de todas y cada una de las ciudades del archipiélago. El huracán
Isadore acaba de devastar el archipiélago, con lo que todas las carreteras y puentes
construidos en su día han desaparecido. En esta terrible situación se pide ayuda a la
ONU, que acepta reconstruir el archipiélago (es decir volver a comunicar todas las
ciudades del archipiélago) siempre que se haga al mínimo coste.

De cara a poder comparar costes de posibles reconstrucciones se asume lo
siguiente:

1. El coste de construir cualquier carretera o cualquier puente es proporcional a su
longitud (distancia euclídea entre las poblaciones de inicio y fin de la carretera o
del puente).

2. Cualquier puente que se construya siempre será más caro que cualquier carretera
que se construya.

De cara a poder calcular los costes de VIAJAR entre cualquier ciudad del
archipiélago se considerará lo siguiente:

1. El coste directo de viajar, es decir de utilización de una carretera o de un puente,
coincidirá con su longitud (distancia euclídea entre las poblaciones origen y
destino de la carretera o del puente).

En estas condiciones, implementa un subprograma que calcule el coste mínimo
de viajar entre dos ciudades de Grecoland, origen y destino, después de haberse
reconstruido el archipiélago, dados los siguientes datos:

1. Lista de ciudades de Fobos representadas mediante sus coordenadas cartesianas.
2. Lista de ciudades de Deimos representadas mediante sus coordenadas
cartesianas.
3. Lista de ciudades costeras de Fobos.
4. Lista de ciudades costeras de Deimos.
5. Ciudad origen del viaje.
6. Ciudad destino del viaje.    */

GrafoP<double> calcularDistanciaIsla (const vector<coordenadas>& ciudades,
    const vector<size_t> ciudades_costeras)
{
    size_t n = ciudades.size();
    GrafoP<double> G(n);

    for (size_t i = 0; i < n; i++){
        for (size_t j = 0; j < n; j++){
            G[i][j] = G[j][i] = distanciaEuclidiana(ciudades[i], ciudades[j]);
        }
        G[i][i] = 0;
    }

    return G;
}

double viajarZuelandiaLocura(
const vector<coordenadas>& ciudades_fobos, const vector<coordenadas>& ciudades_deimos,
const vector<size_t>& ciudades_costeras_fobos, const vector<size_t>& ciudades_costeras_deimos,
size_t origen, size_t destino)
{
    size_t nDeimos = ciudades_deimos.size();
    size_t nFobos = ciudades_fobos.size();

    // creamos grafos de arboles generadores minimo de reparacion de islas (carreteras y puentes) MST
    GrafoP<double> Deimos = calcularDistanciaIsla(ciudades_fobos, ciudades_costeras_fobos);
    GrafoP<double> Fobos = calcularDistanciaIsla(ciudades_deimos, ciudades_costeras_deimos);

    // solo montara una reparacion de todas las carreteras, sin juntar puentes pues son siempre más caro que las carreteras
    GrafoP<double> costes_min_d = Kruskall(Deimos);
    GrafoP<double> costes_min_f = Kruskall(Fobos);

    size_t puente_origen, puente_destino;
    double puenteMin = GrafoP<double>::INFINITO;

    // escogemos el puente más cercanos entre islas
    for (size_t i = 0; i < nDeimos; i++){
        for (size_t j = 0; j < nFobos; j++){
            double actual = distanciaEuclidiana(ciudades_fobos[i], ciudades_deimos[j]);
            if (actual < puenteMin){
                puente_origen = ciudades_costeras_fobos[i];
                puente_destino = ciudades_costeras_deimos[j];
                puenteMin = actual;
            }
        }
    }

    // Fusionamos ambos grafos, y resolvemos montando los puentes
    GrafoP<double> fusion(nDeimos + nFobos);

    // 1. Metemos el grafo de Deimos copiamos asi ya que la distancia de a->b es la misma de b->a
    for (size_t i = 0; i < nDeimos; i++){
        for (size_t j = i + 1; j < nDeimos; j++){
            fusion[i][j] = fusion[j][i] = costes_min_d[i][j]; 
        }
        fusion[i][i] = 0;
    }

    // 2. Metemos el grafo de Fobos
    for (size_t i = nDeimos; i < nDeimos + nFobos; i++){
        for (size_t j = i + 1; j < nDeimos + nFobos; j++){
            fusion[i][j] = fusion[j][i] = costes_min_f[i - nDeimos][j - nDeimos]; 
        }
        fusion[i][i] = 0;
    }

    // 3. Añadimos los puentes
    fusion[puente_origen][puente_destino + nDeimos] = fusion[puente_destino + nDeimos][puente_origen] = puenteMin;

    // 4. Calculamos el coste de ir de una ciudad de Deimos a otra de Fobos
    vector<size_t> P;
    vector<double> D = Dijkstra(fusion, origen, P);
    
    return D[destino];
}