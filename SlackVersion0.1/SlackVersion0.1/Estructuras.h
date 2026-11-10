#pragma once
#include <iostream>
#include <functional>
#include <vector>
using namespace std;

// ==================== LISTA DOBLE ENLAZADA ====================
template <typename T>
class ListaDoble {
private:
    struct Nodo {
        T dato;
        Nodo* sig;
        Nodo* ant;
        Nodo(const T& d) : dato(d), sig(nullptr), ant(nullptr) {}
    };
    Nodo* cabeza;
    Nodo* cola;
    int tam;

    // Metodo recursivo: Busqueda recursiva
    Nodo* buscarRec(Nodo* n, const T& d) const {
        if (!n) return nullptr;
        if (n->dato == d) return n;
        return buscarRec(n->sig, d);
    }

public:
    ListaDoble() : cabeza(nullptr), cola(nullptr), tam(0) {}
    ~ListaDoble() { vaciar(); }

    void vaciar() {
        Nodo* act = cabeza;
        while (act) {
            Nodo* sig = act->sig;
            delete act;
            act = sig;
        }
        cabeza = cola = nullptr;
        tam = 0;
    }

    void insertarInicio(const T& d) {
        Nodo* n = new Nodo(d);
        if (!cabeza) { cabeza = cola = n; }
        else { n->sig = cabeza; cabeza->ant = n; cabeza = n; }
        tam++;
    }

    void insertarFinal(const T& d) {
        Nodo* n = new Nodo(d);
        if (!cola) { cabeza = cola = n; }
        else { n->ant = cola; cola->sig = n; cola = n; }
        tam++;
    }

    bool eliminar(const T& d) {
        Nodo* act = cabeza;
        while (act) {
            if (act->dato == d) {
                if (act->ant) act->ant->sig = act->sig; else cabeza = act->sig;
                if (act->sig) act->sig->ant = act->ant; else cola = act->ant;
                delete act;
                tam--;
                return true;
            }
            act = act->sig;
        }
        return false;
    }

    T* buscar(const T& d) const {
        Nodo* n = buscarRec(cabeza, d);
        return n ? &(n->dato) : nullptr;
    }

    // --- METODOS NUEVOS ---
    // 1. Conteo condicional con lambda
    int contarSi(function<bool(const T&)> predicado) const {
        int c = 0;
        Nodo* act = cabeza;
        while (act) {
            if (predicado(act->dato)) c++;
            act = act->sig;
        }
        return c;
    }

    // 2. Modificacion selectiva in-situ con lambda
    void modificarSi(function<bool(const T&)> predicado, function<void(T&)> mutador) {
        Nodo* act = cabeza;
        while (act) {
            if (predicado(act->dato)) mutador(act->dato);
            act = act->sig;
        }
    }

    // 3. Extraccion de sublista como vector segun criterio
    vector<T> filtrar(function<bool(const T&)> predicado) const {
        vector<T> res;
        Nodo* act = cabeza;
        while (act) {
            if (predicado(act->dato)) res.push_back(act->dato);
            act = act->sig;
        }
        return res;
    }

    void mostrar(function<void(const T&)> f) const {
        Nodo* act = cabeza;
        while (act) { f(act->dato); act = act->sig; }
    }

    void recorrerInverso(function<void(const T&)> f) const {
        Nodo* act = cola;
        while (act) { f(act->dato); act = act->ant; }
    }

    int size() const { return tam; }
    bool vacia() const { return tam == 0; }
};


// ==================== PILA ====================
template <typename T>
class Pila {
private:
    struct Nodo {
        T dato;
        Nodo* sig;
        Nodo(const T& d) : dato(d), sig(nullptr) {}
    };
    Nodo* tope;
    int tam;

public:
    Pila() : tope(nullptr), tam(0) {}
    ~Pila() {
        while (tope) {
            Nodo* a = tope;
            tope = tope->sig;
            delete a;
        }
    }

    void push(const T& d) {
        Nodo* n = new Nodo(d);
        n->sig = tope;
        tope = n;
        tam++;
    }

    bool pop(T& out) {
        if (!tope) return false;
        out = tope->dato;
        Nodo* a = tope;
        tope = tope->sig;
        delete a;
        tam--;
        return true;
    }

    T* peek() const { return tope ? &(tope->dato) : nullptr; }
    bool vacia() const { return tope == nullptr; }
    int size() const { return tam; }

    // --- METODOS NUEVOS ---
    // 1. Obtener elemento del fondo sin perder el orden
    bool verFondo(T& out) const {
        if (!tope) return false;
        Nodo* act = tope;
        while (act->sig) act = act->sig;
        out = act->dato;
        return true;
    }

    // 2. Eliminar ocurrencias que cumplan una condici�n lambda
    int eliminarSi(function<bool(const T&)> predicado) {
        Nodo* act = tope;
        Nodo* prev = nullptr;
        int borrados = 0;
        while (act) {
            if (predicado(act->dato)) {
                Nodo* aBorrar = act;
                if (!prev) tope = act->sig;
                else prev->sig = act->sig;
                act = act->sig;
                delete aBorrar;
                tam--;
                borrados++;
            }
            else {
                prev = act;
                act = act->sig;
            }
        }
        return borrados;
    }

    // 3. Volcar elementos a un vector sin destruir la pila
    vector<T> toVector() const {
        vector<T> res;
        Nodo* act = tope;
        while (act) { res.push_back(act->dato); act = act->sig; }
        return res;
    }

    void mostrar(function<void(const T&)> f) const {
        Nodo* act = tope;
        while (act) { f(act->dato); act = act->sig; }
    }
};

// ==================== COLA ====================
template <typename T>
class Cola {
private:
    struct Nodo {
        T dato;
        Nodo* sig;
        Nodo(const T& d) : dato(d), sig(nullptr) {}
    };
    Nodo* frente;
    Nodo* fin;
    int tam;

public:
    Cola() : frente(nullptr), fin(nullptr), tam(0) {}
    ~Cola() {
        while (frente) {
            Nodo* a = frente;
            frente = frente->sig;
            delete a;
        }
    }

    void encolar(const T& d) {
        Nodo* n = new Nodo(d);
        if (fin) fin->sig = n; else frente = n;
        fin = n;
        tam++;
    }

    bool desencolar(T& out) {
        if (!frente) return false;
        out = frente->dato;
        Nodo* a = frente;
        frente = frente->sig;
        if (!frente) fin = nullptr;
        delete a;
        tam--;
        return true;
    }

    T* verFrente() const { return frente ? &(frente->dato) : nullptr; }
    bool vacia() const { return frente == nullptr; }
    int size() const { return tam; }

    // --- METODOS NUEVOS ---
    // 1. Vaciar cola por lotes
    int desencolarLote(int k, vector<T>& lote) {
        int sacados = 0;
        T temp;
        while (sacados < k && desencolar(temp)) {
            lote.push_back(temp);
            sacados++;
        }
        return sacados;
    }

    // 2. Reencolar elemento prioritario hacia el frente
    void encolarPrioritario(const T& d) {
        Nodo* n = new Nodo(d);
        n->sig = frente;
        frente = n;
        if (!fin) fin = n;
        tam++;
    }

    // 3. Inspeccionar existencia con lambda
    bool existe(function<bool(const T&)> predicado) const {
        Nodo* act = frente;
        while (act) {
            if (predicado(act->dato)) return true;
            act = act->sig;
        }
        return false;
    }

    void mostrar(function<void(const T&)> f) const {
        Nodo* act = frente;
        while (act) { f(act->dato); act = act->sig; }
    }
};