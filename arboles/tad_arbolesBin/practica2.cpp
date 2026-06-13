#include "arbolBinDIM.h"
#include "arbolBinPosRel.h"
#include 
#include <iostream>
#include <cassert>
#include <utility>

/*1. Dos árboles binarios son similares cuando tienen idéntica estructura de ramificación,
es decir, ambos son vacíos, o en caso contrario, tienen subárboles izquierdo y derecho
similares. Implementa un subprograma que determine si dos árboles binarios son
similares.*/

template <typename T>
bool sonSimilares(const Abin<T> A,const Abin<T> B){
    return sonSimilaresREC(A, A.raiz(), B, B.raiz());
}

template <typename T>
bool sonSimilaresREC (const Abin<T> A, typename Abin<T>::nodo nA, const Abin<T> B, typename Abin<T>::nodo nB ){
    if ((nA == Abin<T>::NODO_NULO || nB == Abin<T>::NODO_NULO)){ // si encontramos un nodo nulo, el otro nodo del arbol debe ser nulo tb
        return (nA == Abin<T>::NODO_NULO && nB == Abin<T>::NODO_NULO);
    } else {
        return (sonSimilaresREC(A, A.hijoIzq(nA), B, B.hijoIzq(nB)) && sonSimilaresREC(A, A.hijoDer(nA), B, B.hijoDer(n)));
    }
}

/*2. Para un árbol binario B, podemos construir el árbol binario reflejado B R cambiando los
subárboles izquierdo y derecho en cada nodo. Implementa un subprograma que devuelva
el árbol binario reflejado de uno dado.*/

// añadimos nodos hijos a los padres inviertiendolos
template <typename T>
Abin<T> reflejarArbol(const Abin<T>& A){
    Abin<T> resultado;
    if (!A.vacio()){
        // guardamos el nodo raíz al comenzar copiar el árbol
        resultado.insertarRaiz(A.elemento(A.raiz())); // metemos el nodo raiz
        reflejarArbolRec(A, A.raiz(), resultado, resultado.raiz()); // retornamos la modificación en resultado
    }
    return resultado;
}

template <typename T>
void reflejarArbolRec(const Abin<T>& A, typename Abin<T>::nodo nA, Abin<T>& B, typename Abin<T>::nodo nB){
    // verificamos en cada nodo si posee hijos izquierdos/derechos, operación en preorden

    // insertamos los hijos izquierdo del arbol A en el derecho, si existen
    if (A.hijoIzq(nA) != Abin<T>::NODO_NULO){
        B.insertarHder(nB, A.elemento(A.hijoIzq(nA))); // cargamos el hijo izquierdo, en el hijo derecho de B
        reflejarArbolRec(A, A.hijoIzq(nA), B, B.hijoDer(nB));
    } 
    // insertamos los hijos derecho del arbol A en el izquierdo, si existen
    if (A.hijoDer(nA) != Abin<T>::NODO_NULO){
        B.insertarHizq(nB, A.elemento(A.hijoDer(nA)));
        reflejarArbolRec(A, A.hijoDer(nA), B, B.eliminarHizq(nB));
    }
}

