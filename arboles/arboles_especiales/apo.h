#ifndef APO_H
#define APO_H
#include <cassert>
#include <cstddef>   // size_t
#include <utility>   // swap

template <typename T> class Apo {
public:
   explicit Apo(size_t maxNodos = 0);
   void insertar(const T& e);
   void suprimir();
   const T& cima() const;
   bool vacio() const;
   size_t tama() const;
   size_t tamaMax() const;
   Apo(const Apo& A);               // Ctor. de copia
   Apo& operator =(const Apo& A);   // Asignación de Apo
   ~Apo();                          // Destructor
private:
   typedef size_t nodo; // Índice del vector entre 0 y maxNodos-1

   T* nodos;          // Vector de elementos
   size_t maxNodos,   // Tamaño del vector
          numNodos;   // Tamaño del Apo

   nodo padre(nodo i) const { return (i-1)/2; }
   nodo hIzq(nodo i)  const { return 2*i+1; }
   nodo hDer(nodo i)  const { return 2*i+2; }
   void flotar(nodo i);
   void hundir(nodo i);
};

/*--------------------------------------------------------*/
/* clase Apo genérica: vector de posiciones relativas.    */
/*    Un Apo de n nodos se almacena en celdas             */
/*    consecutivas del vector, desde 0 hasta numNodos-1.  */
/*    La raíz del APO se almacena en la celda 0 del       */
/*    vector. Al hijo izquierdo del nodo i le corresponde */
/*    la celda 2i+1 y al hijo derecho la celda 2i+2.      */
/*                                                        */
/* Implementación de operaciones                          */
/*--------------------------------------------------------*/

template <typename T>
inline Apo<T>::Apo(size_t maxNodos) :
   nodos{new T[maxNodos]},
   maxNodos{maxNodos},
   numNodos{0}
{}

template <typename T>
inline void Apo<T>::insertar(const T& e)
{
   assert(numNodos < maxNodos);   // Apo no lleno.
   nodos[numNodos++] = e;
   if (numNodos > 1)
      flotar(numNodos-1);   // Reordenar.
}

template <typename T>
inline void Apo<T>::suprimir() // eliminamos el inmediato y ordenamos
{
   assert(numNodos > 0);   // Apo no vacío.
   if (--numNodos > 0) {   // Apo no queda vacío.
      nodos[0] = nodos[numNodos];
      if (numNodos > 1)     // Quedan dos o más elementos.
         hundir(0);         // Reordenar.
   }
}

template <typename T>
inline const T& Apo<T>::cima() const
{
   assert(numNodos > 0);
   return nodos[0];
}

template <typename T>
inline bool Apo<T>::vacio() const
{
   return numNodos == 0;
}

template <typename T>
inline size_t Apo<T>::tama() const
{
   return numNodos;
}

template <typename T>
inline size_t Apo<T>::tamaMax() const
{
   return maxNodos;
}

template <typename T>
Apo<T>::Apo(const Apo& A) : Apo{A.maxNodos}
{
   if (!A.vacio()) {
      numNodos = A.numNodos;
      for (nodo n = 0; n < numNodos; ++n)
         nodos[n] = A.nodos[n];
   }
}

template <typename T>
Apo<T>& Apo<T>::operator =(const Apo& A)
{
   Apo B{A};
   std::swap(nodos, B.nodos);
   std::swap(maxNodos, B.maxNodos);
   std::swap(numNodos, B.numNodos);
   return *this;
}

template <typename T>
inline Apo<T>::~Apo()
{
   delete[] nodos;
}

// Métodos privados

template <typename T>
void Apo<T>::flotar(nodo i)
{
   T e = nodos[i];
   while (i > 0 && e < nodos[padre(i)]) {
      nodos[i] = nodos[padre(i)];   // Bajar el padre.
      i = padre(i);
   }
   nodos[i] = e;   // Colocar e.
}

template <typename T>
void Apo<T>::hundir(nodo i)
{
   bool fin = false;
   T e = nodos[i];
   while (hIzq(i) < numNodos && !fin) {   // Hundir e
      nodo hMin;   // Hijo menor del nodo i
      if (hDer(i) < numNodos && nodos[hDer(i)] < nodos[hIzq(i)])
         hMin = hDer(i);
      else
         hMin = hIzq(i);
      if (nodos[hMin] < e) { // Subir el hijo menor
         nodos[i] = nodos[hMin];
         i = hMin;
      }
      else   // e <= nodos[hMin]
         fin = true;
   }
   nodos[i] = e;   // Colocar e.
}

#endif // APO_H
