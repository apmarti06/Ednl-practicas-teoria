#ifndef ABIN_VEC0_H
#define ABIN_VEC0_H
#include <cassert>
#include <cstddef> // size_t
#include <cstdint> // SIZE_MAX
#include <utility> // swap

template <typename T> class Abin {
public:
    typedef size_t nodo;
    static const nodo NODO_NULO;
    explicit Abin(size_t maxNodos = 0);
    void insertarRaiz(const T& e);
    void insertarHijoIzqdo(nodo n, const T& e);
    void insertarHijoDrcho(nodo n, const T& e);
    void eliminarHijoIzqdo(nodo n);
    void eliminarHijoDrcho(nodo n);
    void eliminarRaiz();

    bool vacio() const;
    size_t tama() const;
    size_t tamaMax() const; // Requerido por la implementación
    const T& elemento(nodo n) const; // Lec. en Abin const
    T& elemento(nodo n); // Lec/Esc. en Abin no-const
    // operaciones ejercicio 4 practica 1
    T altura(nodo n) const;
    T profundidad(nodo n) const;

    nodo raiz() const;
    nodo padre(nodo n) const;
    nodo hijoIzqdo(nodo n) const;
    nodo hijoDrcho(nodo n) const;
    Abin(const Abin& A); // Ctor. de copia
    Abin& operator =(const Abin& A); // Asig. de árboles
    Abin~();

private:
    struct celda {
        T elto;
        nodo padre, hizq, hder;
    };
    celda* nodos; // Vector de celdas 
    size_t maxNodos, // Tamaño del vector
    numNodos; // Tamaño del árbol
    nodo libre; // Lista de celdas libres

    // Método privado que verifica si la posicion es válida
    bool valido(nodo n)const;
}

// Declaracion nodo nulo + métodos privados
template <typename T>
const typename Abin<T>::nodo Abin<T>::NODO_NULO{SIZE_MAX};

template <typename T>
inline bool Abin<T>::valido(nodo n)const {
    return !vacio() && n < maxNodos && (n == 0 || nodos[n].padre != NODO_NULO); // o bien no pertenezca al vector o este lleno, o que la celda sea ocupada
}

template <typename T>
inline Abin<T>::Abin(size_t maxNodos) : nodos{new celda[maxNodos]}, maxNodos{maxNodos}, numNodos{0} {
    if (maxNodos > 1){ // implementamos la lista de celdas
        libre = 1;
        for (nodo n = 1; n < maxNodos; n++){ // empezamos desde la pos 1 pues la 0 esta destinada para el nodo raíz
            nodos[n].hizq = n + 1;
            // colocamos todas las celdas como libres
            nodos[n].padre = NODO_NULO;
        }
    }
}

template <typename T>
inline void Abin<T>::insertarRaiz(const T& e) {
    assert(maxNodos > 0);
    assert(vacio());
    nodos[0] = {e, NODO_NULO, NODO_NULO, NODO_NULO};
    numNodos = 1;
}

template <typename T>
inline void Abin<T>::insertarHijoIzqdo(nodo n, const T& e) {
    assert(tama() < tamaMax());
    assert(valido(n));
    assert(nodos[n].hizq == NODO_NULO);
    // Sacar la primera celda de la lista de libres.
    nodo hizqdo = libre;
    libre = nodos[libre].hizq;
    // Añadir el nuevo nodo.
    nodos[n].hizq = hizqdo;
    nodos[hizqdo] = {e, n, NODO_NULO, NODO_NULO};
    ++numNodos;
}

template <typename T>
inline void Abin<T>::insertarHijoDrcho(nodo n, const T& e){
    assert(tama() < tamaMax());
    assert(valido(n));
    assert(nodos[n].hder == NODO_NULO);
    // Sacar la primera celda de la lista de libres.
    nodo hdrcho = libre;
    libre = nodos[libre].hizq;
    // Añadir el nuevo nodo.
    nodos[n].hder = hdrcho;
    nodos[hdrcho] = {e, n, NODO_NULO, NODO_NULO};
    ++numNodos;
}

template <typename T>
inline void Abin<T>::eliminarHijoIzqdo(nodo n) {
    assert(valido(n));
        nodo hizqdo = nodos[n].hizq;
    assert(hizqdo != NODO_NULO); // Existe hijo izqdo.
    assert(nodos[hizqdo].hizq == NODO_NULO && // y es
        nodos[hizqdo].hder == NODO_NULO); // hoja.
        nodos[n].hizq = NODO_NULO;
        // Añadir hizqdo al inicio de la lista de libres.
        nodos[hizqdo].hizq = libre;
        libre = hizqdo;
        nodos[hizqdo].padre = NODO_NULO; // Marcar celda libre.
        --numNodos;
}

template <typename T>
inline void Abin<T>::eliminarHijoIzqdo(nodo n){
    assert(valido(n));
    nodo hizqdo = nodos[n].hizq;
    assert(hizqdo != NODO_NULO); // Existe hijo izqdo.
    assert(nodos[hizqdo].hizq == NODO_NULO && // y es
        nodos[hizqdo].hder == NODO_NULO); // hoja.
        nodos[n].hizq = NODO_NULO;
        // Añadir hizqdo al inicio de la lista de libres.
        nodos[hizqdo].hizq = libre;
        libre = hizqdo;
        nodos[hizqdo].padre = NODO_NULO; // Marcar celda libre.
        --numNodos;
}

template <typename T>
inline void Abin<T>::eliminarRaiz(){
    assert(numNodos == 1);
    numNodos = 0;
}

template <typename T>
inline bool Abin<T>::vacio() const{
    return numNodos == 0; 
}

template <typename T>
inline size_t Abin<T>::tama() const{
    return numNodos; 
}

template <typename T>
inline size_t Abin<T>::tamaMax() const{
    return maxNodos; 
}

template <typename T>
inline const T& Abin<T>::elemento(nodo n) const{
 assert(valido(n));
    return nodos[n].elto;
}

template <typename T>
inline T& Abin<T>::elemento(nodo n){
 assert(valido(n));
    return nodos[n].elto;
}

template <typename T>
inline typename Abin<T>::nodo Abin<T>::raiz() const{
    return vacio() ? NODO_NULO : 0;
}

template <typename T>
inline typename Abin<T>::nodo Abin<T>::padre(nodo n) const{
assert(valido(n));
    return nodos[n].padre;
}

template <typename T>
inline typename Abin<T>::nodo Abin<T>::hijoIzqdo(nodo n) const{
assert(valido(n));
    return nodos[n].hizq;
}

template <typename T>
inline typename Abin<T>::nodo Abin<T>::hijoDrcho(nodo n) const{
assert(valido(n));
    return nodos[n].hder;
}

// constructor asignador de copia

template <typename T>
Abin<T>::Abin(const Abin& A) : Abin{A.maxNodos} {
if (!A.vacio()) {
    for (nodo n = 0; n < maxNodos; ++n)
        nodos[n] = A.nodos[n];
    numNodos = A.numNodos;
    libre = A.libre;
 }
}

template <typename T>
inline Abin<T>& Abin<T>::operator =(const Abin& A){
    Abin B{A};
    std::swap(nodos, B.nodos);
    std::swap(maxNodos, B.maxNodos);
    std::swap(numNodos, B.numNodos);
    std::swap(libre, B.libre);
        return *this;
}


template <typename T>
inline Abin<T>::Abin~(){
    delete[] nodos;
}

#endif // ABIN_VEC0_H