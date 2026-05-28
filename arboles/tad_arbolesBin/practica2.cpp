#include "arbolBinDIM.h"
//#include "arbolBinPS.h"
#include <iostream>

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
    if ((nA == Abin<T>::NODO_NULO || nB == Abin<T>::NODO_NULO)){
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
    if (A.hijoIzq(nA) != Abin<T>::NODO_NULO){
        B.insertarHder(nB, A.elemento(A.hijoIzq(nA))); // cargamos el hijo izquierdo, en el hijo derecho de B
        reflejarArbolRec(A, A.hijoIzq(nA), B, B.hijoDer(nB));
    } 
    //
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

template <typename T>
class Abin {
    public:
        static const nodo NODO_NULO;
        typedef size_t nodo;
        Abin(size_t altura, const T& elto_nulo); // constructor árboles vacios
        void insertar_raiz(const T& elto);
        void insertarHijoIzqdo(nodo n, const T& elto);
        void insertarHijoDerch(nodo n, const T& elto);
        nodo padre(nodo n)const;

    private:
        T* nodos;
        size_t alturaMax, num_nodos, max_nodos;
        T ELTO_NULO;
        // metodos privados no públicos
        static int profundidad(nodo n, int i, int f);
        bool valido(nodo n) const;
        bool esHijoIzquierdo(nodo n) const;
}

// Añadimos el método de insertar posiciones deseadas del árbol + su método privada
template <typename T>
bool Abin<T>::valido(typename Abin<T>::nodo n) const{
    return num_nodos < max_nodos && !(nodos[n] == ELTO_NULO);
}

template <typename T>
static int Abin<T>::profundidad(typename Abin<T>::nodo n, int inicio, int fin){
    int pos = (inicio + fin) / 2;
    if (n < pos){
        return 1 + profundidad(n, inicio, pos - 1);
    } else if (n > pos) {
        return 1 + profundidad(n, pos + 1 , fin);
    } else {
        return 0;
    }
}

template <typename T>
bool Abin<T>::esHijoIzquierdo(nodo n) const { // verificamos si vale como padre el nodo x
    if (n == raiz()) return false;  // La raíz no es hijo
    
    int h = profundidad(n, 0, max_nodos);  // Altura del padre
    size_t potencia = 1 << (h + 2);  // 2^(h+2)
    size_t resto = n % potencia;
    
    return resto == (1 << h) - 1;  // n mod 2^(h+2) = 2^h - 1
}

// Ejercicio c


template <typename T>  
const typename Abin<T>::nodo Abin<T>::NODO_NULO{SIZE_MAX};

//constructor del árbol binario
template <typename T>
Abin<T>::Abin(size_t altura, const T& e_nulo) : alturaMax{altura} ,ELTO_NULO{e_nulo} {
    
    // Calcular número máximo de nodos: N = 2^(H+1) - 1
    max_nodos = (1 << (altura + 1)) - 1;  // 2^(H+1) - 1 usando desplazamiento de bits
    nodos = new T[max_nodos];
    num_nodos = 0;
    
    // Inicializar todos los nodos con el valor nulo
    for (size_t i = 0; i < max_nodos; i++) {
        nodos[i] = ELTO_NULO;
    }
}

// Métodos publicos de las funciones
template <typename T>
void Abin<T>::insertar_raiz(const T& elto){
    assert(!vacio());
    // insertamos la raíz al arbol
    size_t pos_raiz = (1 << alturaMax) - 1;
    nodos[pos_raiz] = elto;
    num_nodos = 1;
}

// metodo insertar
template <typename T>
void Abin<T>::insertarHijoIzqdo(typename Abin<T>::nodo n, const T& elto){
    assert(valido(n));
    // Insertamos en la posición adecuada
    int alt = profundidad(n, 0, alturaMax);
    size_t pos_hijo = n - (1 <<(alt - 1));
    nodos[pos_hijo] = elto;
    num_nodos++;
}

// metodo insertar
template <typename T>
void Abin<T>::insertarHijoDerch(typename Abin<T>::nodo n, const T& elto){
    assert(valido(n));
    // Insertamos en la posición adecuada
    int alt = profundidad(n, 0, alturaMax);
    size_t pos_hijo = n + (1 <<(alt - 1));
    nodos[pos_hijo] = elto;
    num_nodos++;
}

// devolvemos el nodo padre
template <typename T>
typename Abin<T>::nodo Abin<T>::padre(nodo n) const{
    assert(n != raiz()); // el unico nodo que no tiene padre es el ráiz
    size_t padre = profundidad(n) ;  // Altura del padre
    if (esHijoIzquierdo(n)) {
        // Si es hijo izquierdo: padre = n + 2^h
        return n + (1 << padre);
    } else {
        // Si es hijo derecho: padre = n - 2^h
        return n - (1 << padre);
    }
}

