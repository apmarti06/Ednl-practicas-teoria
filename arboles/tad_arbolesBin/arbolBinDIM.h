#ifndef ABIN_H
#define ABIN_H
#include <cassert> // verifica precondiciones
#include <cstddef> // size_t
#include <utility> // swap


template <typename T> class Abin {
// declaracion adelantada para usar en miembros publicos
    struct celda;
public:
    typedef celda* nodo;
    static const nodo NODO_NULO;
    // constructor, constructor de copia, asignador de copia y destructor
    Abin();
    Abin(const Abin& A); 
    Abin& operator=(const Abin& A);
    ~Abin();

    // operaciones modificadoras
    void insertarRaiz(const T& e);
    void insertarHizq(nodo n, const T& e);
    void insertarHder(nodo n, const T& e);
    void eliminarHizq(nodo n);
    void eliminarHder(nodo n);
    void eliminarRaiz();

    // operaciones consultoras
    bool vacio() const;
    size_t tama() const;
    const T& elemento(nodo n) const; // excepcion funcion no-const
    T& elemento(nodo n);
    nodo raiz() const;
    nodo padre(nodo n) const;
    nodo hijoIzq(nodo n) const;
    nodo hijoDer(nodo n) const;
    //ejercicio 5 practica 1
    int altura(nodo n) const;
    int profundidad(nodo n) const;

private:
    struct celda{
        T elto;
        nodo padre, hizq, hder;
    };

    nodo r; // raiz del arbol del que se parte (ademas tendremos acceso al resto de nodos)
    size_t num_nodos; // para facilita la implementacion
    
    // metodos privados que usamos en funciones
    static nodo copiar(nodo n);
    static void destruir(nodo n);
};

// implementacion usando inline

// definimos primero nodo nulo
template <typename T>
const typename Abin<T>::nodo Abin<T>::NODO_NULO{nullptr};

template <typename T>
inline Abin<T>::Abin() : r{NODO_NULO} , num_nodos{0} {}

// ejercicio 4/5 practica 1
template <typename T> // metodo privado de alturaRec
int Abin<T>::altura(nodo n) const{
    if (vacio()){
        int izq, der;
        if (n == NODO_NULO){
            return -1;
        }
        izq = altura(n->hizq);
        der =  altura(n->hder);
        
        return 1 + (izq > der ? izq : der);
    }
    return 0;
}

template <typename T>
inline int Abin<T>::profundidad(nodo n) const{
    if (n == NODO_NULO){
        return 0; // se ha llegado ya al nodo raíz
    } else {
        return 1 + (profundidad(n->padre)); 
    }
}

// resto de operaciones

template <typename T> // constructor de copia
inline Abin<T>::Abin(const Abin& A) {
    // creamos el árbol al completo rellenandose sus atributos
    r = copiar(A.r); // le pasamos el nodo raíz para copiar todos sus descendientes (subárboles)
    num_nodos = A.num_nodos;
}

template <typename T>
typename Abin<T>::nodo Abin<T>::copiar(nodo n){
    nodo m = NODO_NULO;
    if (n != NODO_NULO){
        Abin A; // contiene nodos copiados
        A.r = new celda{n->elto};
        A.r->hizq = copiar(n->hizq);
        if (A.r->hizq != NODO_NULO) A.r->hizq->padre = A.r; // apuntamos al nodo padre
        A.r->hder = copiar(n->hizq);
        if (A.r->hder != NODO_NULO) A.r->hder->padre = A.r; // apuntamos al nodo padre
        // actualizamos para que no se colasionen todos los nodos
        m = A.r;
        A.r = NODO_NULO;
    }
    return m;
}

template <typename T> // operador de asignacion
inline Abin<T>& Abin<T>::operator=(const Abin& A){
    Abin B{A};
    // igual que el constructor de copia
    std::swap(r, B.r);
    std::swap(num_nodos, B.num_nodos);
    return *this;
}

