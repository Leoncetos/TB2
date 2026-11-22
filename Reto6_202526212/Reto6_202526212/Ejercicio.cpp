#include <iostream>
#include <algorithm>
using namespace std;

//Template
template<typename T>
struct Nodo {
    T dato;
    Nodo<T>* izquierda;
    Nodo<T>* derecha;

    Nodo(T valor) {
        dato = valor;
        izquierda = nullptr;
        derecha = nullptr;
    }
};


template<typename T>
class BST {
private:
    Nodo<T>* raiz;

    // Insertar un elemento
    bool insertar(Nodo<T>*& nodo, T dato) {
        if (nodo == nullptr) {
            nodo = new Nodo<T>(dato);
            return true;
        }
        if (dato < nodo->dato)
            return insertar(nodo->izquierda, dato);
        if (dato > nodo->dato)
            return insertar(nodo->derecha, dato);
        return false;
    }

    // Buscar un elemento
    Nodo<T>* buscar(Nodo<T>* nodo, T dato) {
        if (nodo == nullptr || nodo->dato == dato)
            return nodo;
        if (dato < nodo->dato)
            return buscar(nodo->izquierda, dato);
        return buscar(nodo->derecha, dato);
    }

    // Recorrido InOrden
    void inOrden(Nodo<T>* nodo) {
        if (nodo == nullptr) return;
        inOrden(nodo->izquierda);
        cout << nodo->dato << " ";
        inOrden(nodo->derecha);
    }

    // Altura
    int altura(Nodo<T>* nodo) {
        if (nodo == nullptr) return -1;
        return 1 + max(altura(nodo->izquierda), altura(nodo->derecha));
    }

    // Mínimo
    Nodo<T>* minimo(Nodo<T>* nodo) {
        if (nodo == nullptr) return nullptr;
        while (nodo->izquierda != nullptr)
            nodo = nodo->izquierda;
        return nodo;
    }

    // Máximo
    Nodo<T>* maximo(Nodo<T>* nodo) {
        if (nodo == nullptr) return nullptr;
        while (nodo->derecha != nullptr)
            nodo = nodo->derecha;
        return nodo;
    }

    // Eliminar un elemento
    bool eliminar(Nodo<T>*& nodo, T dato) {
        if (nodo == nullptr) return false;

        if (dato < nodo->dato)
            return eliminar(nodo->izquierda, dato);
        if (dato > nodo->dato)
            return eliminar(nodo->derecha, dato);

        // Caso 1: el nodo es una hoja
        if (nodo->izquierda == nullptr && nodo->derecha == nullptr) {
            delete nodo;
            nodo = nullptr;
        }
        // Caso 2: solo tiene hijo derecho
        else if (nodo->izquierda == nullptr) {
            Nodo<T>* temp = nodo;
            nodo = nodo->derecha;
            delete temp;
        }
        // Caso 3: solo tiene hijo izquierdo
        else if (nodo->derecha == nullptr) {
            Nodo<T>* temp = nodo;
            nodo = nodo->izquierda;
            delete temp;
        }
        // Caso 4: tiene dos hijos
        else {
            Nodo<T>* sucesor = minimo(nodo->derecha);
            nodo->dato = sucesor->dato;
            eliminar(nodo->derecha, sucesor->dato);
        }
        return true;
    }

    int _contarEnRango(Nodo<T>* nodo, T min, T max) {
        if (nodo == nullptr) return 0;

        int contador = 0;

        if (nodo->dato >= min && nodo->dato <= max)
            contador++;

        if (nodo->dato > min)
            contador += _contarEnRango(nodo->izquierda, min, max);

        if (nodo->dato < max)
            contador += _contarEnRango(nodo->derecha, min, max);

        return contador;
    }

    // Mostrar nodos
    void _mostrarEnRango(Nodo<T>* nodo, T min, T max) {
        if (nodo == nullptr) return;

        if (nodo->dato > min)
            _mostrarEnRango(nodo->izquierda, min, max);

        if (nodo->dato >= min && nodo->dato <= max)
            cout << nodo->dato << " ";

        if (nodo->dato < max)
            _mostrarEnRango(nodo->derecha, min, max);
    }

