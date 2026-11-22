#pragma once
template <typename K, typename V>
struct NodoHash {
    K clave;
    V valor;
    NodoHash<K, V>* siguiente;

    NodoHash(K c, V v) {
        clave = c;
        valor = v;
        siguiente = nullptr;
    }
};

template <typename K, typename V>
class TablaHash {
private:
    vector<NodoHash<K, V>*> tabla;
    int capacidad;
    int numElementos;

    int funcionHash(const K& clave) const {
        unsigned long long h = calcularHashPropio(clave);
        return (int)(h % (unsigned long long)capacidad);
    }

public:
    TablaHash(int cap = 15013) {
        capacidad = cap;
        numElementos = 0;
        tabla.resize(capacidad, nullptr);
    }

    ~TablaHash() { limpiar(); }

    void limpiar() {
        for (int i = 0; i < capacidad; i++) {
            NodoHash<K, V>* actual = tabla[i];
            while (actual != nullptr) {
                NodoHash<K, V>* temp = actual;
                actual = actual->siguiente;
                delete temp;
            }
            tabla[i] = nullptr;
        }
        numElementos = 0;
    }

    void insertar(const K& clave, const V& valor) {
        int idx = funcionHash(clave);
        NodoHash<K, V>* actual = tabla[idx];
        while (actual != nullptr) {
            if (actual->clave == clave) {
                actual->valor = valor;
                return;
            }
            actual = actual->siguiente;
        }
        NodoHash<K, V>* nuevo = new NodoHash<K, V>(clave, valor);
        nuevo->siguiente = tabla[idx];
        tabla[idx] = nuevo;
        numElementos++;
    }

    V* buscar(const K& clave) {
        int idx = funcionHash(clave);
        NodoHash<K, V>* actual = tabla[idx];
        while (actual != nullptr) {
            if (actual->clave == clave) return &(actual->valor);
            actual = actual->siguiente;
        }
        return nullptr;
    }

    bool eliminar(const K& clave) {
        int idx = funcionHash(clave);
        NodoHash<K, V>* actual = tabla[idx];
        NodoHash<K, V>* anterior = nullptr;
        while (actual != nullptr) {
            if (actual->clave == clave) {
                if (anterior == nullptr) {
                    tabla[idx] = actual->siguiente;
                }
                else {
                    anterior->siguiente = actual->siguiente;
                }
                delete actual;
                numElementos--;
                return true;
            }
            anterior = actual;
            actual = actual->siguiente;
        }
        return false;
    }

    int getTamano() const { return numElementos; }

    // Calcular factor de carga (alfa = n / m)
    double obtenerFactorCarga() const {
        return capacidad > 0 ? (double)numElementos / capacidad : 0.0;
    }

    // Contar nodos colisionados en las listas enlazadas
    int contarColisiones() const {
        int colisiones = 0;
        for (int i = 0; i < capacidad; i++) {
            NodoHash<K, V>* actual = tabla[i];
            if (actual != nullptr && actual->siguiente != nullptr) {
                NodoHash<K, V>* temp = actual->siguiente;
                while (temp != nullptr) {
                    colisiones++;
                    temp = temp->siguiente;
                }
            }
        }
        return colisiones;
    }

    // Filtrar elementos de toda la tabla hash segun una condicion
    template <typename Predicado>
    vector<V> filtrar(Predicado pred) const {
        vector<V> res;
        for (int i = 0; i < capacidad; i++) {
            NodoHash<K, V>* actual = tabla[i];
            while (actual != nullptr) {
                if (pred(actual->valor)) {
                    res.push_back(actual->valor);
                }
                actual = actual->siguiente;
            }
        }
        return res;
    }
};