#include "ABB_enla.h"
#include <vector>

/*1. Implementa una nueva operación del TAD Abb que tomando un elemento del mismo
elimine al completo el subárbol que cuelga de él. Ejemplo: Para el árbol binario de búsqueda
de la figura se muestra la transformación si la entrada fuera el valor 9.*/

// Operaciones del tad

template <typename T>
void Abb<T>::descolgar_subarbol(const T& e)
{   
    if (!vacio()){
        // Buscamos el deseado por la izq/der
        if (e < r->elto){
            r->izq.descolgar_subarbol(e);
        } else if (e > r->elto){
            r->der.descolgar_subarbol(e);
        }

        // si se ha encontrado elto, eliminamos todo subnodo
        else {
            arbol* subABB = r;
            r = nullptr;
            delete subABB;  // por implementacion del ABB se eliminara los subarboles
        }
    }
}

/*2. Un árbol binario de búsqueda se puede equilibrar realizando el recorrido en inorden
del árbol para obtener el listado ordenado de sus elementos y a continuación, repartir
equitativamente los elementos a izquierda y derecha colocando la mediana en la raíz y
construyendo recursivamente los subárboles izquierdo y derecho de cada nodo.
Implementa este algoritmo para equilibrar un ABB. */

template <typename T>
void recorrido_inorden(const Abb<T>& A, std::vector<T>& v){
    if (!A.vacio()){
        recorrido_inorden(A.izqdo(), v);
        v.push_back(A.elemento());
        recorrido_inorden(A.drcho(), v);
    }
}

// construccion en preorden
template <typename T>
void construir_arbol (Abb<T>& A, const std::vector<T>& v, int inicio, int fin)
{
    if (inicio <= fin){
        // insertamos primero la raíz, y luego el resto de nodos 
        int medio = (inicio + fin) / 2;
        A.insertar(v[medio]);

        // llamamos de forma recursiva a sus subárboles
        construir_arbol(A, v, inicio, medio - 1);
        construir_arbol(A, v, medio + 1, fin);
    } 
}



template <typename T>
Abb<T> equilibrar(const Abb<T>& A)
{
    // 1. Guardamos los elementos en inorden para escoger bien la mediana, donde para mayor rendimiento hemos de usar un vector
    std::vector<T> elementos;
    recorrido_inorden(A, elementos);

    // 2. Una vez listado los elementos obtenemos la mediana, construyendo el ABB
    Abb resultado;
    construir_arbol(resultado, elementos, 0, elementos.size() - 1);
    
    return resultado;
}

// funcion inorden 
// 1. caso insertamos en el nuevo vector, todos los elementos no repetidos  
/*
[3, 4, 6, 8, 9, 10, 11]
[4, 9, 10, 11, 23]

while (*i, *j acaben)
    si i* < j* guardamos en v[l], e incrementamos dichoo punteros i
    sino si i* > *j  guardamos en v[l], e incrementamos dichoo punteros j
    si son iguales guardamos un elemento de ellos, e incrementamos ambos punteros

// Una vez recorrido insertamos equilibrando el árbol

*/
// 2. caso insertamos en el nuevo vector, solo los elementos repetidos

/*3. Dados dos conjuntos representados mediante árboles binarios de búsqueda,
implementa la operación unión de dos conjuntos que devuelva como resultado otro
conjunto que sea la unión de ambos, representado por un ABB equilibrado.*/

template <typename T>
Abb<T> union_abb(const Abb<T>& A, const Abb<T>& B)
{
    Abb union;
    if (!A.vacio() || !B.vacio()){
        modificarABB(union, A, B);
    }

    Abb resultado = equilibrar(union);

    return resultado;
}

// Función auxiliar recursiva para insertar todos los elementos de un árbol, (PREORDEN)
template <typename T>
void insertarArbol(Abb<T>& destino, const Abb<T>& origen) {
    if (!origen.vacio()) {
        destino.insertar(origen.elemento());
        insertarArbol(destino, origen.izqdo());
        insertarArbol(destino, origen.drcho());
    }
}

template <typename T>
void modificarABB(Abb<T>& A, const Abb<T>& B, const Abb<T>& C)
{   
    insertarArbol(A, B);
    insertarArbol(A, C);   
}

/*4. Dados dos conjuntos representados mediante árboles binarios de búsqueda,
implementa la operación intersección de dos conjuntos, que devuelva como resultado
otro conjunto que sea la intersección de ambos. El resultado debe quedar en un árbol
equilibrado. */

template <typename T>
Abb<T> interseccion_abb(const Abb<T>& A, const Abb<T>& B)
{
    Abb resultado;
    if (!A.vacio() || !B.vacio()){
        elementos_repetidos(resultado, A, B);
    }
    equilibrar(resultado);

    return resultado;
}

template <typename T>
void elementos_repetidos(Abb<T>& A, const Abb<T>& B, const Abb<T>& C)
{   
    T elto_actual = B.elemento();

    if (elto_actual == C.buscar(elto_actual)){
        A.insertar(elto_actual);
    }

    // hacemos el resto de llamadas recursivas, recorriendo subárboles izquierdo y derecho
    elementos_repetidos(A, B.izqdo(), C);
    elementos_repetidos(A, B.drcho(), C);
}