/*4. Una posible representación del TAD Árbol Binario consiste en almacenar los
elementos del árbol en un vector cuyo tamaño, 𝑁 = 2 𝐻+1 − 1 , depende de la altura
máxima 𝐻 que pueda llegar a alcanzar el árbol. Cada nodo del árbol se corresponde con
una única posición del vector, la cual viene determinada por el recorrido en inorden del
árbol. Es decir, en el vector aparecen primero los nodos del subárbol izquierdo en inorden,
luego la raíz y a continuación los nodos del subárbol derecho también en inorden. 

Por ejemplo, el árbol de la figura se representa como el vector
(-, -, -, n, -, -, -, a, f, l, -, b, -, d, -, h, -, -, -, -, -, -, -, m, -, c, -, j, -, -, -),
donde ‘-’ representa una posición vacía.

Los hijos izquierdo y derecho de un nodo n corresponden, respectivamente,
a las posiciones n - 2^(h-1) y n + 2^(h-1), donde h es la altura máxima
que puede alcanzar n, la cual se puede calcular a partir de la profundidad p
de n y de la altura máxima del árbol, h = H - p.

Por tanto, el padre de un nodo n se calcula de la siguiente forma:
 *
 *     Padre(n) = { n + 2^h   si n es hijo izquierdo
 *                { n - 2^h   si n es hijo derecho
 *
 * Un nodo n es hijo izquierdo de su padre si se cumple la igualdad:
 *
 *     n mod 2^(h+2) = 2^h - 1

a) Define la clase genérica Abin<T> para esta representación.
b) Implementa una función miembro privada que calcule la altura de un nodo de un
árbol binario representado de la forma descrita.
c) Para esta representación implementa, al menos, el constructor de árboles vacíos y
las operaciones insertarRaiz(), insertarHijoIzqdo() y padre(), según la
especificación del TAD Árbol Binario vista en clase.

Anotaciones importantes, representar pensando que estan en inorden -->
Hacer el constructor con la altura para que el nodo quede bien guardado
Si altura es 4 --> 2^4+1 - 1 se colocara dicho nodo
*/

/*

Índice:   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14
Valor:    -   -   -   n   -   -   -   a   f   l   -   b   -   d   -

         [7] = a
        /        \
     [4] = f    [10] = l
     /   \       /    \
  [1] = n [8] = b [12] = d [13] = h
   /   \   / \   / \   / \   / \
 [0] [2] [3] [5] [6] [9] [11] [14]
  -   -   -   -   -   -   -   -

*/

class Abinn {
    public:
        typedef size_t nodo;
        static const nodo NODO_NULO;
        Abinn(int altura, const int& elto_nulo); // constructor árboles vacios
        bool vacio() const;
        void insertar_raiz(const int& elto);
        void insertarHijoIzqdo(nodo n, const int& elto);
        void insertarHijoDerch(nodo n, const int& elto);
        nodo padre(nodo n)const;

    private:
        int* nodos;
        size_t alturaMax, num_nodos, max_nodos;
        int ELTO_NULO;
        // metodos privados no públicos
        static int profundidad(nodo n, int i, int f);
        bool valido(nodo n) const;
        bool esHijoIzquierdo(nodo n) const;
};

// Añadimos el método de insertar posiciones deseadas del árbol + su método privada
bool Abinn::valido(typename Abinn::nodo n) const{
    return n < num_nodos && num_nodos < max_nodos && !(nodos[n] == ELTO_NULO);
}

int Abinn::profundidad(typename Abinn::nodo n, int inicio, int fin){
    // empezamos por el raíz
    int pos = (inicio + fin) / 2;
    if (n < pos){
        return 1 + profundidad(n, inicio, pos - 1);
    } else if (n > pos) {
        return 1 + profundidad(n, pos + 1 , fin);
    } else {
        return 0;
    }
}

bool Abinn::esHijoIzquierdo(typename Abinn::nodo n) const { // verificamos si vale como padre el nodo x
    assert(n != (1 << (alturaMax) - 1));
    int H = alturaMax - profundidad(n, 0, max_nodos - 1);  // Altura del padre
    size_t potencia = 1 << (H + 2);  // 2^(H+2)
    size_t resto = n % potencia;
    return resto == (1 << H) - 1;  // n mod 2^(h+2) = 2^h - 1
}

// Ejercicio c

const typename Abinn::nodo Abinn::NODO_NULO{SIZE_MAX};

//constructor del árbol binario
Abinn::Abinn(int altura, const int& e_nulo) : alturaMax{altura} , ELTO_NULO{e_nulo} {
    
    // Calcular número máximo de nodos: N = 2^(H+1) - 1
    max_nodos = (1 << (altura + 1)) - 1;  // 2^(H+1) - 1 usando desplazamiento de bits
    nodos = new int[max_nodos];
    num_nodos = 0;
    
    // Inicializar todos los nodos con el valor nulo
    for (size_t i = 0; i < max_nodos; i++) {
        nodos[i] = ELTO_NULO;
    }
}

