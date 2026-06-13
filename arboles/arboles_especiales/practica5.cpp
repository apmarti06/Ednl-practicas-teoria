#include "apo.h"
#include "arbolBinPosRel.h"
#include "AgenEnla.h"
#include "apoMIN-MAX.h"
#include "cola.h"
#include <limits>
#include <cmath>

/*1. Dado un árbol binario de enteros donde el valor de cada nodo es menor que el de sus
hijos, implementa un subprograma para eliminar un valor del mismo preservando la
propiedad de orden establecida. Explica razonadamente la elección de la estructura de
datos.

Nota: Se supone que en el árbol no hay elementos repetidos, y que el número de
nodos del mismo no está acotado*/

const int NODO_NULO = Abin<int>::NODO_NULO;

typename Abin<int>::nodo buscarNodo (const Abin<int>& A,typename Abin<int>::nodo n, const int elto)
{
    if (n == NODO_NULO) return NODO_NULO;
    else if (A.elemento(n) == elto) return n;
    else {
        Abin<int>::nodo izq = buscarNodo(A, A.hijoIzqdo(n), elto);
        Abin<int>::nodo der = buscarNodo(A, A.hijoDrcho(n), elto);

        return izq != NODO_NULO ? izq : der;
    }
}

typename Abin<int>::nodo hundirNodo (Abin<int>& A, typename Abin<int>::nodo n)
{   
    // si no hay nada alli esque no existe el nodo, y si es hoja (dos hijos nodo nulo, quiere decir que hemos llegado al nivel deseado)
    if (n == NODO_NULO) return NODO_NULO;
    else if(A.hijoIzqdo(n) == NODO_NULO && A.hijoDrcho(n) == NODO_NULO) return A.padre(n);
    else {
        int izq = std::numeric_limits<int>::max(), 
        der = std::numeric_limits<int>::max(); // como es un apo Max, cogemos dos valores absolutos, asegurandonos de hundirlo, para elegir cual es el 

        // elegimos el menor hijo, para que se convierta en el nuevo padre
        if (A.hijoDrcho(n) != NODO_NULO){
            der = A.elemento(A.hijoDrcho(n));
        } 

        if (A.hijoIzqdo(n) != NODO_NULO){
            izq = A.elemento(A.hijoIzqdo(n));
        }

        // evaluamos cual intercambiar en funcion de cual es el menor, para mantener la propiedad de orden del apo
        if (izq < der){
           A.elemento(A.hijoIzqdo(n)) = A.elemento(n);
           A.elemento(n) = izq;
           return hundirNodo(A, A.hijoIzqdo(n));
        } else {
            A.elemento(A.hijoDrcho(n)) = A.elemento(n);
            A.elemento(n) = der;
            return hundirNodo(A, A.hijoDrcho(n));
        }
    }
}

void eliminar(Abin<int>& A, typename Abin<T>::nodo n, const int elto)
{   
    // caso más basico
    if (A.tama() == 1 && A.elemento(A.raiz()) == elto){
        A.eliminarRaiz();
    }

    // sino reordenamos todo el árbol, hacemos la reordenacion para cualquier tipo de nodo
    typename Abin<T>::nodo n = buscarNodo(A, A.raiz(), elto);
    if(n != NODO_NULO){
        // actualizamos enlaces
        typename Abin<T>::nodo padre = hundirNodo(A, n);
        // eliminamos el nodo que hayamos hundido, si es hoja (hundir nodo nos asegura colocar si n existe colocarlo en un nodo hoja)
        if (A.hijoIzqdo(padre) != NODO_NULO && A.elemento(A.hijoIzqdo(padre)) == elto){
            A.eliminarHijoIzqdo(padre);
        } else if (A.hijoDrcho(padre) != NODO_NULO && A.elemento(A.hijoDrcho(padre)) == elto){
            A.eliminarHijoDrcho(padre);

        }
    }
}

/*HACEMOS EL APO MIN-MAX dentro del apoMinMax*/