/*5. Implementa el operador  para conjuntos definido como A  B = (A  B) - (A  B).
La implementación del operador  debe realizarse utilizando obligatoriamente la
operación , que nos indica si un elemento dado pertenece o no a un conjunto. La
representación del tipo Conjunto debe ser tal que la operación de pertenencia esté en el
caso promedio en O(log n).*/

// No ponemos clase generica <T> pues necesitariamos de una cabecera .hpp
class Conjunto {
    public:
        // Operaciones del conjunto
        Conjunto();
        bool pertenece(const int& e) const; // O(log n) pues se representa con un ABB

        // Operadores del conjunto
        Conjunto operator |(const Conjunto& A, const Conjunto& B);
        Conjunto operator &(const Conjunto& A, const Conjunto& B);
        Conjunto operator♦(const Conjunto& A, const Conjunto& B);

    private:
        Abb<int> arbol; // el conjunto se representa mediante un ABB, donde la operación
};

Conjunto::Conjunto() : arbol() {} 

bool Conjunto::pertenece(const int& e) const {
    return !arbol.buscar(e).vacio(); // si el resultado de buscar no es vacio, entonces pertenece
}

Conjunto Conjunto::operator |(const Conjunto& A, const Conjunto& B) {
    return union_abb(A.arbol, B.arbol);
}

Conjunto Conjunto::operator &(const Conjunto& A, const Conjunto& B) {-
    return interseccion_abb(A.arbol, B.arbol);
}

Conjunto Conjunto::operator♦(const Conjunto& A, const Conjunto& B) { // orden de complejidad O(n log n) 
    // escogemos los elementos que hay en A, y en B, pero no en ambos, es decir, los elementos que no se repiten
    Conjunto res;
    if (!A.arbol.vacio() || !B.arbol.vacio()){
        elementos_no_repetidos(res.arbol, A.arbol, B.arbol);
    }
    equilibrar(res.arbol); // equilibramos el resultado final
    return res;
}

void elementos_no_repetidos(Abb<int>& A, const Abb<int>& B, const Abb<int>& C)
{   
    int elto_actual = B.elemento();

    if (B.pertenece(elto_actual) && !C.pertenece(elto_actual)){
        A.insertar(elto_actual);
    } else if (!B.pertenece(elto_actual) && C.pertenece(elto_actual)){
        A.insertar(elto_actual);
    }

    // hacemos el resto de llamadas recursivas, recorriendo subárboles izquierdo y derecho
    elementos_no_repetidos(A, B.izqdo(), C);
    elementos_no_repetidos(A, B.drcho(), C);
}

// EJERCICIOS EXTRAS 

template <typename T>
T valor_superior(const Abb<T>& A, const T& e, T& candidato, bool& encontrado) {
    if (A.vacio()) { // se termina de buscar, pues en el lado que se busca no hay mas elementos, donde si se ha encontrado se devuelve
        if (!encontrado) {
            throw std::runtime_error("No existe elemento estrictamente superior");
        }
        return candidato;
    }
    
    if (A.elemento() > e) {
        // Actualizar candidato
        candidato = A.elemento();
        encontrado = true;
        // Buscar en izquierda por si hay uno mejor (más pequeño pero sigue siendo > e)
        return valor_superior(A.izqdo(), e, candidato, encontrado);
    } else {
        // A.elemento() <= e, buscar en derecha
        return valor_superior(A.drcho(), e, candidato, encontrado);
    }
}

// LCA conocer el arbol ancestral


// Dado un Abb A, decir cual es el k-esimo elemento del árbol, (podemos usar el abin)

template <typename T>
T kEsimo(const Abb<T>& A, int cont, int k){
    if(A.vacio()) return T(); // si no se ha encontrado el elto, devolvemos un valor nulo T()

    else{
        // Procesamos la parte izquierda
        T izq = kEsimo(A.izqdo(), cont, k);
        if (izq != T()) return izq;

        // Procesamos el nodo actual y comprobamos si es el k-esimo
        cont++;
        if(cont == k) return A.elemento();

        // Procesamos la parte derecha
        return kEsimo(A.drcho(), cont, k),
    }
}

template <typename T>
T LCA(const Abb<T>& A, const T& e1, const T& e2){
    if (A.vacio()) return T(); // si no se ha encontrado el elto, devolvemos un valor nulo T()
    
    if (A.elemento() > e1 && A.elemento() > e2) {
        // Ambos elementos están en el subárbol izquierdo
        return LCA(A.izqdo(), e1, e2);
    } else if (A.elemento() < e1 && A.elemento() < e2) {
        // Ambos elementos están en el subárbol derecho
        return LCA(A.drcho(), e1, e2);
    } else {
        // Uno de los elementos está en el subárbol izquierdo y el otro en el derecho, o uno de ellos es el nodo actual
        return A.elemento();
    }
}