// verificamos si existe algun elto en la raiz
bool Abinn::vacio() const{
    return nodos[(1 << (alturaMax)) - 1] == 0;
}

// Métodos publicos de las funciones
void Abinn::insertar_raiz(const int& elto){
    assert(!vacio());
    // insertamos la raíz al arbol
    size_t pos_raiz = (1 << alturaMax) - 1;
    nodos[pos_raiz] = elto;
    num_nodos = 1;
}

// metodo insertar
void Abinn::insertarHijoIzqdo(typename Abinn::nodo n, const int& elto){
    assert(valido(n));
    // Insertamos en la posición adecuada
    int alt = alturaMax - profundidad(n, 0, max_nodos - 1);
    size_t pos_hijo = n - (1 <<(alt - 1));
    if (nodos[pos_hijo] == ELTO_NULO){
        nodos[pos_hijo] = elto;
        num_nodos++;
    }
}

// metodo insertar
void Abinn::insertarHijoDerch(typename Abinn::nodo n, const int& elto){
    assert(valido(n));
    // Insertamos en la posición adecuada
    int alt = alturaMax - profundidad(n, 0, max_nodos - 1);
    size_t pos_hijo = n + (1 <<(alt - 1));
    if (nodos[pos_hijo] == ELTO_NULO){
        nodos[pos_hijo] = elto;
        num_nodos++;
    }
}

// devolvemos el nodo padre, para verificar donde esta primero vemos si es izquierdo o derecho, pues
// son posiciones distintas y de total importancia a calcular
typename Abinn::nodo Abinn::padre(nodo n) const{
    assert(valido(n)); // comprobar que el nodo existe
    // Si es la raíz, no tiene padre
    size_t pos_raiz = (1 << alturaMax) - 1;
    if (n == pos_raiz) {
        return NODO_NULO;
    }

    // Calcular h = altura que puede alcanzar el padre (altura máxima - profundidad del padre)
    // La profundidad del padre es prof - 1
    int prof = profundidad(n, 0, max_nodos - 1);
    int h = alturaMax - (prof - 1);

    // Altura del padre
    if (esHijoIzquierdo(n)) {
        // Si es hijo izquierdo: padre = n + 2^h
        return n + (1 << h);
    } else {
        // Si es hijo derecho: padre = n - 2^h
        return n - (1 << h);
    }
}


/* Un nodo verde es aquel nodo que cumple cierta condicion: posee 3 nietos*/
template <typename T>
size_t contarNodosVerdes(const Abin<T>& A){
    if (!A.vacio()){
        return contarNodosVerdesRec(A, A.raiz());
    } else {
        return 0;
    }
}

// Aseguramos que tenga el nodo hijos para ver si se cumple que hay tres nietos
template <typename T>
bool DosHijos(const Abin<T>& A, typename Abin<T>::nodo n){
    return (A.hijoIzq(n) != Abin<T>::NODO_NULO && A.hijoDer(n) != Abin<T>::NODO_NULO);
}

template <typename T>
size_t contarNodosVerdesRec(const Abin<T>& A, typename Abin<T>::nodo n){
    if (n == Abin<T>::NODO_NULO){
        return 0;
    } else {
        size_t nietos = 0;
        if (DosHijos(A, n)){
            // contamos los nietos del nodo n
            if (A.hijoIzq(A.hijoIzq(n)) != Abin<T>::NODO_NULO) nietos++;
            if (A.hijoDer(A.hijoIzq(n)) != Abin<T>::NODO_NULO) nietos++;
            if (A.hijoIzq(A.hijoDer(n)) != Abin<T>::NODO_NULO) nietos++;
            if (A.hijoDer(A.hijoDer(n)) != Abin<T>::NODO_NULO) nietos++;
        }
        // seguimos recorriendo el árbol para contar el resto de nodos verdes
        if (nietos == 3){
            return 1 + contarNodosVerdesRec(A, A.hijoIzq(n)) + contarNodosVerdesRec(A, A.hijoDer(n));
        } else {
            return contarNodosVerdesRec(A, A.hijoIzq(n)) + contarNodosVerdesRec(A, A.hijoDer(n));
        }
    }
}
/*
Enunciado: Dado un nodo cualquiera flotarlo hasta que se cumpla una condicion. Por
ejemplo, flotar un nodo hasta que el elemento del mismo sea menor que el de su padre.

Vamos a implementar un ABB mediante un Abin, para ello vamos a ir recorriendo el
arbol binario comparando los elementos de los nodos y orden ́andolos.

Como vamos a ir modificando el  ́arbol, este seŕa una referencia no constante y se
devolera por referencia 
*/

