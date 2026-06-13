#ifndef AGEN_LIS_H
#define AGEN_LIS_H

#include <cassert>
#include <cstddef> // size_t
#include <cstdint> // SIZE_MAX, máx. de size_t
#include <utility> // swap 
#include "listaEnlazada.h" // implementamos la lista para conocer hijos

template <typename T> class Agen {
public:
    typedef size_t nodo;
    static const nodo NODO_NULO;

    explicit Agen(size_t maxNodos = 0);
    Agen(const Agen& A); // Ctor. de copia
    Agen& operator =(const Agen& A); // Asignación de árboles
    ~Agen(); // Destructor

    void insertarRaiz(const T& e);
    void insertarHijoIzqdo(nodo n, const T& e);
    void insertarHermDrcho(nodo n, const T& e);
    void eliminarHijoIzqdo(nodo n);
    void eliminarHermDrcho(nodo n);
    void eliminarRaiz();
    bool vacio() const;

    size_t tama() const;
    size_t tamaMax() const; // Requerido por la implementación
    const T& elemento(nodo n) const; // Lectura en const Agen
    T& elemento(nodo n); // Lectura/escritura en Agen (no-const)
    nodo raiz() const;
    nodo padre(nodo n) const;
    nodo hijoIzqdo(nodo n) const;
    nodo hermDrcho(nodo n) const;

private:
    struct celda {  // guardamos en el registro, quien es su padre y sus hijos
        T elto;
        nodo padre;
        Lista<nodo> hijos;
    };
    // atributos vector nodos, tama, lista de nodos libres
    celda* nodos;
    size_t numNodos, max_Nodos;
    nodo libre;

    bool valido(nodo n) const;
};

// Definición del nodo nulo
template <typename T>
const typename Agen<T>::nodo Agen<T>::NODO_NULO{SIZE_MAX};

template <typename T>
inline bool Agen<T>::valido (nodo n) const{
    return !vacio() && n < maxNodos && (n == 0 || nodos[n].padre != NODO_NULO);
}

template <typename T>
Agen<T>::Agen(size_t maxNodos) : nodos{new celda[maxNodos]}, numNodos{0}, max_Nodos{maxNodos} 
{
    if (maxNodos > 1){ // creamos listas de celdas libres
        libre = 1;
        for (nodo n = 1; n < maxNodos; n++){
            Lista<nodo>& Lh = nodos[n].hijos;
            Lh.insertar(n + 1, Lh.primera())
            #ifndef NDEBUG
            // Sólo para comprobar precondiciones durante depuración.
            nodos[n].padre = NODO_NULO; // Marcar celda libre.
            #endif
        }
    }
}

template <typename T>
Agen<T>::Agen(const Agen& A) : Agen{A.max_Nodos}{
    if (!A.vacio()){
        for (nodo n = 0; n < max_Nodos; n++){
            nodos[n] = A.nodos[n];
        }
        numNodos = A.numNodos;
        libre = A.libre;
    }
}

template <typename T>
inline Agen<T>& Agen<T>::operator =(const Agen& A){
    Agen B{A};
    std::swap(nodos, B.nodos);
    std::swap(maxNodos, B.maxNodos);
    std::swap(numNodos, B.numNodos);
    std::swap(libre, B.libre);
    return *this;
}

template <typename T>
inline Agen<T>::~Agen(){
    delete[] nodos;
}

template <typename T>
inline void Agen<T>::insertarRaiz(const T& e) {
    assert(maxNodos > 0);
    assert(vacio());
    nodos[0] = {e, NODO_NULO}; // Asigna lista de hijos vacía.
    numNodos = 1;
}

template <typename T>
inline void Agen<T>::insertarHijoIzqdo(nodo n, const T& e){
    // verificamos que se pueda seguir introduciendo, y que la pos del nodo n sea válida
    assert(tama() < tamaMax());
    assert(valido(n));

    nodo hizqdo = libre; // le asignamos la pos libre
    libre = nodos[libre].hijos.elemento(nodos[libre].hijos.primera());
    // Asignamos al vector el nuevo hijo
    nodos[hizqdo] = {e, n};
    Lista<nodo>& Lh = nodos[n].hijos; // Lista de hijos de n.
    Lh.insertar(hizqdo, Lh.primera());
    ++numNodos;
}

