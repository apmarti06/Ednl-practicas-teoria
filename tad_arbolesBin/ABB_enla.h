#ifndef ABB_H
#define ABB_H 

#include <cassert>
#include <utility>

template <typename T> class Abb {
public:
    Abb();
    const Abb& buscar(const T& e);
    // Operaciones que modifican el ABB
    void insertar(const T& e);
    void eliminar(const T& e);
    void descolgar_subarbol(const T& e);

    // Métodos consultoras
    bool vacio() const;
    const T& elemento() const;
    const Abb& izqdo() const;
    const Abb& drcho() const


    // Operadores del tad definidos no implementados
    Abb operator O(const Abb& A, const Abb& B);
    Abb operator A(const Abb& A, const Abb& B);
    Abb operator ♦(const Abb& A, const Abb& B);

    // Consultora de pertenece el elemento
    bool pertenece(const T& e) const;

    // Constructores + Operador + Destructor
    Abb(const Abb& A);
    Abb& operator =(const Abb& A);
    ~Abb();


private:
    struct arbol {
        T elto;
        Abb izq, der; // subarboles

        arbol(const arbol& a) = default;
        ~arbol() = default;
    };

    arbol* r; // Raíz del árbol
};


template <typename T>
inline Abb<T>::Abb() : r{nullptr} {}

template <typename T>
inline bool Abb<T>::vacio() const {
    return r == nullptr;
}

template <typename T> 
const Abb<T>& Abb<T>::buscar(const T& e){
    if (vacio()){ // aquí no se encontraria lo cual seria elto nulo
        return *this; 
    } else if (e < r->elto){
        retuºrn r->izq.buscar(e)
    } else if (e > r->elto){
        return r->der.buscar(e)
    } else {
        return *this; // se ha encontrado el elto necesario
    }
}

template <typename T> 
void Abb<T>::insertar(const T& e){
    if (vacio){ // si en el subárbol no existe el elemento e, se introduce
        r = new arbol{e};
    } else if (e < r->elto){
        r->izq.insertar(e);
    } else if (e > r->elto){
        r->der.insertar(e);
    }
}

template <typename T>
void Abb<T>::eliminar(const T& e){
    if (!vacio()){
        if (e < r->elto){
            r->izq.eliminar(e);

        } else if (e > r->elto){
            r->der.eliminar(e);

        } else { // se ha encontrado el elto en esta rama
            arbol* a;
            if(!r->izq.r && !r->der.r){ // 1. La raiz es hoja (ambos son nullptr)
                a = nullptr;
            } else if (!r->der.r){  // 2. La raíz solo tiene hijo izq
                a = r->izq.r;
                r->izq.r = nullptr; // descolgamos la raíz del árbol

            } else if (!r->izq.r){ // 3. La raíz solo tiene hijo der
                a = r->der.r;
                r->der.r = nullptr; // descolgamos la raíz del árbol
            } else { // 4. La raíz posee 2 hijos

                // Podemos buscar bien el mínimo/máximo (el minimo estara en el extremo izquierdo del subarbol derecho)
                // Y de forma reciproca, el máximo estara en el extremo derecho del subárbol izquierdo
                Abb* minimo = &r->der;
                while (minimo->r->izq.r != nullptr) minimo = &minimo->r->izq;

                // Extraemos mínimo
                a = minimo->r;
                minimo->r = a->der.r;
                // Transferir al mínimo los subárboles de la raíz.
                a->izq.r = r->izq.r;
                a->der.r = r->der.r; 
                // colgamos los subárboles sobre el nuevo minimo para el lado derecho
                r->izq.r = nullptr;
                r->der.r = nullptr;
            }
            delete r;
            r = a;
            // terminamos de eliminar la raiz
        }
    }
}

template <typename T>
inline const T& Abb<T>::elemento() const{
    assert(!vacio());
    return r->elto;
}

template <typename T>
inline const Abb<T>& Abb<T>::izqdo() const{
    assert(!vacio());
    return r->izq;
}

template <typename T>
inline const Abb<T>& Abb<T>::drcho() const{
    assert(!vacio());
    return r->der;
}

template <typename T>
inline Abb<T>::Abb(const Abb<T>& A) : Abb{}
{
    if (!A.vacio()){ // Entonces *A.r no es un puntero, sino el objeto raíz del árbol A.
        r = new arbol{*A.r}; // crea una raíz nueva copiando el contenido
    }
}

template <typename T>
inline Abb<T>& Abb<T>::operator=(const Abb<T>& B){
    Abb A{B};
    std::swap(r, A.r);
    return *this;
}

template <typename T>
inline Abb<T>::~Abb() {
    delete r;
    r = nullptr;
}

#endif //ABB_H