#include "AgenVect.h"
#include "colaS.h"  
#include "listaEnlazada.h"

/*1. Implementa un subprograma que dado un árbol general nos calcule su grado. */

template <typename T>
int llamada_iterativa(const Agen<T>& A){
    if (A.raiz() == Agen<T>::NODO_NULO) return 0;
    return gradoMaxAgenIterativa(A, A.raiz()); 
}

template <typename T>
int gradoMaxAgen(const Agen<T>& A){
    if (A.raiz() == Agen<T>::NODO_NULO) return 0;
    return gradoMaxAgenRec(A, A.raiz()); 
}

template <typename T>
// Implementación no recursiva, recorriendo por niveles
int gradoMaxAgenIterativa(const Agen<T>& A, typename Agen<T>::nodo n){
    cola<typename Agen<T>::nodo> C; 
    int maximo = 0;
    // posible caso distinto if (A.tama() == 1) return 0;
    if (n != Agen<T>::NODO_NULO){
        do {
            int cuenta_actual = 0;

            if(!C.vacia()) {
                n = C.frente();
                C.pop();
            }
    
            typename Agen<T>::nodo hijo = A.hijoIzqdo(n); // cogemos desdenciente del subárboles izq
            while (hijo != Agen<T>::NODO_NULO){
                cuenta_actual++;
                C.push(hijo);
                hijo = A.hermDrcho(hijo);
            }
            // verificamos si es el de mayor grado
            if (cuenta_actual > maximo){
                maximo = cuenta_actual;
            }

        } while (!C.vacia())
    }
    return maximo;
}

// Recursiva

template <typename T>
int gradoMaxAgenRec(const Agen<T>& A, typename Abin<T>::nodo n){
    if (n != Abin<T>::NODO_NULO){
        int maxCont = 0;
        int numHijos = 0;
        typename Agen<T>::nodo hijo = A.hijoIzqdo(n); // cogemos los desdenciente del subárbol

        while (hijo != Abin<T>::NODO_NULO){
            numHijos++;
            int gradoH = gradoMaxAgenRec(A, A.hijoIzqdo(n));
            if (gradoH > maxCont) maxCont = gradoH;
            hijo = A.hermDrcho(hijo);
        }
        
        // si es nodo_nulo, osea no hay más hermanos devolvemos si la cuenta de nodos actual, es mayor que el max Historico
        return (numHijos > maxCont) ? numHijos : maxCont;

    } else {
        return 0;
    }
}


/* 2. Implementa un subprograma que dados un árbol y un nodo dentro de dicho árbol determine
la profundidad de éste nodo en el árbol. */

template <typename T>
int profundidadAgen(const Agen<T>& A, typename Agen<T>::nodo n){
    if (!A.vacio()){
        return profundidadAgenRec(A, n);
    }
}

template <typename T>
int profundidadAgenRec(const Agen<T>& A, typename Agen<T>::nodo n){
    if (n == Agen<T>::NODO_NULO){
        return -1;
    } else {
        return 1 + profundidadAgenRec(A, A.padre(n));
    }
}

/*3. Se define el desequilibrio de un árbol general como la máxima diferencia entre las alturas
de los subárboles más bajo y más alto de cada nivel. Implementa un subprograma que calcule
el grado de desequilibrio de un árbol general. */

// IMPORTANTE, comparamos las alturas de subárboles de distintos padres, osea nivel x, hijos y no son hermanos, sino son hijos
// de los padres del nivel x que son distintos

template <typename T>
int gradoDesequilibrioAgen(const Agen<T>& A)
{
    if (!A.vacio())
    {
        return gradoDesequilibrioAgenRec(A, A.raiz());
    }
}

template <typename T>
int alturaNodo(const Agen<T>& A, typename Agen<T>::nodo n)
{
    if (n == Agen<T>::NODO_NULO) return 0;
    int max = -1;

    typename Agen<T>::nodo hijo = A.hijoIzqdo(n);
    // para escoger la altura del nodo máximo escogemos la del hermano que tenga mayor altura, así recorreremos el nodo
    while (hijo != Agen<T>::NODO_NULO)
    {
        int alt_hijo = alturaNodo(A, hijo);
        if (alt_hijo > max)
        {
            max = alt_hijo;
        }
        hijo = A.hermDrcho(hijo);
    }
    // siempre que comparemos entre todos los hermanos devolvemos la altura conseguida por el nodo calculado para volver a recalcular

    return max + 1;
}

