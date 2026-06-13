#include <cassert>
#include <cstddef>
#include <cmath>

#ifndef APO_MIN_MAX_H
#define APO_MIN_MAX_H

 // Posiciones relativas, nodo n
// Padre (n-1)/2 Abuelo (n-3)/4
// Hijo Izq (2*n+1) Hijo Der (2*n+2)
// Hijo izq del hijo Izq (4*n+3) + Hijo der (4*n+4)
// Hijo izq del hijo der (4*n+5) + Hijo der (4*n+6)


template <typename T>
class ApoMinMax {
    public:
        ApoMinMax(size_t maxNodos = 0);
        size_t tama() const;
        bool vacio() const;
        const T& min() const;
        const T& max() const;
        void insertar(const T& e);
        void eliminarMin();
        void eliminarMax();
        // demas metodos innecesarios para el apo min-max
    private:
        typedef size_t nodo; // Índice del vector entre 0 y maxNodos-1
        T* nodos;          // Vector de elementos
        size_t maxNodos, numNodos; // Tamaño del vector y número de nodos actuales

        inline nodo padre(nodo i) const { return (i-1)/2;}
        inline nodo abuelo(nodo i) const { return (i-3)/4;}
        inline nodo hIzq(nodo i)  const { return 2*i+1; }
        inline nodo hDer(nodo i)  const { return 2*i+2; }

        // hacemos operaciones especiales para flotar y hundir
        nodo minNietos(nodo i) const;
        nodo maxNietos(nodo i) const;
        nodo MaxHijos(nodo i) const;
        bool NivelEsPar() const;

        void flotarMin(nodo i);
        void flotarMax(nodo i);
        void hundirMin(nodo i);
        void hundirMax(nodo i);
};

template <typename T>
inline ApoMinMax<T>::ApoMinMax<T>(size_t maxNodos) : nodos{new T[maxNodos]}, maxNodos{maxNodos}, numNodos{0} {}

template <typename T>
inline size_t ApoMinMax<T>::tama() const {
    return numNodos;
}

template <typename T>
inline bool ApoMinMax<T>::vacio() const {
    return numNodos == 0;
}

template <typename T>
inline const T& ApoMinMax<T>::min() const { // como solo hay un nodo devolvemos la cima
    assert(!vacio());
    return nodos[0];
}

template <typename T>
inline const T& ApoMinMax<T>::max() const { // vemos si del nivel 1, cual es el mayor, para devolverlo
    assert(!vacio());
    if (numNodos == 1) return nodos[0];
    else if (numNodos == 2) return nodos[1];
    else return (nodos[1] > nodos[2]) ? nodos[1] : nodos[2];
}


// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------

// Implementamos los metodos privados para usar la operacion insertar
// cada nivel (0, 1, 2 ...) Equivale a la altura del arbol construido
// 2^0 nivel 0, 2^0 - 2^1 nivel 1 ... sumatorio de 0 hasta n-1 de 2^i - 2î+1 -1 nivel n

template <typename T>
inline bool ApoMinMax<T>::NivelEsPar() const{ 
    return (log2(numNodos) % 2 == 0); 
}

template <typename T>
inline void ApoMinMax<T>::flotarMin(nodo i){
    T elto = nodos[i];
    while (i > 2 && elto < nodos[abuelo(i)]){ // mientras no sea nivel 0, o 1 y comparamos solo con el abuelo
        nodos[i] = nodos[abuelo(i)];
        i = abuelo(i);
    }
    nodos[i] = elto;
}

template <typename T>
inline void ApoMinMax<T>::flotarMax(nodo i){
    T elto = nodos[i];
    while (i > 2 && elto > nodos[abuelo(i)]){ // mientras no sea nivel 0, o 1 y comparamos solo con el abuelo
        nodos[i] = nodos[abuelo(i)];
        i = abuelo(i);
    }
    nodos[i] = elto;
}