template <typename T>
inline void Agen<T>::insertarHermDrcho(nodo n, const T& e){
    // verificamos que se pueda seguir introduciendo, y que la pos del nodo n sea válida
    assert(tama() < tamaMax());
    assert(valido(n) && n != 0); // no puede ser en la raíz

    nod hedrcho = libre; // le asignamos la pos libre
    libre = nodos[libre].hijos.elemento(nodos[libre].hijos.primera());

    // Asignamos al vector el nuevo hijo
    nodos[hedrcho] = {e, nodos[n].padre};

    Lista<nodo>& Lhp = nodos[nodos[n].padre].hijos; // Lista de hijos del padre del nodo n

    // Lista de hijos vacía insertarlo en la lista de hijos del padre siguiendo a n.
    Lista<nodo>::posicion p = Lhp.primera();
    while (n != Lhp.elemento(p)){ // cuando sea el deseado, lo guardamos
        p = Lhp.posterior(p);
    }
    Lhp.insertar(herdrcho, Lhp.siguiente(p)); // almacenamos la posicion nueva del nodo,
    // en la lista de hijos del nodo padre 
    ++numNodos;
}

template <typename T>
void Agen<T>::eliminarHijoIzqdo(nodo n){
    assert(valido(n));
    Lista<nodo>& Lh = nodos[n].hijos;
    assert(!Lh.vacia());
    nodo hizqd = Lh.elemento(Lh.primera());
    // Una vez verificado que el nodo n tiene hijos vemos si este es hoja
    assert(nodos[hizqd].hijos.vacia());

    // Una vez sabemos que el hijo izq existe y es hoja lo borramos, asignamos espacio y convertimos si hay derecho en el nuevo izq
    Lh.eliminar(Lh.primera());
    nodos[hizqd].hijos.insertar(libre, nodos[hizqd].hijos.primera());
    libre = hizqd;
    nodos[hizqd].padre = NODO_NULO;
    numNodos--;
}

template <typename T>
void Agen<T>::eliminarHermDrcho(nodo n) {
    // Precondiciones basicas
    assert(valido(n));
    assert(n != 0); 

    // Buscar hermano drcho. de n en la lista de hijos del padre.
    Lista<nodo>& Lhp = nodos[nodos[n].padre].hijos;
    // Se hace una busqueda donde p es la pos del hermano derecho
    Lista<nodo>::posicion p = Lhp.primera();
    while (n != Lhp.elemento(p)) p = Lhp.siguiente(p);
    p = Lhp.siguiente(p);
    assert(p != Lhp.fin()); //nose pa que se usa

    nodo hdrcho = Lhp.elemento(p);
    assert(nodos[hdrcho].hijos.vacia()); // Herm. drcho de n es hoja y lo eliminamos pues!!!
    Lhp.eliminar(p);
    // Añadir hdrcho al inicio de la lista de libres, para usar
    nodos[hdrcho].hijos.insertar(libre, nodos[hdrcho].hijos.primera());
    libre = hdrcho; // se vuelve la pos libre esa
    #ifndef NDEBUG
    // Sólo para comprobar precondiciones durante depuración.
    nodos[hdrcho].padre = NODO_NULO; // Marcar celda libre.
    #endif
    --numNodos;
}

template <typename T>
inline void Agen<T>::eliminarRaiz(){
    assert(numNodos == 1);
    numNodos = 0;
}

template <typename T>
inline bool Agen<T>::vacio() const{ 
    return numNodos == 0; 
}

template <typename T>
inline size_t Agen<T>::tama() const{ 
    return numNodos; 
}

template <typename T>
inline size_t Agen<T>::tamaMax() const{ 
    return maxNodos; 
}

template <typename T>
inline const T& Agen<T>::elemento(nodo n) const {
    assert(valido(n));
    return nodos[n].elto;
}

template <typename T>
inline T& Agen<T>::elemento(nodo n){
    assert(valido(n));
    return nodos[n].elto;
}

template <typename T>
inline typename Agen<T>::nodo Agen<T>::raiz() const{
    return vacio() ? NODO_NULO : 0; 
}

template <typename T>
inline typename Agen<T>::nodo Agen<T>::padre(nodo n) const{
    assert(valido(n));
    return nodos[n].padre;
}

template <typename T>
inline typename Agen<T>::nodo Agen<T>::hijoIzqdo(nodo n) const {
    assert(valido(n));  
    Lista<nodo>& Lh = nodos[n].hijos;
    // devolvemos la pos o nula si no hay nada(osea n es una hoja)
    return !Lh.vacia() ? Lh.elemento(Lh.primera()) : NODO_NULO;
}

template <typename T>
inline typename Agen<T>::nodo Agen<T>::hermDrcho(nodo n) const {
    // precondiciones
    if(n == 0) return NODO_NULO;
    assert(valido(n)); 

        // como buscamos el hermano derecho, debemos preguntarselo al padre 
        Lista<nodo>& Lhp = nodos[nodos[n].padre].hijos;
        Lista<nodo>::posicion p = Lhp.primera();
        while (n != Lhp.elemento(p)){
            p = Lhp.siguiente(p);
        }
        p = Lhp.siguiente(p);

    return p == Lhp.fin() ? NODO_NULO :Lhp.elemento(p);
}

#endif

