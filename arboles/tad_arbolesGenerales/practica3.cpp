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
int gradoMaxAgenRec(const Agen<T>& A, typename Agen<T>::nodo n){
    if (n != Agen<T>::NODO_NULO){
        int maxCont = 0;
        int numHijos = 0;
        typename Agen<T>::nodo hijo = A.hijoIzqdo(n); // cogemos los desdenciente del subárbol
        while (hijo != Agen<T>::NODO_NULO){
            numHijos++;
            int gradoH = gradoMaxAgenRec(A, hijo);
            // vemos si su hijo recorrido tiene un grado mayor que el maximo histórico, si es así lo actualizamos
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


// 1. Paso creamos una funcion auxliar para pasarle al desequilibrio la raiz del nodo para procesar el arbol
// 2. Creamos una funcion auxiliar que calcule la altura de un nodo, para luego comparar las alturas de los nodos del mismo nivel
// 3. Creamos la función recursiva que calcule el desequilibrio, donde usando un recorrido por niveles (anchura),
// veamos todos los nodos del mismo nivel y obtengamos el desequilibrio (maximo - minimo) de ese nivel, y comparandolo con el desequilibrio máximo histórico, para devolver el desequilibrio máximo del árbol

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
    // para escoger la altura del nodo máximo escogemos la del hermano que tenga mayor altura, así recorreremos todos los hijos del nodo n
    while (hijo != Agen<T>::NODO_NULO)
    {
        int alt_hijo = alturaNodo(A, hijo);
        if (alt_hijo > max) max = alt_hijo;
        hijo = A.hermDrcho(hijo);
    }
    // siempre que comparemos entre todos los hermanos devolvemos la altura conseguida por el nodo calculado para volver a recalcular
    return max + 1;
}

template <typename T>
int gradoDesequilibrioAgenRec(const Agen<T>& A, typename Agen<T>::nodo n)
{   
    int maxDesequilibrio = 0;
    if (n != Agen<T>::NODO_NULO) // si hay raiz lo procesamos, sino el desequilibrio es 0
    {   
        // en esta iteración si entra el nodo deberia de ser el raíz
        cola<typename Agen<T>::nodo> nivel;
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

                // Determinamos que del nivel, cual es el min y el max
                if (alt_sub < minAltura) minAltura = alt_sub;
                if (alt_sub > maxAltura) maxAltura = alt_sub;

                // una vez procesado el nodo, añadimos sus hijos al nivel siguiente
                typename Agen<T>::nodo hijo = A.hijoIzqdo(n);
                while (hijo != Agen<T>::NODO_NULO){
                    nivel.push(hijo);
                    hijo = A.hermDrcho(hijo);
                }
                // debemos controlar siempre bien el tamNivel, ya que podriamos incluir algun nodo de algun hijo izquierdo de n
            }
            // Calculamos el desequilibrio, ya que tu calculas el desequilibrio de cada nodo y comparamos
            if (tamNivel >= 2){
                int desequilibrio = maxAltura - minAltura;
                if (desequilibrio > maxDesequilibrio) maxDesequilibrio = desequilibrio;
            }
        }
    }
    return maxDesequilibrio;
}

/*4. Dado un árbol general de enteros A y un entero x, implementa un subprograma que realice
la poda de A a partir de x. Se asume que no hay elementos repetidos en A. */

// buscamos el nodo n, que posee x
typename Agen<int>::nodo buscar_nodo(const Agen<int>& A ,typename Agen<int>::nodo n , int x){
    // el nodo no es del árbol, o no existe hijo izquierdo de n
    if (n != Agen<int>::NODO_NULO){
        // hacemos una solo busqueda
        if (x == A.elemento(n)){
            return n;
        } else {
            typename Agen<int>::nodo hijo = A.hijoIzqdo(n);
            while (hijo != Agen<int>::NODO_NULO){
                typename Agen<int>::nodo res = buscar_nodo( A, A.hijoIzqdo(n), x);  
                // si no es nodo nulo es porque se ha encontrado el valor deseado en el nodo res, y devolvemos directamente
                if (res != Agen<int>::NODO_NULO){
                    return res;
                }
                hijo = A.hermDrcho(hijo);
            }
        }
    }
    // si no se ha encontrado nada 
    return Agen<int>::NODO_NULO;
}

// El nodo que se pasa es el buscado, hacemos la eliminacion de nodos en postorden
void eliminarSubarboles(Agen<int>& A, typename Agen<int>::nodo n) 
{
    if (n == Agen<int>::NODO_NULO) { // No hay nodos por debajo
        return;
    }
    // mientras haya hijo izquierdos y derechos seguimos eliminando hijos
    typename Agen<int>::nodo hijo = A.hijoIzqdo(n);
    while (hijo != Agen<int>::NODO_NULO) {una vez elim
        // eliminamos el subarbol del hijo, y luego el hijo, para luego pasar al siguiente hermano, hasta que no haya más hermanos
        eliminarSubarboles(A, hijo);  
        A.eliminarHijoIzqdo(n, hijo);  
        hijo = A.hermanoDrcho(hijo);
    }
}

template <typename T>
Agen<T> ArbolPodado(const Agen<T>& A, int x) {
    Agen<T> res = A;

    // Verificamos si el árbol es válido
    if (!res.vacio()){
        typename Agen<T>::nodo n = buscar_nodo(res, res.raiz(), x);

        // Si existe el nodo a podar, verificando los 3 casos distintos (nodo sea hijo izq de n, sea nodo raiz, o sea un hermano derecho de un subarbol)
        if (n != Agen<T>::NODO_NULO){

            // Si el nodo a podar es la raíz, se elimina todo el árbol
            if (n == res.raiz()){
                eliminarsubarboles(res, n);
                res.eliminarRaiz();

            }  
            // sino podamos desde el nodo deseado, asegurandonos que no eliminemos otro nodo
            else {
                // buscamos si es hijo izquierdo o hermano derecho, para eliminarlo correctamente
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

//nodos verdes (igual que abin distinta implementacion) 
template <typename T>
size_t contarNodosVerdes(const Agen<T>& A){
    if (A.vacio()) return 0;
    return contarNodosVerdesRec(A, A.raiz());
}

template <typename T>
size_t TresNietos(const Agen<T>& A, typename Agen<T>::nodo n){
    if (n == Agen<T>::NODO_NULO) return 0;
    size_t nietos = 0;
    typename Agen<T>::nodo hijo = A.hijoIzqdo(n);
    while (hijo != Agen<T>::NODO_NULO){
        typename Agen<T>::nodo nieto = A.hijoIzqdo(hijo);
        while (nieto != Agen<T>::NODO_NULO){
            nietos++;
            nieto = A.hermDrcho(nieto);
        }
        hijo = A.hermDrcho(hijo);
    }
    return nietos;
}

template <typename T>
size_t contarNodosVerdesRec(const Agen<T>& A, typename Agen<T>::nodo n){
    // primero probamos n, luego probamos el resto de nodos a recorrer
    if (n == Agen<T>::NODO_NULO) return 0;
    size_t cont = 0;
    if (TresNietos(A, n)){
        cont = 1;
    }

    //recorremos todos los hijos del actual, asi recursivamente
    typename Agen<T>::nodo hijo = A.hijoIzqdo(n);
    while (n != Agen<T>::NODO_NULO){
        cont += contarNodosVerdesRec(A, hijo);
        hijo = A.hermDrcho(n);
    }
    return cont;
}

