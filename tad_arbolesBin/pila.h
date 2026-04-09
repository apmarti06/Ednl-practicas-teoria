// pila_enlazada.h
#ifndef PILA_ENLAZADA_H
#define PILA_ENLAZADA_H

#include <cassert>
#include <cstddef>

template <typename T>
class pila{
    public:
    // operadores de pila (constructor, destructor, operador de copia =)
    pila();
    pila(const pila& P);
    ~pila();
    pila& operator=(const pila& P);
    
    bool vacia() const;
    size_t tama() const;
    const T& tope() const;
    void pop();
    void push(const T& x);

    private:
        struct nodo {
            T elem_;
            nodo* sig;
            nodo(const T& e, nodo* p = nullptr) : elem_(e) , sig(p) {}
        };

    nodo* tope_;
    size_t n_elementos;
};

template <typename T>
inline pila<T>::pila() : tope_(nullptr) , n_elementos(0) {}

template <typename T>
inline bool pila<T>::vacia() const {
    return (n_elementos == 0);
}

template <typename T>
inline size_t pila<T>::tama() const {
    return n_elementos;
}

template <typename T>
inline const T& pila<T>::tope() const {
    assert(!vacia());
    return tope_->elem_;
}

template <typename T>
inline void pila<T>::pop(){
    assert(!vacia());
    // salvaguardamos el tope_ actualizandolo
    nodo* p = tope_;
    tope_ = tope_->sig;

    delete p;
    n_elementos--;
}

template <typename T>
inline void pila<T>::push(const T& x){
    tope_ = new nodo(x, tope_);
    n_elementos++;
}

template <typename T> // constructor de copia
inline pila<T>::pila(const pila& P) : pila() {
    if (!P.vacia()){
        tope_ = new nodo(P.tope());
        nodo* p = tope_;
        nodo* q = P.tope_->sig;
        while (q){
            p->sig = new nodo(q->elem_);
            p = p->sig;
            q = q->sig;
        }
        n_elementos = P.elem_;
    }
}

template <typename T> //operador de asignacion
inline pila<T>& pila<T>::operator=(const pila& Q){
    pila P(Q);
    //intercambiamos los elementos
    nodo* t = tope_;
    tope_ = Q.tope_;
    Q.tope_ = t;
    size_t n = n_elementos;
    n_elementos = Q.n_elementos;
    Q.n_elementos = n;
    return *this;
}

template <typename T>
inline pila<T>::~pila(){
    nodo* p;
    while (tope_){
        p = tope_->sig;
        delete tope_;
        tope_ = p;
    }
    n_elementos = 0;
}

#endif 