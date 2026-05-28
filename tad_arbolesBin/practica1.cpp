#include "arbolBinDIM.h"
//#include "arbolBinPS.h"
#include <iostream>
#include <algorithm>

/* 1. Implementa un subprograma que calcule el número de nodos de un árbol binario, sin usar la
operación tama() del TAD Abin.*/

template <typename T>
int contar_nodos(const Abin<T> A, typename Abin<T>::nodo n){
    if (n == Abin<T>::NODO_NULO){
        return 0;
    } else {
        return 1 + contar_nodos(A.hijoIzq(n)) + contar_nodos(A.hijoDer(n));
    }
}

template <typename T>
int num_nodosArbol (Abin<T> A){
    return contar_nodos(A, A.raiz());
}

/* 2. Calcula la altura del árbol*/

template <typename T>
int Altura_A(const Abin<T> A, typename Abin<T>::nodo n){
    int hijoIzq, hijoDrch;
    if (n == Abin<T>::NODO_NULO){
        return -1; // la altura maxima era la anterior
    }

    hijoIzq = Altura_A(A, A.hijoIzq(n));
    hijoDrch = Altura_A(A, A.hijoDrch(n));

    return 1 + (hijoIzq > hijoDrch ? hijoIzq : hijoDrch);
}

template <typename T>
int tama_Altura_Abin(const Abin<T> A){
    return Altura_A(A, A.raiz());
}

/*3. Implementa un subprograma que, dados un árbol binario y un nodo del mismo, determine
la profundidad de este nodo en dicho árbol. (recorremos el árbol alreves, desde el hijo hacia el nodo padre raiz)*/

template <typemame T>
int profundidad(const Abin<T> A, typename Abin<T>::nodo n){ 
    if (n == Abin<T>::NODO_NULO){ // no nos quedan mas nodos por recorrer, pues estamos en el raíz
        return 0;
    } else if (n == A.raiz()){
        return 1;
    } else {
        return 1 + profundidad(A, A.padre(n));
    }
}

/* 4. Añade dos nuevas operaciones al TAD árbol binario, una que calcule la profundidad de un
nodo y otra que calcule la altura de un nodo en un árbol dado. Implementa esta operación para
la representación vectorial (índices del padre, hijo izquierdo e hijo derecho).*/

// hecho en el tad arbol binario vectorial

/* 5. lo mismo en una enlazada*/

/* 6. Implementa un subprograma que determine el nivel de desequilibrio de un árbol binario,
definido como el máximo desequilibrio de todos sus nodos. El desequilibrio de un nodo se
define como la diferencia entre las alturas de los subárboles del mismo. */

template <typename T>
int desiquilibrio (const Abin<T> A){
    return desiquilibrioRec(A, A.raiz());
}

template <typename T>
int desiquilibrioRec(const Abin<T> A, typename Abin<T>::nodo n){
    if (n == Abin<T>::NODO_NULO){
        return 0;
    } else {
        int izq = Altura_A(A, A.hijoIzq(n));
        int der = Altura_A(A, A.hijoDer(n));

        return std::max(desiquilibrioRec(A, A.hijoIzq(n)), desiquilibrioRec(A, A.hijoDer(n)), std::abs(izq - der));
    }
}

/*7. Implementa un subprograma que determine si un árbol binario es o no pseudocompleto.
En este problema entenderemos que un árbol es pseudocompleto, si en el penúltimo nivel
del mismo cada uno de los nodos tiene dos hijos o ninguno.*/

// Definimos la altura como la longitud que hay desde una hoja al nodo raiz

template <typename T>
bool esComprobarPseudocompleto(const Abin<T> A, typename Abin<T>::nodo n, int altura, int profundidad_actual){
Abin<T>::NODO_NULO;
    if (n == NODO_NULO){
        return true;
    }
    else if (altura - 1 == profundidad_actual){ //comparamos la altura actual para ver si es la deseada del nodo
        
        if (A.hijoIzq(n) != NODO_NULO && A.hijoDer(n) != NODO_NULO ||
        A.hijoIzq(n) == NODO_NULO && A.hijoDer(n) == NODO_NULO){
            return true;
        } else {
            return false; 
        }

    }
    else {
        return (esComprobarPseudocompleto(A, A.hijoIzq(n), altura , profundidad_actual + 1) &&
        esComprobarPseudocompleto(A, A.hijoDer(n), altura, profundidad_actual + 1));
    }
}

/* Otra forma de implementarlo (pseudocodigo)

    bool es_pseudocompleto(A Abin){
        entero Altura = Altura_Bin(A); // entregamos el nodo inicial
            si alt <= 0
                devolver true; // caso base
            sino
                devolver pseudocompletoRec(A, A.raiz());
    }

bool es_pseudocompletoRec(A Abin, nodo n)

    AlturaIzq = Altura_Bin(A, A.hijoI(n));
    AlturaDer = Altura_Bin(A, A.hijoD(n));

    si AlturaIzq = 1
        devolver (A.hijoIzq(n) != NODO_NULO && A.hijoDer(n) != NODO_NULO ||
                    A.hijoIzq(n) == NODO_NULO && A.hijoDer(n) == NODO_NULO)) ? true : false;
    sino
        sino_si Izq > Der 
            devolver pseudocompletoRec(A, A.hijoI(n))

        sino 
            si Der > Izq
                devolver pseudocompletoRec(A, A.hijoD(n))
            sino
                devolver pseudocompletoRec(A, A.hijoI(n)) AND pseudocompletoRec(A, A.hijoD(n))

            fin_si
            
        fin_si

    fin_si

    
*/      