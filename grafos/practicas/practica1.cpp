#include "./grafoPMC.h"
#include "./grafoPLA.h"
#include "../algoritmos/Dijkstra.h"


/*1. Añadir una función genérica, llamada DijkstraInv, en el fichero
alg_grafoPMC.h para resolver el problema inverso al de Dijkstra, con los mismos
tipos de parámetros y de resultado que la función ya incluida para éste. La nueva
función, por tanto, debe hallar el camino de coste mínimo hasta un destino desde cada
vértice del grafo y su correspondiente coste. */

// Suma de costes
template <typename tCoste>
tCoste suma(tCoste x, tCoste y)
{
    const tCoste INF = GrafoP<tCoste>::INFINITO; // asi no declaramos dos veces lo mismo

    return (x == INF || y == INF) ? INF : x + y;
}

// Si tu haces la matriz traspuesta, te ahorras implementar este algoritmo
template <typename tCoste>
vector<tCoste> Dijkstra_inv(const GrafoP<tCoste>& G, typename GrafoP<tCoste>::vertice destino,
    vector<typename GrafoP<tCoste>::vertice>& P){
        // assert(G.esdirecto())
    typedef typename GrafoP<tCoste>::vertice vertice;
    vertice v, w;
    const size_t n = G.numVert();

    vector<bool> S(n, false); // vector de vertices procesados
    vector<tCoste> D(n);

    // escogemos como pivote la columna x, todos sus costes
    for (vertice i = 0; i < n; i++){
        D[i] = G[i][destino];
    }

    // Procesamos como coste minimo el destino 
    S[destino] = true;
    D[destino] = 0;

    // procesamos desde 1, pues ya tenemos uno siempre procesado, y casi siempre el ultimo sera el de maximo coste, lo cual lo quitamos
    for (vertice i = 1; i <= n - 2; i++){

        // Escogemos el mejor candidato de menor coste para ver sus caminos, si es mejor que desde (vertices iniciales) --> destino
        tCoste coste_minimo = GrafoP<tCoste>::INFINITO; 
        for (v = 0; v < n; v++){
            if (!S[i] && D[v] <= coste_minimo){ // si no esta procesado y es el minimo lo elegimos como candidato
                coste_minimo = D[v];
                w = v;
            } 
        }

        // Procesamos el camino de menor coste
        S[w] = true;
        
        for (v = 0; v < n; n++){
            if (!S[v]){
                // verificamos que si hay camino alterno, este sea menor que el coste minimo actual
                tCoste uvw = suma(G[v][w], D[w]);
                if (uvw < D[v]){
                    // si el camino nuevo es menor lo actualizamos tanto el coste, como el nodo previo a llegar a este
                    P[v] = w;
                    D[v] = uvw;
                }
            }
        }

    }
    return D;
}

/*2. Definiremos el pseudocentro de un grafo conexo como el nodo del mismo que
minimiza la suma de las distancias mínimas a sus dos nodos más alejados. Definiremos
el diámetro del grafo como la suma de las distancias mínimas a los dos nodos más
alejados del pseudocentro del grafo. Dado un grafo conexo representado mediante matriz 
de costes, implementa un subprograma que devuelva la longitud de su diámetro. */

// devolveremos la distancia del epicentro que los nodos más alejados, donde la suma sera el diametro deseado