/*IMPLEMENTA UNA FUNCION GENERICA QUE TRANSFORME UN ARBOL BINARIO DE UN TIPO GENERICO T, ELIMINANDO LOS DESCENDIENTES PROPIOS DE TODOS AQUELLOS NODOS
 CUYO CONTENIDO SEA, AL MISMO TIEMPO MAYOR O IGUAL QUE EL DE SUS ASCENDIENTES PROPIOS Y MENOR O IGUAL QUE EL DE SUS DESCENDIENTES PROPIOS*/

template <typename T>
bool menorIgualQueDescendientes(const Abin<T>& A, typename Abin<T>::nodo n, const T& elto){
    if (n != NODO_NULO){
        if (elto > A.elemento(n)) return false; // vemos si el nodo n, su elto es menor que su descendiente
        else return menorIgualQueDescendientes(A, A.hijoIzqdo(n), elto) 
        && menorIgualQueDescendientes(A, A.hijoDrcho(n), elto);       
    }
    return true;
}

template <typename T>
bool mayorIgualQueAscendentes(const Abin<T>& A, typename Abin<T>::nodo n, const T& elto){
    if (n != NODO_NULO){
        if (elto < A.elemento(n)) return false;
        else return mayorIgualQueAscendentes(A, A.padre(n), elto);
    }
    return true;
}

// si se cumple borramos todos los nodos, desde ese nivel
template <typename T>
void eliminarRec(Abin<T>& A, typename Abin<T>::nodo n){
    if (n != NODO_NULO){
        eliminarRec(A, A.hijoIzqdo(n));
        eliminarRec(A, A.hijoDrcho(n));
        // si tenemos nodos en los ultimos niveles los borramos, pues hay que borrar al completo los arboles
        if (A.hijoIzqdo(n) != NODO_NULO) A.eliminarHijoIzqdo(n);
        if (A.hijoDrcho(n) != NODO_NULO) A.eliminarHijoDrcho(n);
    }
}

template <typename T>
void comprobar_rec(Abin<T>& A, typename Abin<T>::nodo n){
    if (n != NODO_NULO){ // vemos si menores son estrictamente mayor, y que mayor estrictamente menor
        if (menorIgualQueDescendientes(A, A.hijoIzqdo(n), A.elemento(A.hijoIzqdo(n))) && 
        menorIgualQueDescendientes(A, A.hijoDrcho(n), A.elemento(A.hijoDrcho(n))) 
        && mayorIgualQueAscendentes(A, n, A.elemento())){
            // si se cumplen las condiciones eliminamos desde ese nodo
            eliminarRec(A, n);
        } else{ // si no se cumple miramos los siguientes
            comprobar_rec(A, A.hijoIzqdo(n));
            comprobar_rec(A, A.hijoDrcho(n));
        }
    }
}

/*4. Un árbol es estrictamente ternario si todos sus nodos son hojas o tienen tres hijos.
Escribe una función que, dado un árbol de grado arbitrario, nos indique si es o no
estrictamente ternario. 
*/

template <typename T>
bool arbolTernario(const Agen<T>& A){
    // nos vamos a nodos hojas, 
    if (A.vacio()) return true; // si es vacio, es ternario
    else return esTernarioRec(A, A.raiz());
}

template <typename T>
bool esTernarioRec(const Agen<T>& A, typename Agen<T>::nodo n) {
    if (n == Agen<T>::NODO_NULO) return true;

    size_t hijos = 0;
    bool flag = true; 

    typename Agen<T>::nodo hijo = A.hijoIzqdo(n);

    while (hijo != Agen<T>::NODO_NULO && flag) {
        hijos++;
        flag = esTernarioRec(A, hijo); 
        hijo = A.hermDrcho(hijo);
    }

    // El nodo es ternario si:
    // 1. Sus descendientes fueron ternarios (posible == true)
    // 2. ÉL mismo tiene 0 o 3 hijos
    return flag && (hijos == 0 || hijos == 3);
}

