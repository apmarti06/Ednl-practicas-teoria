#ifndef PARTICION_VEC_H
#define PARTICION_VEC_H

#include <vector>

class ParticionVec {
public:
    // Constructor: Crea n conjuntos (0 a n-1)
    ParticionVec(int n) : P(n) {
        for (int i = 0; i < n; ++i) P[i] = i;
    }

    // Devuelve el identificador del conjunto de x
    int encontrar(int x) const {
        return P[x];
    }

    // Une los conjuntos de a y b (Complejidad O(n))
    void unir(int a, int b) {
        int id_a = P[a];
        int id_b = P[b];
        if (id_a != id_b) {
            for (int i = 0; i < P.size(); ++i) {
                if (P[i] == id_b) P[i] = id_a;
            }
        }
    }

private:
    std::vector<int> P;
};

#endif