template <typename tCoste>
std::pair<size_t, tCoste> Pseudocentro(const GrafoP<tCoste>& G)
{
    size_t n = G.numVert();
    diametro = GrafoP<tCoste>::INFINITO;
    vector<tCoste> sumas(n, GrafoP<tCoste>::INFINITO);


    for (size_t i = 0; i < n; i++){
        // probamos en cada vertice, cual es del coste minimo, es el que mayor coste tiene entre pares, y cogemos otro que los minimice
        vector<size_t> P;
        vector<tCoste> d = Dijkstra(G, i, P);

        tCoste m1 = -1, m2 = -1;
        // Calculamos por cada vertice el camino menos costoso, de dos vertices a uno A-> B <-C
        for (size_t j = 0; j < n; j++){
            if (d[j] > m1) {  // Si encontramos una distancia mayor que m1
                m2 = m1;
                m1 = d[j];
            }
            else if (d[j] > m2) {  // Si encontramos una distancia mayor que m2
                m2 = d[j];
            }
        }

        // si se ha encontrado un par de distancias maximas, las guardamos en sumas para ver cual es mejor
        if (m1 != -1 && m2 != -1){
            diametro = std::min(diametro, suma(m1, m2));
            sumas[i] = suma(m1, m2);
        }
    }
 
    // Ahora, buscamos el nodo con la menor suma de las distancias mínimas
    tCoste minSuma = GrafoP<tCoste>::INFINITO;
    size_t pseudocentro = 0;

    for (size_t i = 0; i < n; i++) {
        if (sumas[i] < minSuma) {
            minSuma = sumas[i];
            pseudocentro = i;  // Encontramos el nodo con la menor suma de distancias
        }
    }
    
    return std::make_pair{pseudocentro, minSuma};
}

/*
template <typename tCoste>
tCoste diametro_pseudocentro(const GrafoP<tCoste>& G){
    typedef typename GrafoP<tCoste>::vertice vertice;
    // 1. Recibimos los nodos más alejados (el camino que mayor coste tenga, que no sea infinito)
    vertice v, w;
    obtener_nodosAlejados(G, v, w);

    // 2. Calculamos el epicentro 
    vertice epicentro = calcular_epicentro()

    // 3. Calculamos el diametro
    tcoste diametro_grafo = suma(G[v][epicentro], G[w][epicentro]);

    return diametro_grafo;
}   

template <typename tCoste>
void obtener_nodosAlejados(const GrafoP<tCoste>& P , typename GrafoP<tCoste>::vertice& v, typename GrafoP<tCoste>::vertice& w){
    // buscamos el que tenga mayor coste
    typedef typename GrafoP<tCoste>::vertice vertice;

    tCoste coste_invalido = GrafoP<tCoste>::INFINITO; 
    vertice maximo_i = -1, maximo_j = -1;

    // Calculamos recorriendo todo el grafo, la arista de mayot coste, respetando precondiciones
    for (vertice i = 0; i < P.numVert(); i++){
        for (vertice j = 0; j < P.numVert(); j++){
            if (G[i][j] > G[maximo_i][maximo_j] && G[i][j] != coste_invalido){
                maximo_i = i;
                maximo_j = j;
            }
        }
    }
}   
 
*/

/*3. Tu empresa de transportes “PEROTRAVEZUNGRAFO S.A.” acaba de recibir la
lista de posibles subvenciones del Ministerio de Fomento en la que una de las más
jugosas se concede a las empresas cuyo grafo asociado a su matriz de costes sea
acíclico. ¿Puedes pedir esta subvención? Implementa un subprograma que a partir
de la matriz de costes nos indique si tu empresa tiene derecho a dicha subvención.*/

template <typename tCoste>
void profundidad(const GrafoP<tCoste>& G, size_t u, vector<int>& color, bool& ciclo) {
    // Se marca el nodo u como en proceso
    color[u] = 1;

    // Recorremos los nodos adyacentes de u
    for (size_t v = 0; v < G.numVert() && !ciclo; v++) {
        if (G[u][v] != GrafoP<tCoste>::INFINITO && G[u][v] != 0) {  // Si hay una arista de u a v
            if (color[v] == 1) {
                // Si encontramos un nodo en proceso, hay un ciclo
                ciclo = true;
                return;
            }
            if (color[v] == 0) {
                // Si el nodo v no ha sido visitado, hacemos la llamada recursiva
                profundidad(G, v, color, ciclo);
            }
        }
    }

    // Después de procesar todos los adyacentes de u, lo marcamos como procesado
    color[u] = 2;
}