template <typename T>
inline void ApoMinMax<T>::insertar(const T& elto){
    assert(numNodos < maxNodos);
    nodos[numNodos++] = elto;

    // si no es el unico elto, vemos si es el nuevo minimo, máximo o nivel max-min
    if (numNodos - 1 > 0){
        nodo actual = numNodos - 1;
        nodo p = padre(actual); // localizamos el nodo padre del nuevo introducido

        // contemplamos los dos casos si el nivel es par
        if (NivelEsPar()){ 
            if (nodos[actual] < nodos[p]){ // nos quedamos en niveles MIN
                flotarMin(actual);
            } else { // elegimos niveles MAX
                // hacemos el primer intercambio y sera flotar Max
                nodos[actual] = nodos[p];
                nodos[p] = elto;
                flotarMax(p); // ahora el actual se encuentra en el padre
            }
        // Miramos ahora en un nivel impar
        } else {
            if (nodos[actual] < nodos[p]){
                nodos[actual] = nodos[p];
                nodos[p] = elto;
                flotarMin(p);
            } else {
                flotarMax(actual);
            }
        }
    } 
}

// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------
// Ahora hacemos las operaciones suprimir, primero el minimo

template <typename T>
inline nodo ApoMinMax<T>::minNietos(nodo i) const{
    nodo izq = hIzq(i);
    nodo drcho = hDer(i);

    nodo minNietosIzq = numNodos, minNietosDer = numNodos;
    T nIzq1 = std::numeric_limits<T>::max();
    T nDrcho1 = std::numeric_limits<T>::max();
    T nIzq2 = std::numeric_limits<T>::max();
    T nDrcho2 = std::numeric_limits<T>::max();
    
    T minI = std::numeric_limits<T>::max();
    T minD = std::numeric_limits<T>::max();

    // si n (hijo de p) tiene hijos vemos si existen nietos
    if (izq < numNodos){ 
        if (hIzq(izq) < numNodos) nIzq1 = nodos[hIzq(izq)];
        if (hDer(izq) < numNodos) nDrcho1 = nodos[hDer(izq)];
        // comparamos cual es el mejor
        if (nIzq1 <= nDrcho1){
            minI = nIzq1;
            minNietosIzq = hIzq(izq);
        } else if (nDrcho1 <= nIzq1){
            minI = nDrcho1;
            minNietosIzq = hDer(izq);
        }

    }
    
    if (der < numNodos){
        if (hIzq(drcho) < numNodos) nIzq2 = nodos[hIzq(drcho)];
        if (hDer(drcho) < numNodos) nIzq2 = nodos[hDer(drcho)];
        // comparamos cual es el mejor
        if (nIzq2 <= nDrcho2){
            minD = nIzq2;
            minNietosDer = hIzq(der);
        } else if (nDrcho2 <= nIzq2){
            minD = nDrcho2;
            minNietosDer = hDer(der);
        }
    }

    if (minI < minD){
        return minNietosIzq;
    } else if (minD < minI){
        return minNietosDer;
    } else {
        return i; // el mejor es el actual
    }
}

template <typename T>
inline void ApoMinMax<T>::hundirMin(nodo i){
    assert(i < numNodos);
    bool fin = false;
    T elto = nodos[i];
    nodo nMin, hMin = numNodos;

    // aparte de los hijos, existan nietos
    while (i + 2 < numNodos && !fin){
        nMin = minNietos(i);
        // si no encontramos menor (porque no hay nietos) o el menor de los nietos es mayor que el actual, terminamos
        if (nMin != i && elto > nodos[nMin]){
            nodos[i] = nodos[nMin];
            i = nMin;
        } else {
            fin = true;
        }
    }

    // verificamos si el nodo que hemos bajado es mayor del de algun par de sus hijos, en ese caso lo intercambiamos (caso que el ultimo nivel sea Max)
    if (hIzq(i) < numNodos && hDer(i) < numNodos){
       if (hDer(i) < numNodos && nodos[hDer(i)] < nodos[hIzq(i)]){
            hMin = hDer(i);
       } else {
            hMin = hIzq(i);
       }
    }

    // actualizamos el nodo i con el menor de sus hijos, si es menor que el que hemos bajado (existe nivel par debajo del actual)
    if (hMin < numNodos && nodos[hMin] < elto){
        nodos[i] = nodos[hMin];
        i = hMin;
    } 
    nodos[i] = elto;
}

template <typename T>
inline void ApoMinMax<T>::eliminarMin(){
    assert(numNodos > 0);
    if(--numNodos > 0){
        nodos[0] = nodos[numNodos];
        if (numNodos > 1){
            hundirMin(0);
        }
    }
}


// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------
// Creamos el eliminar maximo, que es similar al eliminar minimo pero con las operaciones maximas

