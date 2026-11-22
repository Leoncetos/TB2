#pragma once
#include <vector>

using namespace std;

template <typename T>
struct NodoPila {
    T dato;
    NodoPila<T>* siguiente;

    NodoPila(T d) {
        dato = d;
        siguiente = nullptr;
    }
};

template <typename T>
class Pila {
private:
    NodoPila<T>* cima;
    int tamano;

public:
    Pila() {
        cima = nullptr;
        tamano = 0;
    }

    ~Pila() { limpiar(); }

    void limpiar() {
        while (!estaVacia()) pop();
    }

    void push(T valor) {
        NodoPila<T>* nuevo = new NodoPila<T>(valor);
        nuevo->siguiente = cima;
        cima = nuevo;
        tamano++;
    }

    bool pop() {
        if (estaVacia()) return false;
        NodoPila<T>* temp = cima;
        cima = cima->siguiente;
        delete temp;
        tamano--;
        return true;
    }

    T top() const { return cima->dato; }
    bool estaVacia() const { return cima == nullptr; }
    int getTamano() const { return tamano; }

    T verFondo() const {
        NodoPila<T>* actual = cima;
        while (actual != nullptr && actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        return actual->dato;
    }

    vector<T> listarTodos(int limite = -1) const {
        vector<T> res;
        NodoPila<T>* actual = cima;
        while (actual != nullptr && (limite < 0 || (int)res.size() < limite)) {
            res.push_back(actual->dato);
            actual = actual->siguiente;
        }
        return res;
    }

    vector<T> deshacerLote(int k) {
        vector<T> restauradas;
        int cont = 0;
        while (!estaVacia() && cont < k) {
            restauradas.push_back(top());
            pop();
            cont++;
        }
        return restauradas;
    }
};
