#ifndef AGEN_H
#define AGEN_H
#include <cassert>
#include <cstddef> // size_t
#include <utility> // swap

template <typename T>
    
class Agen {
    struct celda;
public:
    typedef celda* nodo;
    // constructores, asignador de copia, destructor
    Agen();
    Agen(const Agen& A);
    Agen& operator=(const Agen& A);
    ~Agen();

    // Operaciones + un miembro clase Agen
    static const nodo NODO_NULO; 
    void insertarRaiz(const T& e);
    void insertarHijoIzqdo(nodo n, const T& e);
    void insertarHermDrcho(nodo n, const T& e);
    void eliminarHijoIzqdo(nodo n);
    void eliminarHermDrcho(nodo n);
    void eliminarRaiz();

    // Metodos de consulta const y no-const
    bool vacio() const;
    size_t tama() const;
    const T& elemento(nodo n) const; // lectura
    T& elemento(nodo n); // lectura-escritura
    nodo raiz() const;
    nodo padre(nodo n) const;
    nodo hijoIzqdo(nodo n) const;
    nodo hermDrcho(nodo n) const;

private: 
    struct celda {
        T elto;
        nodo padre, hizq, heder;
    };

    nodo r; // nodo raíz 
    size_t num_nodos; //cont nodos

    // Metodos privados
    nodo copiar(nodo n);
    void destruir(nodo& n);
};

// Declaracion constante NODO_NULO
template <typename T>
const typename Agen<T>::nodo Agen<T>::NODO_NULO{nullptr};

// Métodos privados + operaciones internas del Agen
template <typename T>
inline Agen<T>::Agen() : r{NODO_NULO}, num_nodos{0} {}

// Constructor de copia
template <typename T>
inline typename Agen<T>::nodo Agen<T>::copiar(nodo n){
    nodo m = NODO_NULO;
    if (n != NODO_NULO){
        // copiamos el árbol desde cero
        Agen A;
        A.r = new celda{n->elto};
        if (n->hizq != NODO_NULO){
            // copiamos nuevos subarboles, más el izquierdo, le mandamos un puntero al padre
            A.r = copiar(n->hizq);
            A.r->hizq->padre = A.r;
            // copiamos el resto de nodos a la derecha (desde el izq a todos)
            nodo hijo = A.r->hizq;
            nodo hermano = n->hizq->heder;
            while (hermano != NODO_NULO){
                hijo = hijo->heder = copiar(hermano);
                hijo->padre = A.r;
                hermano = hermano->heder;
            }
        }
        m = A.r; A.r = NODO_NULO;
    }
    return m;
}

template <typename T>
inline Agen<T>::Agen(const Agen& A) {
    r = copiar(A.r);
    num_nodos = A.num_nodos;
}

template <typename T>
void Agen<T>::destruir(nodo& n){
nodo hedrcho;
    if (n != NODO_NULO){
        while (n->hizq != NODO_NULO){ // Destruir hijos
            hedrcho = n->hizq->heder;
            destruir(n->hizq);
            n->hizq = hedrcho;
        }
        delete n;
        n = NODO_NULO;
    }
}

template <typename T>
inline Agen<T>::~Agen() {
    destruir(r); // vaciar el árbol
}

template <typename T>
inline Agen<T>& Agen<T>::operator =(const Agen& A){
    Agen B{A};
    std::swap(r, B.r);
    std::swap(num_nodos, B.num_nodos);
    return *this;
}

// Operaciones del TAD

template <typename T>
inline void Agen<T>::insertarRaiz(const T& e){
    assert(vacio());
    r = new celda{e};
    num_nodos = 1;
}

template <typename T>
inline void Agen<T>::insertarHijoIzqdo(nodo n, const T& e){
    assert(n != NODO_NULO);
    // añadimos el nodo e incrementamos el numero de nodos, donde n apunta al nuevo
    n->hizq = new celda{e, n, NODO_NULO, n->hizq};
    num_nodos++;   
}

template <typename T>
inline void Agen<T>::insertarHermDrcho(nodo n, const T& e){
    assert(n != NODO_NULO);
    assert(n != r); // n no es la raíz
    // añadimos el nodo e incrementamos el numero de nodos
    n->heder = new celda{e, n->padre, NODO_NULO, n->heder}; // n apunta a un nuevo nodo, que es el hermano derecho de n, y el hermano derecho de n pasa a ser el hermano derecho del nuevo nodo
    num_nodos++;   
}

template <typename T>
inline void Agen<T>::eliminarHijoIzqdo(nodo n){
    // verificamos que sea hoja y que exista en el árbol
    nodo hizqdo;
    assert(n != NODO_NULO);
    hizqdo = n->hizq;
    assert(hizqdo != NODO_NULO && hizqdo->hizq == NODO_NULO); // Existe hijo izqdo. y es hoja.
    // hacemos que n apunte al hermano del hijo izquierdo y lo borramos (como no apuntamos a esa direccion de memoria no hacemos nullptr)
    n->hizq = hizqdo->heder; 
    delete hizqdo;
    num_nodos--;
}

template <typename T>
inline void Agen<T>::eliminarHermDrcho(nodo n){
    nodo hedrcho;
    assert(n != NODO_NULO);
    hedrcho = n->heder;
    assert(hedrcho != NODO_NULO && hedrcho->hizq == NODO_NULO); // Existe hermano drcho y es hoja
    
    // El hermano del hermano se convierte en el nuevo hermano drcho de n, como n apunta al nuevo hermano, no hacemos nullptr
    n->heder = hedrcho->heder;
    delete hedrcho;
    --num_nodos;
}

template <typename T>
inline void Agen<T>::eliminarRaiz(){
    assert(num_nodos == 1);
    delete(r);
    r = NODO_NULO;
    num_nodos = 0;
}

// operaciones simples de consultas
template <typename T>
inline bool Agen<T>::vacio() const {
    return r == NODO_NULO;
}

template <typename T>
inline size_t Agen<T>::tama() const {
    return num_nodos;
}

// Implementamos los métodos de consulta const y no-const, donde el método de consulta const devuelve una referencia constante al elemento, y el método de consulta no-const devuelve una referencia al elemento, para poder modificarlo
template <typename T>
inline const T& Agen<T>::elemento(nodo n) const {
    assert(n != NODO_NULO);
    return n->elto;
}

template <typename T>
inline T& Agen<T>::elemento(nodo n) {
    assert(n != NODO_NULO);
    return n->elto;
}

template <typename T>
inline typename Agen<T>::nodo Agen<T>::raiz() const {   
    return r;
}

template <typename T>
inline typename Agen<T>::nodo Agen<T>::padre(nodo n) const {
    assert(n != NODO_NULO);
    return n->padre;
}

template <typename T>
inline typename Agen<T>::nodo Agen<T>::hijoIzqdo(nodo n) const {
    assert(n != NODO_NULO);
    return n->hizq;
}

template <typename T>
inline typename Agen<T>::nodo Agen<T>::hermDrcho(nodo n) const  {
    assert(n != NODO_NULO);
    return n->heder;
}






#endif // AGEN_H