template <typename T> // destructor
inline Abin<T>::~Abin(){
    destruir(r);
}

template <typename T> // destruir metodo privado
void Abin<T>::destruir(nodo& n){
    if (n != NODO_NULO){
        destruir(n->hizq);
        destruir(n->hder);
        // aseguramos que no quede basura al borrar el nodo
        delete n;
        n = NODO_NULO;
    }
}

// operaciones modificadoras

template <typename T>
inline void Abin<T>::insertarRaiz(const T& e){
    assert(vacio());
    r = new celda{e}; // no declaramos el resto de parametros pues los punteros se hacen a algún sitio
    num_nodos = 1;
}

template <typename T>
inline void Abin<T>::insertarHizq(nodo n ,const T& e){
    // condiciones, tener una raiz, el nodo n existe y no tiene hijo_izquierdo
    assert(!vacio());
    assert(n != NODO_NULO);
    assert(n->hizq == NODO_NULO); 

    n->hizq = new celda{e, n}; // indicamos que su nodo padre es n, apuntando hacia este, y su padre al hijo (n->izq)
    num_nodos++;
}

template <typename T>
inline void Abin<T>::insertarHder(nodo n ,const T& e){
    // condiciones, tener una raiz, el nodo n existe y no tiene hijo_izquierdo
    assert(!vacio());
    assert(n != NODO_NULO);
    assert(n->hder == NODO_NULO);

    n->hder = new celda{e, n}; // indicamos que su nodo padre es n, apuntando hacia este, y su padre al hijo (n->hder)
    num_nodos++;
}

template <typename T>
inline void Abin<T>::eliminarHizq(nodo n){
    assert(n != NODO_NULO);
    assert(n->hizq != NODO_NULO); // que exista nodo nulo y que este sea hoja
    assert(n->hizq->hizq == NODO_NULO && n->hizq->hder == NODO_NULO); 

    // eliminamos dicho nodo, y ponemos esa zona de memoria a nullptr para que no apunte a basura
    delete n->hizq; 
    n->hizq = NODO_NULO;
    --num_nodos;
}

template <typename T>
inline void Abin<T>::eliminarHder(nodo n){
    assert(n != NODO_NULO);
    assert(n->hder != NODO_NULO); // que exista nodo nulo y que este sea hoja
    assert(n->hder->hizq == NODO_NULO && n->hder->hder == NODO_NULO); 

    // eliminamos dicho nodo, y ponemos esa zona de memoria a nullptr para que no apunte a basura
    delete n->hder; 
    n->hder = NODO_NULO;
    --num_nodos;
}

template <typename T>
inline void Abin<T>::eliminarRaiz(){
    assert(num_nodos == 1); // verificamos directamente que solo quede un nodo (el raiz)
    delete r;
    r = NODO_NULO;
    num_nodos = 0;
}

// operaciones de miembros publicos consultoras
template <typename T>
inline bool Abin<T>::vacio() const{
    return num_nodos == 0;
}

template <typename T>
inline size_t Abin<T>::tama() const{
    return num_nodos;
}

template <typename T>
inline const T& Abin<T>::elemento(nodo n) const {
    assert(n != NODO_NULO);
    return n->elto;
}

template <typename T>
inline T& Abin<T>::elemento(nodo n) {
    assert(n != NODO_NULO);
    return n->elto;
}

template <typename T> 
inline typename Abin<T>::nodo Abin<T>::raiz() const{
    assert(!vacio());
    return r;
}

template <typename T>
inline typename Abin<T>::nodo Abin<T>::padre(nodo n) const{ 
    assert(n != NODO_NULO);
    return n->padre;
}

template <typename T>
inline typename Abin<T>::nodo Abin<T>::hijoIzq(nodo n) const{
    assert(n != NODO_NULO);
    return n->hizq;
}

template <typename T>
inline typename Abin<T>::nodo Abin<T>::hijoDer(nodo n) const{
    assert(n != NODO_NULO);
    return n->hder;
}



#endif 