    // Liberar memoria
    void liberar(Nodo<T>* nodo) {
        if (nodo == nullptr) return;
        liberar(nodo->izquierda);
        liberar(nodo->derecha);
        delete nodo;
    }

public:
    BST() { raiz = nullptr; }
    ~BST() { liberar(raiz); }

    bool insertar(T dato) { return insertar(raiz, dato); }

    bool buscar(T dato) { return buscar(raiz, dato) != nullptr; }

    void mostrarInOrden() { inOrden(raiz); }

    int altura() { return altura(raiz); }

    const T* obtenerMinimo() {
        Nodo<T>* n = minimo(raiz);
        return n ? &n->dato : nullptr;
    }

    const T* obtenerMaximo() {
        Nodo<T>* n = maximo(raiz);
        return n ? &n->dato : nullptr;
    }

    bool eliminar(T dato) { return eliminar(raiz, dato); }

    bool estaVacio() { return raiz == nullptr; }

    int contarEnRango(T min, T max) {
        return _contarEnRango(raiz, min, max);
    }

    void mostrarEnRango(T min, T max) {
        _mostrarEnRango(raiz, min, max);
    }
};

void GestionSensores() {
    BST<int> arbol;
    cout << "Se ha cargado los siguientes sensores: 10, 6, 26, 4, 8, 7, 9, 15, 20, 23, 30";
    arbol.insertar(10);
    arbol.insertar(6);
    arbol.insertar(26);
    arbol.insertar(4);
    arbol.insertar(8);
    arbol.insertar(7);
    arbol.insertar(9);
    arbol.insertar(15);
    arbol.insertar(20);
    arbol.insertar(20);
    arbol.insertar(23);
    arbol.insertar(30);
    int opcion;
    do {
        cout << "\n=== Gestion de sensores ambientales ===" << endl;
        cout << "1. Ingresar lectura" << endl;
        cout << "2. Buscar lectura" << endl;
        cout << "3. Mostrar en orden" << endl;
        cout << "4. Obtener lecturas dentro de un rango" << endl;
        cout << "5. Mostrar altura del arbol" << endl;
        cout << "6. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1: {
            int lectura;
            cout << "Ingrese la lectura: ";
            cin >> lectura;
            if (arbol.insertar(lectura))
                cout << "Lectura registrada correctamente." << endl;
            else
                cout << "Error: lectura duplicada, no registrada." << endl;
            break;
        }
        case 2: {
            int lectura;
            cout << "Ingrese la lectura a buscar: ";
            cin >> lectura;
            if (arbol.buscar(lectura))
                cout << "Lectura encontrada." << endl;
            else
                cout << "Lectura no encontrada." << endl;
            break;
        }
        case 3: {
            if (arbol.estaVacio()) {
                cout << "El arbol esta vacio." << endl;
            }
            else {
                cout << "Lecturas en orden ascendente: ";
                arbol.mostrarInOrden();
                cout << endl;
            }
            break;
        }
        case 4: {
            if (arbol.estaVacio()) {
                cout << "El arbol esta vacio." << endl;
                break;
            }
            int minR, maxR;
            cout << "Ingrese la lectura minima: ";cin >> minR;
            cout << "Ingrese la lectura maxima: ";cin >> maxR;

            if (minR > maxR) {
                cout << "Error: la lectura minima no puede ser mayor que la maxima." << endl;
                break;
            }

            int cantidad = arbol.contarEnRango(minR, maxR);

            cout << "\nCantidad de lecturas en el rango (" << minR << " , " << maxR  << "): " << cantidad << endl;

            if (cantidad > 0) {
                cout << "Lecturas dentro del rango: ";
                arbol.mostrarEnRango(minR, maxR);
                cout << endl;
            }
            else {
                cout << "No hay lecturas dentro del rango indicado." << endl;
            }
            break;
        }
        case 5: {
            cout << "Altura del arbol: " << arbol.altura() << endl;
            break;
        }
        case 6:
            cout << "Saliendo del sistema..." << endl;
            break;
        default:
            cout << "Opcion no valida." << endl;
        }
    } while (opcion != 6);
}