template <typename T>
bool buscarNodo(const Abin<T>& A, const T& elto, typename Abin<T>::nodo n, typename Abin<T>::nodo& encontrado){
    if (n == Abin<T>::NODO_NULO) return false;
    // nos movemos en el arbol buscando el elto
    if (A.elemento == elto){
        encontrado = n;
        return true;
    } else {
        // con que hayamos encontrado una vez el elto nos vale
        return buscarNodo(A, elto, A.hijoIzq(n), encontrado) 
        || buscarNodo(A, elto, A.hijoDer(n), encontrado);
    }

}

template <typename T>
void FlotarNodosRec(Abin<T>& A, typename Abin<T>::nodo n){
    if (A.elemento() <= A.elemento(A.padre(n)) || A.padre(n) == Abin<T>::NODO_NULO){
        return; // si no hay mas nodos que recorrer, o ya este es el definitivo lo guardamos
    } else {
        // flotamos el elto, y vemos si el nuevo valor del padre, es menor qe su respectivo padre
        std::swap(A.elemento(n), A.elemento(A.padre(n)));
        return FlotarNodosRec(A, A.padre(n));
    }
}

template <typename T>
void FlotarNodos(Abin<T>& A, const T& elto){
    if (!A.vacio()){
        typename Abin<T>::nodo encontrado = Abin<T>::NODO_NULO;
        if (buscarNodo(A, elto, A.raiz(), encontrado)){
            FlotarNodosRec(A, encontrado);
        }
    }
}

// Repasamos las condiciones para que un Abb sea un AVL ->

/*

1. Poseer un factor de desequilibrio <= 1, es decir que no existe entre todos los nodos un desequilibrio mayor que 1 o menor que -1
2. Le pasamos un Arbol Binario no un Abb, osea le añadimos dificultad
*/

template <typename T>
int desiquilibrio (const Abin<T> A){
    return desiquilibrioRec(A, A.raiz());
}

template <typename T>
int desiquilibrioRec(const Abin<T> A, typename Abin<T>::nodo n){
    if (n == Abin<T>::NODO_NULO){ // no ponemos a -1 pues el de altura ya lo hace
        return 0;
    } else {
        int izq = Altura_A(A, A.hijoIzq(n));
        int der = Altura_A(A, A.hijoDer(n));
        return std::max(desiquilibrioRec(A, A.hijoIzq(n)), desiquilibrioRec(A, A.hijoDer(n)), std::abs(izq - der));
    }
}

template <typename T>
bool es_un_abb(const Abin<T>& A, typename Abin<T>::nodo n, int minimo, int maximo){
    if (n == Abin<T>::NODO_NULO){ // se ha llegado a la ultima capa por lo que tiene que ser para esa rama verdadera
        return true;
    } else {
        T actual = A.elemento(n);
        if (actual > minimo && actual < maximo){
            return es_un_abb(A, A.hijoIzq(n), minimo, actual) && 
            es_un_abb(A, A.hijoDer(n), actual, maximo);
        } else { // si el elto actual es mayor o menor que el de su rama (en izq seria si es mayor a su padre, y en derecho menor, deja de ser abb)
            return false;
        }
    }
}

template <typename T>
bool el_abin_esAVL(const Abin<T>& A){
    if (A.vacio()){ // el arbol esta vacio pues es AVL
        return true;
    } else {
        int min = 100000000;
        int max = -100000000;
        return (desiquilibrio(A) < -1 && desiquilibrio(A) > 1) && es_un_abb(A, A.raiz(), min, max);
    }
}
