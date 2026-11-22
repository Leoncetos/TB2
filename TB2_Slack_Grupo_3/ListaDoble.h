#pragma once
#include <vector>

using namespace std;

// --- LISTA DOBLEMENTE ENLAZADA ---
template <typename T>
struct NodoDoble {
    T dato;
    NodoDoble<T>* siguiente;
    NodoDoble<T>* anterior;

    NodoDoble(T d) {
        dato = d;
        siguiente = nullptr;
        anterior = nullptr;
    }
};

template <typename T>
class ListaDoble {
private:
    NodoDoble<T>* cabeza;
    NodoDoble<T>* cola;
    int tamano;

    template <typename Predicado>
    int contarSiRecAux(NodoDoble<T>* nodo, Predicado pred) const {
        if (nodo == nullptr) return 0;
        return (pred(nodo->dato) ? 1 : 0) + contarSiRecAux(nodo->siguiente, pred);
    }

public:
    ListaDoble() {
        cabeza = nullptr;
        cola = nullptr;
        tamano = 0;
    }

    ~ListaDoble() { limpiar(); }

    void limpiar() {
        NodoDoble<T>* actual = cabeza;
        while (actual != nullptr) {
            NodoDoble<T>* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
        cabeza = nullptr;
        cola = nullptr;
        tamano = 0;
    }

    T* insertarInicio(T valor) {
        NodoDoble<T>* nuevo = new NodoDoble<T>(valor);
        if (cabeza == nullptr) {
            cabeza = nuevo;
            cola = nuevo;
        }
        else {
            nuevo->siguiente = cabeza;
            cabeza->anterior = nuevo;
            cabeza = nuevo;
        }
        tamano++;
        return &(nuevo->dato);
    }

    T* insertarFinal(T valor) {
        NodoDoble<T>* nuevo = new NodoDoble<T>(valor);
        if (cabeza == nullptr) {
            cabeza = nuevo;
            cola = nuevo;
        }
        else {
            cola->siguiente = nuevo;
            nuevo->anterior = cola;
            cola = nuevo;
        }
        tamano++;
        return &(nuevo->dato);
    }

    bool eliminarInicio() {
        if (cabeza == nullptr) return false;
        NodoDoble<T>* temp = cabeza;
        if (cabeza == cola) {
            cabeza = nullptr;
            cola = nullptr;
        }
        else {
            cabeza = cabeza->siguiente;
            cabeza->anterior = nullptr;
        }
        delete temp;
        tamano--;
        return true;
    }

    bool eliminarFinal() {
        if (cola == nullptr) return false;
        NodoDoble<T>* temp = cola;
        if (cabeza == cola) {
            cabeza = nullptr;
            cola = nullptr;
        }
        else {
            cola = cola->anterior;
            cola->siguiente = nullptr;
        }
        delete temp;
        tamano--;
        return true;
    }

    template <typename Predicado>
    bool eliminarSi(Predicado pred) {
        NodoDoble<T>* actual = cabeza;
        while (actual != nullptr) {
            if (pred(actual->dato)) {
                if (actual == cabeza) {
                    return eliminarInicio();
                }
                if (actual == cola) {
                    return eliminarFinal();
                }
                actual->anterior->siguiente = actual->siguiente;
                actual->siguiente->anterior = actual->anterior;
                delete actual;
                tamano--;
                return true;
            }
            actual = actual->siguiente;
        }
        return false;
    }

    int getTamano() const { return tamano; }
    NodoDoble<T>* getCabeza() const { return cabeza; }
    NodoDoble<T>* getCola() const { return cola; }

    template <typename Predicado>
    vector<T> filtrar(Predicado pred, int limite = -1, bool desdeFinal = false) const {
        vector<T> res;
        NodoDoble<T>* actual = desdeFinal ? cola : cabeza;
        while (actual != nullptr && (limite < 0 || (int)res.size() < limite)) {
            if (pred(actual->dato)) {
                res.push_back(actual->dato);
            }
            actual = desdeFinal ? actual->anterior : actual->siguiente;
        }
        return res;
    }

    template <typename Predicado>
    T* buscarPtr(Predicado pred, bool desdeFinal = true) {
        NodoDoble<T>* actual = desdeFinal ? cola : cabeza;
        while (actual != nullptr) {
            if (pred(actual->dato)) return &(actual->dato);
            actual = desdeFinal ? actual->anterior : actual->siguiente;
        }
        return nullptr;
    }

    template <typename Predicado>
    int contarSiRecursivo(Predicado pred, int maxNodos = 2000) const {
        if (cola == nullptr || maxNodos <= 0) return 0;
        NodoDoble<T>* actual = cola;
        int pasos = 1;
        while (actual->anterior != nullptr && pasos < maxNodos) {
            actual = actual->anterior;
            pasos++;
        }
        return contarSiRecAux(actual, pred);
    }

    vector<T> aVector() const {
        vector<T> vec;
        vec.reserve(tamano);
        NodoDoble<T>* actual = cabeza;
        while (actual != nullptr) {
            vec.push_back(actual->dato);
            actual = actual->siguiente;
        }
        return vec;
    }
};