template <typename tCoste>
bool matrizCostes_es_aciclica (const GrafoP<tCoste>& P){
    // Creamos nuestra longitud a recorrer, su vector de visitados, y ver su ciclo
    size_t n = P.numVert();
    vector<int> color(n, 0);  // 0 = no visitado, 1 = en proceso, 2 = procesado
    bool flag = false;

    for (size_t i = 0; i < n; i++){
        if (color[u] == 0){
            profundidad(P, i, color, ciclo);
        }
    }   

    // si es ciclico tenemos que la matriz de costes no es aciclica
    return !ciclo;
}


/*4. Se necesita hacer un estudio de las distancias mínimas necesarias para viajar entre
dos ciudades cualesquiera de un país llamado Zuelandia. El problema es sencillo pero
hay que tener en cuenta unos pequeños detalles:

a) La orografía de Zuelandia es un poco especial, las carreteras son muy estrechas
y por tanto solo permiten un sentido de la circulación.

b) Actualmente Zuelandia es un país en guerra. Y de hecho hay una serie de
ciudades del país que han sido tomadas por los rebeldes, por lo que no pueden
ser usadas para viajar.

c) Los rebeldes no sólo se han apoderado de ciertas ciudades del país, sino que
también han cortado ciertas carreteras, (por lo que estas carreteras no pueden ser
usadas).

d) Pero el gobierno no puede permanecer impasible ante la situación y ha exigido
que absolutamente todos los viajes que se hagan por el país pasen por la capital
del mismo, donde se harán los controles de seguridad pertinentes.

NOS DICE QUE SI UN VIAJE PASA POR CAPITAL origen-CAPITAL-destino, usamod dijkstra normal e inverso

Dadas estas cuatro condiciones, se pide implementar un subprograma que dados
• el grafo (matriz de costes) de Zuelandia en situación normal,
• la relación de las ciudades tomadas por los rebeldes,
• la relación de las carreteras cortadas por los rebeldes
• y la capital de Zuelandia,

Calcule la matriz de costes mínimos para viajar entre cualesquiera dos ciudades
zuelandesas en esta situación.*/

template <typename tCoste>
GrafoP<tCoste> Zuelandia 
(const GrafoP<tCoste>& G, vector<size_t> ciudades, 
vector<std::pair<size_t, size_t>> carreteras, size_t capital){ 
    // usamos std::pair para representar una arista v-w
    // Representamos en un nuevo grafo las carreteras cortadas
    GrafoP<tCoste> G_(G);

    // Carreteras tomadas, las ponemos a infinito pues nuestro habitantes no pueden pasar por ahi
    for (auto carretera: carreteras){
        G_[carreteras.first][carreteras.second] = GrafoP<tCoste>::INFINITO;
    }

    // Marcamos tambien las ciudades tomadas igual que las carreteras,
    // quitando toda posibilidad desde la ciudad hacia cualquier destino, o alreves
    for (auto ciudad: ciudades){
        for (size_t i = 0; i < G.numVert()){
            G_[ciudad][i] = GrafoP<tCoste>::INFINITO;
            G_[i][ciudad] = GrafoP<tCoste>::INFINITO;
        }
    }

    vector<size_t> v1, v2;
    vector<tCoste> d1, d2;

    // Como ha de pasar siempre por la capital los caminos han de ser por aqui
    // recordamos que d es un vector de costes de caminos minimos, y v de vertices intermedios entre el vertice del camino minimo y O/D
    d1 = Dijkstra_inv(G_, capital, v1); 
    d2 = Dijkstra(G_, capital, v2);

    // Creamos el grafo Zuelandia, por el constructor de copia
    GrafoP<tCoste> Zuelandia(G_);

    // Guardamos los caminos que deben de hacer sin pasar por territorio enemigo pasando obligatoriamente por la capital, 
    // Y que pase por la ciudad, donde este debe ser el coste minimo
    for (size_t i = 0; i < Zuelandia.numVert(); i++){
        for (size_t j = 0; j < Zuelandia.numVert(); j++){
            if (d1[i] == GrafoP<tCoste>::INFINITO || d2[j] == GrafoP<tCoste>::INFINITO){
                Zuelandia[i][j] = GrafoP<tCoste>::INFINITO;  // No hay ruta disponible
            } else { // calculamos la distancia entre dos ciudades pasando por la capital
                Zuelandia[i][j] = suma(d1[i], d2[j]);
                // x[i][j] = x[j][i] = suma(d1[i], d2[j]) seria erroneo ya que dejaria de ser un grafo dirigido
            }
        }
    }
    return Zuelandia;
}

