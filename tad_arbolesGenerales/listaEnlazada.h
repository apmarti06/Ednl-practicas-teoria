#ifndef LISTA_ENLAZADA_H
#define LISTA_ENLAZADA_H

#include <cstddef>  // size_t
#include <utility>  // swap
#include <cassert>

template <typename T>
class Lista {
    struct nodo; // Declaración adelantada (privada)

public:
    typedef nodo* posicion; // "posición" = puntero al nodo anterior

    Lista();                          // ctor (requiere T())
    Lista(const Lista& Lis);          // ctor copia (requiere T copiable)
    Lista& operator=(const Lista& Lis); // asignación (requiere T copiable)
    ~Lista();                         // dtor

    bool vacia() const;
    size_t tama() const;

    void insertar(const T& x, posicion p); // insertar x DESPUÉS de p
    void eliminar(posicion p);             // elimina el nodo DESPUÉS de p

    const T& elemento(posicion p) const; // elemento que está DESPUÉS de p
    T& elemento(posicion p);

    posicion siguiente(posicion p) const; // siguiente posición (avanza una)
    posicion anterior(posicion p) const;  // ÚNICA Θ(n)
    posicion primera() const;             // posición de a1 (es la cabecera)
    posicion fin() const;                 // posición "fin" (es L)

private:
    struct nodo {
        T elto;
        nodo* sig;
        nodo(const T& e = T(), nodo* p = nullptr) : elto(e), sig(p) {}
    };

    nodo* L;    // puntero al ÚLTIMO nodo (estructura circular con cabecera)
    size_t n;   // longitud
};

// Constructor: lista vacía => L es la cabecera y L->sig = L (circular)
template <typename T>
inline Lista<T>::Lista() : L(new nodo), n(0) {
    L->sig = L;
}

template <typename T>
inline bool Lista<T>::vacia() const {
    return n == 0;
}

template <typename T>
inline size_t Lista<T>::tama() const {
    return n;
}

// insertar x DESPUÉS de p
template <typename T>
inline void Lista<T>::insertar(const T& x, posicion p) {
    p->sig = new nodo(x, p->sig);
    if (p == fin()) {
        L = p->sig; // nuevo último
    }
    ++n;
}

// eliminar el nodo DESPUÉS de p
template <typename T>
inline void Lista<T>::eliminar(posicion p) {
    assert(p != fin());          // no se puede eliminar "después" de fin()
    posicion q = p->sig;         // nodo a borrar
    if (q == fin()) {
        L = p;                   // el nuevo último es el penúltimo
    }
    p->sig = q->sig;
    delete q;
    --n;
}

// elemento DESPUÉS de p
template <typename T>
inline const T& Lista<T>::elemento(posicion p) const {
    assert(p != fin());
    return p->sig->elto;
}

// siguiente posición: avanzar una
template <typename T>
inline typename Lista<T>::posicion Lista<T>::siguiente(posicion p) const {
    assert(p != fin());
    return p->sig;
}

// anterior: ÚNICA operación Θ(n)
template <typename T>
typename Lista<T>::posicion Lista<T>::anterior(posicion p) const {
    assert(p != primera());
    posicion q = primera();
    while (q->sig != p) q = q->sig; // apunta al siguiente nodo, cuando el siguiente sea p quiere decir que el actual es el anterior a p
    return q;
}

// primera posición: la cabecera (que es L->sig)
template <typename T>
inline typename Lista<T>::posicion Lista<T>::primera() const {
    return L->sig;
}

// fin: el puntero al último (Θ(1))
template <typename T>
inline typename Lista<T>::posicion Lista<T>::fin() const {
    return L;
}

// Constructor de copia
template <typename T>
Lista<T>::Lista(const Lista& Lis) : Lista() {
    if (!Lis.vacia()) {
        // Recorremos nodos reales (desde cabecera->sig hasta volver a cabecera)
        for (nodo* p = Lis.L->sig; p != Lis.L; p = p->sig) {
            L = L->sig = new nodo(p->sig->elto, L->sig);
        }
        n = Lis.n;
    }
}

// Asignación (copy-and-swap)
template <typename T>
inline Lista<T>& Lista<T>::operator=(const Lista& Lis) {
    Lista L1(Lis);
    std::swap(L, L1.L);
    std::swap(n, L1.n);
    return *this;
}

// Destructor: vacía la lista y destruye la cabecera
template <typename T>
Lista<T>::~Lista() {
    nodo* p;
    while (L != L->sig) { // mientras queden nodos reales
        p = L->sig;
        L->sig = p->sig;
        delete p;
    }
    delete L; // borrar cabecera (y último cuando está vacía)
}

#endif // LISTA_ENLAZADA_H
