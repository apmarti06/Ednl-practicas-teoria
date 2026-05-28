#ifndef PARTICION_LISTA_LONG_HPP
#define PARTICION_LISTA_LONG_HPP

#include <vector>

class Particion{
public:
    Particion(int n) : padre(n), sig(n, -1), ultimo(n), num_eltos(n, 1) {
        for (int i = 0; i < n; ++i) {
            padre[i] = i;
            ultimo[i] = i;
        }
    }

    int encontrar(int x) const {
        return padre[x];
    }

    void unir(int a, int b) {
        int id_a = padre[a];
        int id_b = padre[b];

        if (id_a != id_b) {
            // Decidir cuál es el más pequeño para moverlo
            if (num_eltos[id_a] < num_eltos[id_b]) {
                std::swap(id_a, id_b);
            }

            // Movemos los elementos del conjunto más pequeño (id_b) al grande (id_a)
            int temp = id_b;
            while (temp != -1) {
                padre[temp] = id_a;
                if (sig[temp] == -1) {
                    sig[ultimo[id_a]] = id_b;
                    ultimo[id_a] = temp;
                    break;
                }
                temp = sig[temp];
            }
            num_eltos[id_a] += num_eltos[id_b];
        }
    }

private:
    std::vector<int> padre;
    std::vector<int> sig;
    std::vector<int> ultimo;
    std::vector<int> num_eltos; // Vector de tamaños
};

#endif