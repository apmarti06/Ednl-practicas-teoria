/*3. El TAD árbol binario puede albergar expresiones matemáticas mediante un árbol de
expresión. Dentro del árbol binario los nodos hojas contendrán los operandos, y el resto
de los nodos los operadores.
a) Define el tipo de los elementos del árbol para que los nodos puedan almacenar
operadores y operandos.
b) Implementa una función que tome un árbol binario de expresión (aritmética) y
devuelva el resultado de la misma. Por simplificar el problema se puede asumir que el
árbol representa una expresión correcta. Los operadores binarios posibles en la expresión
aritmética serán suma, resta, multiplicación y división.*/

// Definimos la estructura de datos de la siguiente forma --> Apartado A
// Definición del TAD árbol binario para expresiones matemáticas
#include <iostream>
#include <cassert>
#include <cstddef>
#include <utility> // para swap

class Abin {
public:
    // Definición de la estructura elto (elemento)
    struct elto {
        enum Tipo {OPERADOR, OPERANDO} tipo;
        union {
            char operador;
            int operando;
        };
        
        // Constructores para facilitar el uso
        elto() : tipo(OPERANDO), operando(0) {}
        elto(char op) : tipo(OPERADOR), operador(op) {}
        elto(int num) : tipo(OPERANDO), operando(num) {}
    };
    
    typedef struct celda* nodo;
    static const nodo NODO_NULO;
    
    // constructor, constructor de copia, asignador de copia y destructor
    Abin();
    Abin(const Abin& A); 
    Abin& operator=(const Abin& A);
    ~Abin();
    
    // operaciones modificadoras
    void insertarRaiz(const elto& e);
    void insertarHizq(nodo n, const elto& e);
    void insertarHder(nodo n, const elto& e);
    void eliminarHizq(nodo n);
    void eliminarHder(nodo n);
    void eliminarRaiz();
    
    // operaciones consultoras
    bool vacio() const;
    size_t tama() const;
    const elto& elemento(nodo n) const;
    elto& elemento(nodo n);
    nodo raiz() const;
    nodo padre(nodo n) const;
    nodo hijoIzq(nodo n) const;
    nodo hijoDer(nodo n) const;
    
private:
    struct celda {
        elto e;
        nodo padre, hizq, hder;
    };
    
    nodo r; // raíz
    size_t num_nodos;
};

// Inicialización del nodo nulo
const Abin::nodo Abin::NODO_NULO = nullptr;

// Constructor
Abin::Abin() : r(NODO_NULO), num_nodos(0) {}

// IMPLEMENTACIÓN DEL APARTADO B - Evaluación de la expresión

int resultado(Abin A){
    assert(!A.vacio());
    return resultadoRec(A, A.raiz());
}

int resultadoRec(const Abin& A, typename Abin::nodo n) {
    typedef typename Abin::nodo nodo;
    const nodo NODO_NULO = Abin::NODO_NULO;
    assert(n != Abin::NODO_NULO);
    
    // Si es hoja (no tiene hijos), es un operando
    if (A.hijoIzq(n) == NODO_NULO && A.hijoDer(n) == NODO_NULO) {
        return A.elemento(n).operando;
    }
    
    // Aplicar el operador
    switch(A.elemento(n).operador) {
        case '+': 
            return resultadoRec(A, A.hijoIzq(n)) + resultadoRec(A, A.hijoDer(n));
        case '-': 
            return resultadoRec(A, A.hijoIzq(n)) - resultadoRec(A, A.hijoDer(n));
        case '*': 
            return resultadoRec(A, A.hijoIzq(n)) * resultadoRec(A, A.hijoDer(n));
        case '/': 
            assert(resultadoRec(A, A.hijoDer(n)) != 0); // Prevenir división por cero
            return resultadoRec(A, A.hijoIzq(n)) / resultadoRec(A, A.hijoDer(n));
        default:
            return 0;
    }
}

// FUNCIONES EXTERNAS CORREGIDAS
std::ostream& operator<<(std::ostream& os, const Abin::elto& e) {
    if (e.tipo == Abin::elto::OPERADOR) {
        os << e.operador;
    } else { // OPERANDO
        os << e.operando;
    }
    return os;
}

std::istream& operator>>(std::istream& is, Abin::elto& e) {
    char c;
    is >> c;
    
    // Verificar si es un dígito (operando)
    if (c >= '0' && c <= '9') {
        is.putback(c);
        int num;
        is >> num;
        e = Abin::elto(num);  // Usar el constructor
    } else if (c == '+' || c == '-' || c == '*' || c == '/') {
        e = Abin::elto(c);  // Usar el constructor
    } else {
        // Si no es ni operador ni dígito, lanzar error o asignar valor por defecto
        std::cerr << "Carácter no válido: " << c << std::endl;
        e = Abin::elto(0);  // Valor por defecto
    }
    return is;
}

// Función para comparar elementos (útil para pruebas)
bool operator!=(const Abin::elto& a, const Abin::elto& b) {
    if (a.tipo != b.tipo) return true;
    
    if (a.tipo == Abin::elto::OPERADOR) {
        return a.operador != b.operador;
    } else { // OPERANDO
        return a.operando != b.operando;
    }
}