/*5. Una forma de representar una figura plana en blanco y negro consiste en utilizar un
árbol cuaternario en el que cada nodo o tiene exactamente cuatro hijos, o bien es una
hoja. Un nodo hoja puede ser blanco o negro y un nodo interno no tiene color.

Una figura dibujada dentro de un cuadrado de lado 2k se representa de la forma
siguiente: Se divide el cuadrado en cuatro cuadrantes y cada uno se representa como un
hijo del nodo raíz. Si un cuadrante está completamente negro corresponde a una hoja
negra; si, por el contrario, el cuadrante está completamente blanco, éste corresponde a
una hoja blanca; y si un cuadrante está parcialmente ocupado por negro y blanco,
entonces corresponde a un nodo interno del árbol y este cuadrante se representa
siguiendo el mismo método subdividiéndolo en otros cuatro cuadrantes. 

Como ejemplo se muestra una figura en blanco y negro y su árbol asociado, tomando los cuadrantes en
el sentido de las agujas del reloj a partir del cuadrante superior izquierdo. 
*/


// m: matriz, x,y: coordenadas inicio, tam: tamaño del cuadrante, color: blanco/negro
void pintar(std::vector<std::vector<int>>& m, int x, int y, int tam, int color) {
    for (int i = 0; i < tam; ++i) {
        for (int j = 0; j < tam; ++j) {
            m.at(x + i).at(y + j) = color; 
        }
    }
}

void dibujarMatriz(const Agen<bool>& A, typename Agen<bool>::nodo n, int x,
    int y, int tam, std::vector<std::vector<int>>& matriz){
    // vemos si por debajo hay mas nodos
    if (n == Agen<bool>::NODO_NULO) return;

    // si es el ultimo nivel ya comenzamos a pintar, (aqui el control de hijos es más sencillo)
    typename Agen<bool>::nodo hijo = A.hijoIzqdo(n);
    if(hijo == Agen<bool>::NODO_NULO){
        int color = (A.elemento(n)) ? 1 : 0;
        // true será negro, false será blanco
        pintar(matriz, x, y, tam, color);
    } else {
        // cada vez que estamos en un nivel tenemos nuevo 4 cuadrantes, por lo que el tam de cada cuadrante sera anterior/2
        int nuevoTam = tam / 2;
        int dx[4] = {0, 0, 1, 1};
        int dy[4] = {0, 1, 1, 0};

        for (int i = 0; i < 4; i++) {
            // debe existir 4 hijos si o si, si existe un hijo
            dibujarMatriz(A, hijo, x + dx[i] * nuevoTam, y + dy[i] * nuevoTam, nuevoTam, matriz);
            hijo = A.hermDrcho(hijo);
        }
    }    
}


// Probar un Abin es apo, niveles inferior llenos, o parcialmente llenos (ultimo nodo debe ser hijo izquierdo del nivel anterior)
template <typename T>
bool abin_lleno(const Abin<T>& A){
    Cola<typename Abin<T>::nodo> C;
    typename Abin<T>::nodo n, izqdo, drcho;
    bool primerNulo = false;

    if (!A.vacio()) C.push(A.raiz());

    while (!C.vacia()){
        n = C.frente(); C.pop();
        // vemos si tiene hijo izquierdo los siguientes niveles
        izqdo = A.hijoIzqdo(n);
        drcho = A.hijoDrcho(n);
        if (primerNulo && (izdo != NODO_NULO || drcho != NODO_NULO)){ // si hemos encontrado tras encontrar al supuesto ultumo otro nodo en nivel x,
            return false; 
        }

        // Metemos en cola los que tengamos a la izq !!! Apo si no esta lleno, ultima pos ocupada sera un hijo izq
        if (izqdo != NODO_NULO) C.push(izqdo);
        else primerNulo = true;

        // Metemos en cola los que tengamos a la der
        if (drcho != NODO_NULO) C.push(drcho);
        else primerNulo = true;
    }
    return true;
}

template <typename T>
bool eltos_ordenados(const Abin<T>& A, typename Abin<T>::nodo n, const T& elto){
    if (n != NODO_NULO){
        typename Abin<T>::nodo izq = A.hijoIzq(n);
        typename Abin<T>::nodo izq = A.hijoIzq(n);
        if (izq != NODO_NULO && elto > A.elemento(izq) || der != NODO_NULO && elto > A.elemento(der)){
            return false;
        } else {
            return eltos_ordenados(A, izq, A.elemento(izq)) && eltos_ordenados(A, der, A.elemento(der));
        }
    }
    return true;
}

template <typename T>
bool abinEsApo(const Abin<T>& A){
    return abin_lleno(A) && eltos_ordenados(A, A.raiz(), A.elemento(A.raiz()));
}

