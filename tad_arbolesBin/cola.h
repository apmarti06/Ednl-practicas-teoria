#ifndef BICOLA_ENLA_H
#define BICOLA_ENLA_H

#include <cassert>
#include <cstddef> // size_t
#include <utility> // swap

template <typename T> class bicola {
    public:
        bicola(); // constructor 
        bicola(const bicola& B); // constructor de copia
        bicola& operator=(const bicola& B); // asignacion entre bicolas
        ~bicola(); // destructor - IMPORTANTE AÑADIR

        bool vacia() const; 
        size_t tama() const;
        
        // funciones en la izquierda
        const T& izquierda_tope() const;
        void pop_L();
        void push_L(const T& x); 
        
        // funciones en la derecha
        const T& derecha_tope() const;
        void pop_D();
        void push_D(const T& x); 

    private:
        struct nodo {
            T elem;
            nodo* sig;
            nodo(const T& e, nodo* p = nullptr) : elem(e), sig(p) {}
        };
        // simulamos la cola por tramos
        
        nodo* izq; // extremo izquierdo
        nodo* dere; // extremo derecho
        size_t n_elem;
        
        // Método auxiliar para copiar
        void copiar(const bicola& B);
        // Método auxiliar para liberar memoria
        void liberar();
};

// IMPLEMENTACIÓN

// Constructor
template <typename T>
inline bicola<T>::bicola() : izq(nullptr), dere(nullptr), n_elem(0) {}

// Destructor
template <typename T>
inline bicola<T>::~bicola() {
    liberar();
}

// Vacía
template <typename T>
inline bool bicola<T>::vacia() const {
    return n_elem == 0;
}

// Tamaño
template <typename T>
inline size_t bicola<T>::tama() const {
    return n_elem;
}

// Método auxiliar para liberar memoria
template <typename T>
void bicola<T>::liberar() {
    while (!vacia()) {
        pop_L();
    }
}

// Método auxiliar para copiar
template <typename T>
void bicola<T>::copiar(const bicola& B) {
    if (!B.vacia()) {
        izq = dere = new nodo(B.izq->elem);
        for (nodo* p = B.izq->sig; p; p = p->sig) {
            dere = dere->sig = new nodo(p->elem);
        }
        n_elem = B.n_elem;
    } else {
        izq = dere = nullptr;
        n_elem = 0;
    }
}

// Constructor de copia
template <typename T>
inline bicola<T>::bicola(const bicola& B) : izq(nullptr), dere(nullptr), n_elem(0) {
    copiar(B);
}

// Operador de asignación (usando copy-and-swap)
template <typename T>
inline bicola<T>& bicola<T>::operator=(const bicola& B) {
    if (this != &B) {
        bicola<T> aux(B); // Crear copia temporal
        std::swap(izq, aux.izq);
        std::swap(dere, aux.dere);
        std::swap(n_elem, aux.n_elem);

    }
    return *this;
}

// Izquierda tope
template <typename T>
inline const T& bicola<T>::izquierda_tope() const {
    assert(!vacia());
    return izq->elem;
}

// Derecha tope
template <typename T>
inline const T& bicola<T>::derecha_tope() const {
    assert(!vacia());
    return dere->elem;
}

// Push izquierda
template <typename T>
inline void bicola<T>::push_L(const T& x) {
    nodo* nuevo = new nodo(x, izq); // el nodo nuevo, apunta al nodo sig osea a la antigua izq
    if (vacia()) {
        izq = dere = nuevo; // primer elemento
    } else {
        izq = nuevo; // CORREGIDO: actualizar izq siempre
    }
    n_elem++;
}

// Push derecha
template <typename T>
inline void bicola<T>::push_D(const T& x) {
    nodo* nuevo = new nodo(x);
    if (vacia()) {
        izq = dere = nuevo; // primer elemento
    } else {
        dere->sig = nuevo;
        dere = nuevo;
    }
    n_elem++;
}

// Pop izquierda
template <typename T>
inline void bicola<T>::pop_L() {
    assert(!vacia());
    nodo* eliminar = izq;

    if (izq == dere) { // último elemento
        izq = dere = nullptr;
    } else {
        izq = izq->sig;
    }
    
    delete eliminar;
    n_elem--;
}

template <typename T>
inline void bicola<T>::pop_D() {
    assert(!vacia());
    nodo* eliminar = dere; 

    if (izq == dere) { // último elemento
        izq = dere = nullptr;
    } else {
        // Buscar el penúltimo nodo
        nodo* actual = izq;
        while (actual->sig != dere) {
            actual = actual->sig;
        }
        // Actualizar punteros
        actual->sig = nullptr;
        dere = actual;
    }
    
    delete eliminar; // CORREGIDO: eliminar el nodo correcto
    n_elem--;
}

#endif // BICOLA_ENLA_H