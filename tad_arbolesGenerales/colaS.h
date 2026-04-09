#ifndef COLA_CIRCULAR_H
#define COLA_CIRCULAR_H

#include <cassert>
#include <cstddef> // size_t
#include <utility> // swap
#include <iostream>

template <typename T> class cola {
    public:
        cola(size_t tamMax = 0); // constructor
        cola(const cola& C); // Ctor. de copia
        cola& operator=(const cola& C); // Asignación
        ~cola(); // destructor (CORRECCIÓN: ~cola() no cola~())

        // funciones de la especificación 
        bool vacia() const; 
        size_t tama() const;
        size_t tamMax() const;
        const T& frente() const;
        T& frente(); // versión no constante
        void pop();
        void push(const T& x); 

    private:
        T* elementos;
        size_t Lmax, inicio_;
        size_t n_elem; // si es 0 está vacía
}; 

template <typename T>
inline cola<T>::cola(size_t tamMax) : elementos(new T[tamMax]), Lmax(tamMax), inicio_(0), n_elem(0) {}

template <typename T>
inline cola<T>::cola(const cola& C) : elementos(new T[C.Lmax]), Lmax(C.Lmax), inicio_(0), n_elem(0) {
    if (!C.vacia()){
        n_elem = C.n_elem;
        for (size_t i = 0; i < n_elem; i++){
            elementos[i] = C.elementos[(C.inicio_ + i) % C.Lmax];
        }
    }
}

template <typename T>
inline cola<T>& cola<T>::operator=(const cola& C) {
    if (this != &C) {
        cola<T> temp(C);
        std::swap(elementos, temp.elementos);
        std::swap(Lmax, temp.Lmax);
        std::swap(inicio_, temp.inicio_);
        std::swap(n_elem, temp.n_elem);
    }
    return *this;
}

template <typename T>
inline cola<T>::~cola() { // CORRECCIÓN: ~cola() no cola~()
    delete[] elementos; // CORRECCIÓN: delete[] no delete[]
}

template <typename T>
inline bool cola<T>::vacia() const {
    return n_elem == 0;
}

template <typename T>
inline size_t cola<T>::tama() const {
    return n_elem;
}

template <typename T>
inline size_t cola<T>::tamMax() const {
    return Lmax;
}

template <typename T>
inline const T& cola<T>::frente() const {
    assert(!vacia());
    return elementos[inicio_];
}

template <typename T>
inline void cola<T>::pop() {
    assert(!vacia()); // CORRECCIÓN: !vacia() no !n_elem
    inicio_ = (inicio_ + 1) % Lmax;
    n_elem--;
}

template <typename T>
inline void cola<T>::push(const T& x) {
    assert(n_elem < Lmax);
    elementos[(inicio_ + n_elem) % Lmax] = x;
    n_elem++;
}

#endif