/*5. Escribir una función genérica que implemente el algoritmo de Dijkstra usando un
grafo ponderado representado mediante listas de adyacencia.*/

/*template <typename T> class GrafoP {
public:
   typedef T tCoste;
   typedef size_t vertice;
   typedef Lista<vertice> tCamino;

   struct vertice_coste {
      vertice v;
      tCoste c;
   };

   static const tCoste INFINITO;

   GrafoP(size_t n): ady(n) {}
   GrafoP(const std::string& nf);
   size_t numVert() const {return ady.size();}
   const Lista<vertice_coste>& adyacentes(vertice v) const {return ady[v];}
   Lista<vertice_coste>& adyacentes(vertice v) {return ady[v];}

private:
   vector<Lista<vertice_coste>> ady;
};*/

template <typename tCoste>
vector<tCoste> DijkstraLPA(const GrafoP<tCoste>& G, 
    typename GrafoP<tCoste>::vertice origen, vector<size_t>& P) {

    typedef typename GrafoP<T>::tCoste tCoste;
    typedef typename GrafoP<T>::vertice vertice;

    vertice v, w;

    size_t n = G.numVert(); // num vertices
    vector<bool> S(n, false); // conjunto vertices no procesados
    vector<tCoste> D(n, GrafoP<T>::INFINITO);

    // Inicializamos al comienzo el vector P
    for (size_t i = 0; i < n; i++){
        P[i] = 0;
    }

    // Inicializamos D y P con caminos directos desde el vertice origen
    Lista<GrafoP<T>::vertice_coste> Lv = G.adyacentes(origen); // lista con vertices

    // Pasamos los costes al vector que devolvemos que esta inicializado en Infinito
    // Guardando el valor para el vertice x, y su coste guardado en la lista Lv
    for (Lista<GrafoP<T>::vertice_coste> pos = Lv.primera(); Lv.fin() != pos; pos = Lv.siguiente()){
        D[Lv.elemento(pos).v] = Lv.elemento(pos).c;
    }

    // inicializamos el coste a si mismo que es 0
    D[origen] = 0;

    for (size_t i = 0; i < n - 2; i++){
        // Seleccionamos el vertice de menor coste al origen
        tCoste costemin = GrafoP<tCoste>::INFINITO;
        for (v = 0; v < n; v++){
            if (D[v] <= costemin && !S[v]){
                costemin = D[v];
                w = v;
            }
        }

        // Procesamos el vertice y comprobamos si es el camino minimo
        S[w] = true;    
        
        // Como tenemos todos los caminos adyacentes en una lista, vamos viendo en esta los costes
        for (Lista<GrafoP<T>::vertice_coste> pos = Lv.primera(); Lv.fin() != pos; pos = Lv.siguiente()){
            if (!S[Lv.elemento(pos).v]){ // comparamos con los no procesados
                tCoste ijk = suma(D[w], Lv.elemento(pos).c);
                if (ijk < D[Lv.elemento(pos).v]){
                    D[Lv.elemento(pos).c] = ijk;
                    P[Lv.elemento(pos).v] = w;
                }
            }
        }

        return D;
    }

}