template <typename T>
inline nodo ApoMinMax<T>::maxNietos(nodo i) const{
    nodo izq = hIzq(i);
    nodo drcho = hDer(i);

    // como maximo pueden haber 4 nietos, inicializamos a numNodos para que no se tengan en cuenta
    nodo maxNietosIzq = numNodos, maxNietosDer = numNodos;

    // Guardamos los mejores nietos
    T nIzq1 = std::numeric_limits<T>::min();
    T nDrcho1 = std::numeric_limits<T>::min();
    T nIzq2 = std::numeric_limits<T>::min();
    T nDrcho2 = std::numeric_limits<T>::min();

    T maxI = std::numeric_limits<T>::min();
    T maxD = std::numeric_limits<T>::min();

    // Vemos si el nodo i tiene hijos, y despues sus nietos (escogiendo sus mejores hijos)
    if (izq < numNodos){
        if (hIzq(izq) < numNodos) nIzq1 = nodos[hIzq(izq)];
        if (hDer(izq) < numNodos) nDrcho1 = nodos[hDer(izq)];

        // vemos cual es el mejor
        if (nIzq1 < nDrcho1){
            maxNietosIzq = hIzq(izq);
            maxI = nIzq1;
        } else if (nDrcho1 < nIzq1){
            maxNietosIzq = hDer(izq);
            maxI = nDrcho1;
        }
    } 

    if (drcho < numNodos){
        if (hIzq(drcho) < numNodos) nIzq1 = nodos[hIzq(drcho)];
        if (hDer(drcho) < numNodos) nDrcho1 = nodos[hDer(drcho)];

        // vemos cual es el mejor
        if (nIzq2 < nDrcho2){
            maxNietosDer = hIzq(drcho);
            maxD = nIzq2;
        } else if (nDrcho2 < nIzq2){
            maxNietosDer = hDer(drcho);
            maxD = nDrcho2;
        }
    }

    if (maxI > maxD){
        return maxNietosIzq;
    } else if (maxI < maxD){
        return maxNietosDer;
    } else { // si no hay candidatos el mejor es la i
        return i;
    }
}

// Como puede darse que en un nivel impar tras haber bajado un nodo, si hay nivel inferior par, puede que algún hijo sea mayor al actual
template <typename T>
inline nodo ApoMinMax<T>::MaxHijos(nodo i){
    assert(i < numNodos); // aseguramos que se cumpla lo de nivel impar-par
    
    // guardamos el maximo y vemos si se cumple
    nodo mayor = numNodos;
    if (hIzq(i) < numNodos && nodos[hIzq(i)] < nodos[hDer(i)]){
        mayor = hIzq(i);
    } else if (hDer(i) < numNodos && nodos[hDer(i)] < nodos[hIzq(i)]){
        mayor = hDer(i);
    }
} 

// funcion auxiliar hundir max
template <typename T>
inline void ApoMinMax<T>::hundirMax(nodo i){
    assert(i < numNodos);

    bool fin = false;
    T elto = nodos[i];
    nodo nMax;

    // flotamos con las condiciones que no quedan nietos por consultar
    while (i + 2 < numNodos && !fin){
        nMax = maxNietos(i);

        if (nMax != i && elto < nodos[nMax]){
            nodos[i] = nodos[nMax];
            // verificamos que el nivel bajado no sea inferior a su superior (nivel max)
            if (elto < nodos[padre(nMax)]){
                nodos[nMax] = nodos[padre(nMax)];
                nMax = padre(nMax);
            }
            i = nMax;
        } else {
            fin = true;
        }
    }

    // aseguramos que el nodo que estamos (nivel max), sea mayor que los del nivel min
    nodo hMax = maxHijos(i);

    // si vemos que el nivel inferior tiene valores superiores, los cambiamos
    if (hMax < numNodos && elto < nodos[hMax]){
        nodos[hMax] = nodos[i];
        i = hMax;
    } 
    // una vez encontrado el valor de donde se encuentra el nodo i, lo guardamos
    nodos[i] = elto;
}

inline void ApoMinMax<T>::eliminarMax(){
    assert(numNodos > 0);

    if (numNodos == 1) --numNodos;
    else { // para cualquier otro caso
        nodo posMax = maxHijos(0);
        --numNodos;
        // vemos si solo habia dos nodos, si no hay solo dos nodos hacemos hundir
        if (posMax < numNodos){
            nodos[posMax] = nodos[numNodos];
            hundirMax(posMax);
        }
    }
}

#endif // APO_MIN_MAX_H