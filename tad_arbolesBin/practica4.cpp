#include "ABB_enla.h"
#include <vector>

/*1. Implementa una nueva operación del TAD Abb que tomando un elemento del mismo
elimine al completo el subárbol que cuelga de él. Ejemplo: Para el árbol binario de búsqueda
de la figura se muestra la transformación si la entrada fuera el valor 9.*/

// Operaciones del tad

template <typename T>
void Abb<T>::descolgar_subarbol(const T& e)
{   
    if (vacio())
    {
        return; // no existe el elto buscado
    }

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

/*2. Un árbol binario de búsqueda se puede equilibrar realizando el recorrido en inorden
del árbol para obtener el listado ordenado de sus elementos y a continuación, repartir
equitativamente los elementos a izquierda y derecha colocando la mediana en la raíz y
construyendo recursivamente los subárboles izquierdo y derecho de cada nodo.
Implementa este algoritmo para equilibrar un ABB. */

template <typename T>
void recorrido_inorden(const Abb<T>& A, std::vector<T>& v) const {
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
    construir_arbol(resultado, elementos, elementos.begin(), elementos.end());
    
    // De esta manera 
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

// Función auxiliar recursiva para insertar todos los elementos de un árbol
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

/*  template <typename T>
Abb<T> operacion_especial(const Abb<T>& A, const Abb<T>& B){
    Abb<T> resultado;

    // 1. llamamos a los resultados internos de union e interseccion 
    Abb<T> union = union_abb(A, B);
    Abb<T> interseccion = interseccion_abb(A, B);

    // 2. Introducimos solo los elementos que no se repitan, de la union e interseccion
    operador_rec(resultado, union, interseccion);

    return resultado;
}

template <typename T>
void operador_rec(Abb<T>& A, const Abb<T>& B, const Abb<T>& C){
    // insertamos solo los distintos
}

*/ // ¡¡¡¡¡Super ineficiente!!!! Orden n+m

template <typename T>
Abb<T> operator♦(const Abb<T>& A, const Abb<T>& B) {
    Abb<T> resultado;

    // Recorremos todo conjunto A y B, 1 y 2
    xor_recursivo(resultado, A, B);
    xor_recursivo(resultado, B, A);

    //2. retornamos el resultado
    return resultado;
}
 
template <typename T>
void xor_recursivo(Abb<T>& res, const Abb<T>& origen, const Abb<T>& otro){
    if (!origen.vacio()){
        T elto_actual = origen.elemento();

        // comparamos la pertenencia, si no pertenece al arbol b, quiere decir que solo existe en A, y es la operacion (union - interseccion)
        if (elto_actual != otro.buscar(e)){
            res.insertar(elto_actual);
        }

        xor_recursivo(res, origen.izqdo(), otro);
        xor_recursivo(res, origen.drcho(), otro);
    }
}

/*¡IMPORTANTE EL EJERCICIO TE LO PLANTEAS COMO UN TAD conjunto (3, 4, 5)!

Cositas: 

for(auto e: listaB){ // para cuando no sepamos que tipo de estructura de datos, y auto una vez declarada el tipo, como cuando usamos nodos, o tipos internos de tads
    //condiciones
}

*/

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