template <typename T>
int gradoDesequilibrioAgenRec(const Agen<T>& A, typename Agen<T>::nodo n)
{   
    if (n != Agen<T>::NODO_NULO)
    {   
        // en esta iteración si entra el nodo deberia de ser el raíz
        cola<typename Agen<T>::nodo> nivel;
        int maxDesequilibrio = 0;
        nivel.push(n);

        while (!nivel.vacia()){
        // recorremos todos los niveles uno a uno 
            size_t tamNivel = nivel.tama();
            int minAltura = INT_MAX;
            int maxAltura = INT_MIN;

            // procesamos el nivel actual
            for (size_t i = 0; i < tamNivel; i++){
                // Sacamos el nodo actual y actualizamo
                n = nivel.frente(); nivel.pop();
                // Calculamos alturas
                int alt_sub = alturaNodo(A, n);

                if (alt_sub < minAltura) minAltura = alt_sub;
                if (alt_sub > maxAltura) maxAltura = alt_sub;

                // copiamos el resto de subhijos, para pasar al siguiente nivel del nodo n
                typename Agen<T>::nodo hijo = A.hijoIzqdo(n);
                while (hijo != Agen<T>::NODO_NULO){
                    nivel.push(hijo);
                    hijo = A.hermDrcho(hijo);
                }
                
            }
            // Calculamos el desequilibrio, ya que tu calculas el desequilibrio de cada nodo y comparamos
            if (tamNivel >= 2){
                int desequilibrio = maxAltura - minAltura;
                maxDesequilibrio = std::max(desequilibrio, maxDesequilibrio);
            }
        }
    }
    
    return maxDesequilibrio;
}

/*4. Dado un árbol general de enteros A y un entero x, implementa un subprograma que realice
la poda de A a partir de x. Se asume que no hay elementos repetidos en A. */

typename Agen<T>::nodo buscar_nodo(const Agen<T>& A ,typename Agen<T>::nodo n ,int x ){
    // el nodo no es del árbol, o no existe hijo izquierdo de n
    if (n != Agen<T>::NODO_NULO){
        // hacemos una solo busqueda
        if (x == A.elemento(n)){
            return n;
        } else {
            typename Agen<T>::nodo hijo = A.hijoIzqdo(n);
            while (hijo != Agen<T>::NODO_NULO){
                typename Agen<T>::nodo res = buscar_nodo( A, A.hijoIzqdo(n), x);  
                // si no es nodo nulo es porque se ha encontrado el valor deseado en el nodo res
                if (res != Agen<T>::NODO_NULO){
                    return res;
                }
                hijo = A.hermDrcho(hijo);
            }
        }
    }
    // si no se ha encontrado nada 
    return Agen<T>::NODO_NULO;
}

// nose otra forma que no altere la estructura de control de llamadas
template <typename T>
void eliminarSubarboles(Agen<T>& A, typename Agen<T>::nodo n) 
{

    if (n == Agen<T>::NODO_NULO) { // No hay nodos por debajo
        return;
    }
    
    // mientras haya hijo izquierdos y derechos seguimos eliminando hijos
    typename Agen<T>::nodo hijo = A.hijoIzqdo(n);
    while (hijo != Agen<T>::NODO_NULO) {
        // Guardar el siguiente hermano ANTES de eliminar
        typename Agen<T>::nodo siguiente = A.hermanoDrcho(hijo);

        eliminarSubarboles(A, hijo);  // ← RECURSIÓN: limpia los nietos
        A.eliminarHijoIzqdo(n, hijo);  // una vez eliminado la parte de abajo eliminamos su el izquierdo del padre, para que su hermano derecho sea el nuevo izq
        hijo = siguiente;
    }
}

template <typename T>
Agen<T> ArbolPodado(const Agen<T>& A, int x) {
    Agen<T> res = A;

    // Verificamos si el árbol es válido
    if (!res.vacio()){
        typename Agen<T>::nodo n = buscar_nodo(res, res.raiz(), x);

        // Si existe el nodo a podar, verificando los 2 casos distintos
        if (n != Agen<T>::NODO_NULO){

            // Si el nodo a podar es la raíz, se elimina todo el árbol
            if (n == res.raiz()){
                eliminarsubarboles(res, n);
                res.eliminarRaiz();

            }  
            // sino podamos desde el nodo deseado, asegurandonos que no eliminemos otro nodo
            else {
                typename Agen<T>::nodo p = res.padre(n);

                // Si n es el hijo izquierdo de su padre, podamos solo esos, sino buscamos el hermano deseado
                if (n == res.hijoIzqdo(p)){
                    eliminarsubarboles(res, n); // cuando terminamos sus descendientes borramos el suyo
                    res.eliminarHijoIzqdo(p);

                } else {
                    typename Agen<T>::nodo hijo = res.hijoIzqdo(p);
                    // como queremos eliminar primero sus hijos, buscamos donde esta el hermano, para borrar hijos despues borramos este
                    while (res.hermDrcho(hijo) != n){ // estamos solo uno del derecho
                        hijo = res.hermDrcho(hijo)
                    }
                    // una vez encontramos el nodo deseado borramos todos sus descendientes y el
                    eliminarsubarboles(res, n);
                    res.eliminarHermDrcho(hijo);
                }
            }
        }
    }
    return res;
}

// Acordarse de como contar los nodos para DLH y para jose fidel podar árboles

