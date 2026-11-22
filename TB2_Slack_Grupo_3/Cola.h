#pragma once
#include <vector>

using namespace std;

template <typename T>
struct NodoCola {
    T dato;
    NodoCola<T>* siguiente;

    NodoCola(T d) {
        dato = d;
        siguiente = nullptr;
    }
};

template <typename T>
class Cola {
private:
    NodoCola<T>* frente;
    NodoCola<T>* finalCola;
    int tamano;

public:
    Cola() {
        frente = nullptr;
        finalCola = nullptr;
        tamano = 0;
    }

    ~Cola() { limpiar(); }

    void limpiar() {
        while (!estaVacia()) desencolar();
    }

    void encolar(T valor) {
        NodoCola<T>* nuevo = new NodoCola<T>(valor);
        if (estaVacia()) {
            frente = nuevo;
            finalCola = nuevo;
        }
        else {
            finalCola->siguiente = nuevo;
            finalCola = nuevo;
        }
        tamano++;
    }

    bool desencolar() {
        if (estaVacia()) return false;
        NodoCola<T>* temp = frente;
        frente = frente->siguiente;
        if (frente == nullptr) finalCola = nullptr;
        delete temp;
        tamano--;
        return true;
    }

    int desencolarLote(int k) {
        int procesadas = 0;
        while (!estaVacia() && procesadas < k) {
            desencolar();
            procesadas++;
        }
        return procesadas;
    }

    T obtenerFrente() const { return frente->dato; }
    bool estaVacia() const { return frente == nullptr; }
    int getTamano() const { return tamano; }

    vector<T> listarTodos(int limite = -1) const {
        vector<T> res;
        NodoCola<T>* actual = frente;
        while (actual != nullptr && (limite < 0 || (int)res.size() < limite)) {
            res.push_back(actual->dato);
            actual = actual->siguiente;
        }
        return res;
    }

    template <typename Predicado>
    vector<T> filtrarSi(Predicado pred) const {
        vector<T> res;
        NodoCola<T>* actual = frente;
        while (actual != nullptr) {
            if (pred(actual->dato)) {
                res.push_back(actual->dato);
            }
            actual = actual->siguiente;
        }
        return res;
    }
};