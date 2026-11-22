#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <sstream>

using namespace std;

// ============================================================================
// UTILIDADES DE TIEMPO, FORMATO Y ALEATORIEDAD
// ============================================================================
string dosDigitos(int valor) {
    if (valor < 10) {
        return "0" + to_string(valor);
    }
    return to_string(valor);
}

string obtenerTimestampActual() {
    time_t ahora = time(nullptr);
    tm* tiempoLocal = localtime(&ahora);
    int anio = 1900 + tiempoLocal->tm_year;
    int mes = 1 + tiempoLocal->tm_mon;
    int dia = tiempoLocal->tm_mday;
    int hora = tiempoLocal->tm_hour;
    int minuto = tiempoLocal->tm_min;
    return to_string(anio) + "-" + dosDigitos(mes) + "-" + dosDigitos(dia) +
           " " + dosDigitos(hora) + ":" + dosDigitos(minuto);
}

int aleatorioRango(int limite) {
    long long r = (long long)rand() * 32768LL + rand();
    return (int)(r % limite);
}

// ============================================================================
// FUNCIONES HASH PROPIAS COMPLEJAS (ENTERO Y CADENA)
// ============================================================================

// Funcion hash compleja para claves enteras: mezcla multiplicativa con primos y corrimiento de bits
unsigned long long calcularHashPropio(int clave) {
    unsigned long long x = (clave < 0) ? (unsigned long long)(-(long long)clave) : (unsigned long long)clave;
    x = (x ^ (x >> 16)) * 2654435761ULL;
    x = (x ^ (x >> 13)) * 2246822519ULL;
    x = x ^ (x >> 16);
    return x;
}

// Funcion hash compleja para cadenas: acumulacion polinomica (Horner), ponderacion por posicion y mezcla
unsigned long long calcularHashPropio(const string& clave) {
    unsigned long long h = 5381ULL;
    const unsigned long long base = 31ULL;
    const unsigned long long primo = 1000000007ULL;
    for (int i = 0; i < (int)clave.length(); i++) {
        unsigned long long c = (unsigned char)clave[i];
        h = (h * base + c * (unsigned long long)(i + 1)) % primo;
        h = h ^ (h >> 7);
    }
    h = (h ^ (h >> 16)) * 2654435761ULL;
    return h ^ (h >> 13);
}

// ============================================================================
// 1. ESTRUCTURAS DE DATOS GENÉRICAS (LISTA DOBLE, PILA, COLA Y TABLA HASH)
// ============================================================================

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
        } else {
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
        } else {
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
        } else {
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
        } else {
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

// --- PILA (STACK LIFO) ---
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

// --- COLA (QUEUE FIFO) ---
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
        } else {
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

// --- TABLA HASH GENÉRICA CON ENCADENAMIENTO SEPARADO Y FUNCIÓN HASH PROPIA ---
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
                } else {
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

// ============================================================================
// 2. CLASES DEL DOMINIO DEL SISTEMA SLACK
// ============================================================================

// Clase 1: Canal (Clase base abstracta)
class Canal {
protected:
    int id;
    string nombre;
    int cantidadMensajes;

public:
    Canal(int id = 0, string nombre = "", int cantMsgs = 0) {
        this->id = id;
        this->nombre = nombre;
        this->cantidadMensajes = cantMsgs;
    }
    virtual ~Canal() {}

    int getId() const { return id; }
    string getNombre() const { return nombre; }
    int getCantidadMensajes() const { return cantidadMensajes; }
    void incrementarMensajes() { cantidadMensajes++; }

    virtual string getTipo() const = 0;
    virtual bool puedeEntrar(int idUsuario) const = 0;
    virtual bool agregarUsuario(int) { return true; }
    virtual vector<int> getAccesos() const { return {}; }
};

// Clase 2: CanalPublico
class CanalPublico : public Canal {
public:
    CanalPublico(int id = 0, string nombre = "", int cantMsgs = 0)
        : Canal(id, nombre, cantMsgs) {}

    string getTipo() const override { return "publico"; }
    bool puedeEntrar(int) const override {
        return true;
    }
};

// Clase 3: CanalPrivado
class CanalPrivado : public Canal {
private:
    vector<int> usuariosPermitidos;

public:
    CanalPrivado(int id = 0, string nombre = "", int cantMsgs = 0, vector<int> permitidos = {})
        : Canal(id, nombre, cantMsgs) {
        usuariosPermitidos = permitidos;
    }

    string getTipo() const override { return "privado"; }

    bool puedeEntrar(int idUsuario) const override {
        for (int u : usuariosPermitidos) {
            if (u == idUsuario) return true;
        }
        return false;
    }

    bool agregarUsuario(int idUsuario) override {
        if (!puedeEntrar(idUsuario)) {
            usuariosPermitidos.push_back(idUsuario);
            return true;
        }
        return false;
    }

    vector<int> getAccesos() const override { return usuariosPermitidos; }
};

// Clase base auxiliar para polimorfismo de contenido de mensajes
class ContenidoMensaje {
public:
    virtual ~ContenidoMensaje() {}
    virtual string generarContenido() const = 0;
    virtual string getTipo() const = 0;
};

// Clase 4: MensajeTexto
class MensajeTexto : public ContenidoMensaje {
private:
    string texto;

public:
    MensajeTexto(string t = "") { texto = t; }
    string generarContenido() const override { return texto; }
    string getTipo() const override { return "texto"; }
};

// Clase 5: MensajeArchivo
class MensajeArchivo : public ContenidoMensaje {
private:
    string nombreArchivo;

public:
    MensajeArchivo(string arch = "") { nombreArchivo = arch; }
    string generarContenido() const override { return "[" + nombreArchivo + "]"; }
    string getTipo() const override { return "archivo"; }
};

// Clase 6: Emoji
class Emoji : public ContenidoMensaje {
private:
    string nombre;

public:
    Emoji(string nom = "") { nombre = nom; }
    string generarContenido() const override { return ":" + nombre + ":"; }
    string getTipo() const override { return "emoji"; }
};

// Clase 7: Rol
class Rol {
public:
    int id;
    string nombre;
    string descripcion;

    Rol(int id = 1, string nom = "Miembro", string desc = "Usuario estandar del espacio") {
        this->id = id;
        this->nombre = nom;
        this->descripcion = desc;
    }
};

// Clase 8: Estado
class Estado {
public:
    int id;
    int idUsuario;
    string texto;
    string emoji;

    Estado(int id = 1, int idU = 0, string txt = "activo", string em = ":online:") {
        this->id = id;
        this->idUsuario = idU;
        this->texto = txt;
        this->emoji = em;
    }
};

// Clase 9: Usuario
class Usuario {
public:
    int id;
    string nombre;
    string email;
    string estado;
    string rol;

    Usuario(int id = 0, string nom = "", string mail = "", string est = "activo", string r = "Miembro") {
        this->id = id;
        this->nombre = nom;
        this->email = mail;
        this->estado = est;
        this->rol = r;
    }
};

// Clase 10: Amistad
class Amistad {
public:
    int id;
    int idUsuario1;
    int idUsuario2;

    Amistad(int id = 0, int u1 = 0, int u2 = 0) {
        this->id = id;
        this->idUsuario1 = u1;
        this->idUsuario2 = u2;
    }
};

// Clase 11: Mensaje
class Mensaje {
public:
    int id;
    int idAutor;
    int idCanal;
    int idPadre;
    string tipoContenido;
    string contenido;
    string fecha;

    Mensaje(int id = 0, int autor = 0, int canal = 0, int padre = 0, string tipo = "texto", string cont = "", string f = "") {
        this->id = id;
        this->idAutor = autor;
        this->idCanal = canal;
        this->idPadre = padre;
        this->tipoContenido = tipo;
        this->contenido = cont;
        this->fecha = f;
    }
};

// Clase 12: Hilo
class Hilo {
public:
    int id;
    int idMensajePadre;

    Hilo(int id = 0, int idPadre = 0) {
        this->id = id;
        this->idMensajePadre = idPadre;
    }
};

// Clase 13: Notificacion
class Notificacion {
public:
    int id;
    int idUsuario;
    int idMensaje;
    string tipo;
    bool leida;

    Notificacion(int id = 0, int idU = 0, int idM = 0, string t = "mensaje_nuevo", bool l = false) {
        this->id = id;
        this->idUsuario = idU;
        this->idMensaje = idM;
        this->tipo = t;
        this->leida = l;
    }
};

// Clase 14: RegistroActividad
class RegistroActividad {
public:
    int id;
    int idUsuario;
    string fecha;
    string accion;
    string detalle;

    RegistroActividad(int id = 0, int idU = 0, string f = "", string acc = "", string det = "") {
        this->id = id;
        this->idUsuario = idU;
        this->fecha = f;
        this->accion = acc;
        this->detalle = det;
    }
};

// ============================================================================
// 3. ALGORITMOS DE ORDENAMIENTO AVANZADO (QUICKSORT, MERGESORT, HEAPSORT, TIMSORT)
// ============================================================================

template <typename T, typename Comparador>
int particionQuick(vector<T>& arr, int bajo, int alto, Comparador comp) {
    // Seleccion de pivote central para evitar peor caso O(n^2) en arreglos ordenados
    int medio = bajo + (alto - bajo) / 2;
    swap(arr[medio], arr[alto]);
    T pivote = arr[alto];
    int i = bajo - 1;
    for (int j = bajo; j < alto; j++) {
        if (comp(arr[j], pivote)) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[alto]);
    return i + 1;
}

template <typename T, typename Comparador>
void quickSort(vector<T>& arr, int bajo, int alto, Comparador comp) {
    if (bajo < alto) {
        int pi = particionQuick(arr, bajo, alto, comp);
        quickSort(arr, bajo, pi - 1, comp);
        quickSort(arr, pi + 1, alto, comp);
    }
}

template <typename T, typename Comparador>
void merge(vector<T>& arr, int izq, int medio, int der, Comparador comp) {
    int n1 = medio - izq + 1;
    int n2 = der - medio;
    vector<T> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[izq + i];
    for (int j = 0; j < n2; j++) R[j] = arr[medio + 1 + j];

    int i = 0, j = 0, k = izq;
    while (i < n1 && j < n2) {
        if (comp(L[i], R[j])) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

template <typename T, typename Comparador>
void mergeSort(vector<T>& arr, int izq, int der, Comparador comp) {
    if (izq < der) {
        int medio = izq + (der - izq) / 2;
        mergeSort(arr, izq, medio, comp);
        mergeSort(arr, medio + 1, der, comp);
        merge(arr, izq, medio, der, comp);
    }
}

template <typename T, typename Comparador>
void heapify(vector<T>& arr, int n, int i, Comparador comp) {
    int extremo = i;
    int izq = 2 * i + 1;
    int der = 2 * i + 2;

    if (izq < n && comp(arr[extremo], arr[izq])) extremo = izq;
    if (der < n && comp(arr[extremo], arr[der])) extremo = der;

    if (extremo != i) {
        swap(arr[i], arr[extremo]);
        heapify(arr, n, extremo, comp);
    }
}

template <typename T, typename Comparador>
void heapSort(vector<T>& arr, Comparador comp) {
    int n = (int)arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i, comp);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0, comp);
    }
}

template <typename T, typename Comparador>
void insertionSortRango(vector<T>& arr, int inicio, int fin, Comparador comp) {
    for (int i = inicio + 1; i <= fin; i++) {
        T actual = arr[i];
        int j = i - 1;
        while (j >= inicio && comp(actual, arr[j])) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = actual;
    }
}

template <typename T, typename Comparador>
void timSort(vector<T>& arr, Comparador comp) {
    int n = (int)arr.size();
    const int TAMANIO_RUN = 32;

    // 1. Ordenar bloques pequenos (runs) con Insertion Sort
    for (int inicio = 0; inicio < n; inicio += TAMANIO_RUN) {
        int fin = inicio + TAMANIO_RUN - 1;
        if (fin >= n) fin = n - 1;
        insertionSortRango(arr, inicio, fin, comp);
    }

    // 2. Mezclar los runs ordenados de forma iterativa con Merge
    for (int tam = TAMANIO_RUN; tam < n; tam = tam * 2) {
        for (int inicio = 0; inicio < n; inicio += tam * 2) {
            int medio = inicio + tam - 1;
            int fin = inicio + tam * 2 - 1;
            if (medio >= n - 1) continue;
            if (fin >= n) fin = n - 1;
            merge(arr, inicio, medio, fin, comp);
        }
    }
}

// ============================================================================
// CLASE 15: SlackSistema (CONTROLADOR CENTRAL DEL SISTEMA)
// ============================================================================
class SlackSistema {
private:
    ListaDoble<Usuario> usuarios;
    ListaDoble<Mensaje> mensajes;
    ListaDoble<Canal*> canales;
    ListaDoble<Hilo> hilos;
    ListaDoble<Rol> roles;
    ListaDoble<Estado> estados;
    ListaDoble<Amistad> amistades;
    ListaDoble<RegistroActividad> actividad;
    Pila<Mensaje> ediciones;
    Cola<Notificacion> notificaciones;

    // Indices Hash: por ID entero y por nombre de canal (cadena)
    TablaHash<int, Usuario*> indiceUsuarios;
    TablaHash<int, Canal*> indiceCanales;
    TablaHash<string, Canal*> indiceCanalesNombre;

    int nextUserId;
    int nextCanalId;
    int nextMensajeId;
    int nextHiloId;
    int nextNotifId;
    int nextActId;
    int nextAmistadId;

    void liberarCanales() {
        NodoDoble<Canal*>* actual = canales.getCabeza();
        while (actual != nullptr) {
            delete actual->dato;
            actual = actual->siguiente;
        }
        canales.limpiar();
        indiceCanales.limpiar();
        indiceCanalesNombre.limpiar();
    }

    int leerEnteroSeguro() {
        string linea;
        if (!getline(cin, linea)) return -1;

        istringstream entrada(linea);
        int valor;
        char sobrante;
        if (!(entrada >> valor) || (entrada >> sobrante)) return -1;
        return valor;
    }

    void registrarLog(int idUsuario, const string& accion, const string& detalle) {
        actividad.insertarFinal(RegistroActividad(
            nextActId++, idUsuario, obtenerTimestampActual(), accion, detalle
        ));
    }

    int imprimirRespuestasRec(const vector<Mensaje>& msgsCanal, int idPadre, int nivel) {
        int contador = 0;
        for (const auto& m : msgsCanal) {
            if (m.idPadre == idPadre) {
                for (int i = 0; i < nivel; i++) cout << "    |--- ";
                cout << "[Respuesta #" << m.id << "] Autor " << m.idAutor << ": " << m.contenido << "\n";
                contador += 1 + imprimirRespuestasRec(msgsCanal, m.id, nivel + 1);
            }
        }
        return contador;
    }

    // Generador de contenido variado para los mensajes del dataset
    string generarContenidoAleatorio(int idMensaje, int nUsuarios, string& tipoOut) {
        const char* verbos[] = {
            "Revisando", "Subiendo", "Validando", "Corrigiendo", "Compilando", "Probando",
            "Optimizando", "Documentando", "Analizando", "Desplegando", "Integrando", "Verificando",
            "Actualizando", "Depurando", "Coordinando", "Compartiendo", "Exportando", "Consultando"
        };
        const char* sustantivos[] = {
            "el diagrama UML", "la lista doble", "el algoritmo QuickSort", "la pila de ediciones",
            "la cola de notificaciones", "el historial en txt", "los canales privados", "el hilo recursivo",
            "el modulo de usuarios", "la tabla hash", "el reporte de complejidad", "los permisos de acceso",
            "la interfaz en consola", "el commit final", "las pruebas unitarias", "el parser de archivos",
            "el ordenamiento MergeSort", "el monticulo HeapSort", "la estructura de datos", "el sprint actual"
        };
        const char* complementos[] = {
            "para la entrega de hoy", "sin errores de memoria", "segun lo acordado en reunion",
            "antes del cierre del bloque", "con datos de prueba", "en el entorno local",
            "con complejidad logaritmica", "para revisar en equipo", "listo para validar",
            "con todos los casos borde", "en la rama principal", "con los filtros lambda",
            "verificado en consola", "pendiente de aprobacion", "con formato correcto"
        };
        const char* archivos[] = {
            "diagrama_uml.png", "informe_tb2.pdf", "main.cpp", "estructuras.h",
            "capturas_consola.zip", "analisis_bigo.docx", "dataset_prueba.txt", "notas_reunion.md"
        };
        const char* emojis[] = {
            "rocket", "white_check_mark", "eyes", "thumbsup", "fire", "computer", "clap", "bulb"
        };

        int modo = rand() % 10;
        if (modo == 0) {
            tipoOut = "archivo";
            return string("[") + archivos[rand() % 8] + "]";
        } else if (modo == 1) {
            tipoOut = "emoji";
            return string(":") + emojis[rand() % 8] + ":";
        } else {
            tipoOut = "texto";
            string frase = string(verbos[rand() % 18]) + " " +
                           sustantivos[rand() % 20] + " " +
                           complementos[rand() % 15];
            if (rand() % 4 == 0) {
                int idMencion = aleatorioRango(nUsuarios) + 1;
                frase += " @" + to_string(idMencion);
            }
            if (rand() % 3 == 0) {
                frase += " (ref #" + to_string(idMensaje) + ")";
            }
            return frase;
        }
    }

    // Divide una cadena usando un delimitador
    vector<string> separarCadena(const string& linea, char delim) {
        vector<string> partes;
        string actual = "";
        for (int i = 0; i < (int)linea.length(); i++) {
            if (linea[i] == delim) {
                partes.push_back(actual);
                actual = "";
            } else if (linea[i] != '\r') {
                actual += linea[i];
            }
        }
        partes.push_back(actual);
        return partes;
    }

public:
    SlackSistema() : indiceUsuarios(15013), indiceCanales(1009), indiceCanalesNombre(1009) {
        nextUserId = 1;
        nextCanalId = 1;
        nextMensajeId = 1;
        nextHiloId = 1;
        nextNotifId = 1;
        nextActId = 1;
        nextAmistadId = 1;

        srand((unsigned int)time(nullptr));

        roles.insertarFinal(Rol(1, "Admin", "Administrador total del workspace"));
        roles.insertarFinal(Rol(2, "Miembro", "Miembro estandar de canales"));

        // Carga inicial con el dataset base de 150,000 registros
        generarDataset150k();
    }

    ~SlackSistema() {
        liberarCanales();
    }

    // ------------------------------------------------------------------------
    // GENERADOR Y CARGADOR DE 150,000 REGISTROS COMO BASE DEL SISTEMA
    // (Usuarios + Canales + Mensajes + Notificaciones + Historial de Ediciones)
    // ------------------------------------------------------------------------
    void generarDataset150k() {
        cout << "Generando 150 000 dataset\n";

        liberarCanales();
        usuarios.limpiar();
        mensajes.limpiar();
        hilos.limpiar();
        estados.limpiar();
        amistades.limpiar();
        actividad.limpiar();
        ediciones.limpiar();
        notificaciones.limpiar();
        indiceUsuarios.limpiar();

        nextUserId = 1;
        nextCanalId = 1;
        nextMensajeId = 1;
        nextHiloId = 1;
        nextNotifId = 1;
        nextActId = 1;
        nextAmistadId = 1;

        // Distribucion exacta = 150,000 registros:
        // 1) Usuarios:                10,000
        // 2) Canales:                    500
        // 3) Mensajes:               100,000
        // 4) Notificaciones:          24,500
        // 5) Historial de ediciones:  15,000
        // Total = 150,000
        const int N_USR = 10000;
        const int N_CHN = 500;
        const int N_MSG = 100000;
        const int N_NOT = 24500;
        const int N_EDI = 15000;

        const char* nombres[] = {
            "Jaime", "Walter", "Efer", "John", "Rodrigo", "Abraham", "Camila", "Valeria",
            "Mateo", "Lucas", "Sofia", "Diego", "Andrea", "Sebastian", "Lucia", "Gabriel",
            "Mariana", "Carlos", "Fernanda", "Joaquin", "Renata", "Alejandro", "Paula", "Nicolas"
        };
        const char* apellidos[] = {
            "Rojas", "Cochachi", "Avila", "Arias", "Mallqui", "Sopla", "Torres", "Mendoza",
            "Quispe", "Vargas", "Castillo", "Ramos", "Flores", "Chavez", "Salazar", "Paredes",
            "Huaman", "Delgado", "Morales", "Rios", "Vega", "Campos", "Herrera", "Ortiz"
        };
        const char* temasCanal[] = {
            "general", "proyecto-tb2", "algoritmos", "backend", "frontend", "devops",
            "avisos", "consultas", "arquitectura", "testing", "base-datos", "diseno-uml",
            "sprints", "documentacion", "recursos", "equipo-core"
        };
        const char* tiposNotif[] = {
            "mencion", "mensaje_nuevo", "respuesta_hilo", "cambio_estado"
        };

        // Conteo real de mensajes por canal para coherencia exacta
        vector<int> conteoPorCanal(N_CHN + 1, 0);

        ofstream out("dataset_150k.txt");

        // 1. Generar 10,000 Usuarios
        for (int i = 1; i <= N_USR; i++) {
            string nom = string(nombres[rand() % 24]) + " " +
                         apellidos[rand() % 24] + " " +
                         apellidos[rand() % 24];
            string mail = "u2026" + to_string(10000 + i) + "@upc.edu.pe";
            string est = (rand() % 4 == 0) ? "ausente" : "activo";
            string rol = (i <= 50 || rand() % 25 == 0) ? "Admin" : "Miembro";

            Usuario u(i, nom, mail, est, rol);
            Usuario* ptrU = usuarios.insertarFinal(u);
            indiceUsuarios.insertar(i, ptrU);
            out << "U|" << i << "|" << nom << "|" << mail << "|" << est << "|" << rol << "\n";
        }
        nextUserId = N_USR + 1;

        // Pre-asignar canal para cada mensaje para que cantidadMensajes sea 100% coherente
        vector<int> canalDeMensaje(N_MSG + 1);
        for (int i = 1; i <= N_MSG; i++) {
            int cId = aleatorioRango(N_CHN) + 1;
            canalDeMensaje[i] = cId;
            conteoPorCanal[cId]++;
        }

        // 2. Generar 500 Canales (Publicos y Privados)
        for (int i = 1; i <= N_CHN; i++) {
            string nom = string("#") + temasCanal[rand() % 16] + "-" + to_string(i);
            bool esPrivado = (rand() % 4 == 0);
            Canal* c = nullptr;
            if (esPrivado) {
                vector<int> permitidos = {1, 2, 3, aleatorioRango(N_USR) + 1};
                c = new CanalPrivado(i, nom, conteoPorCanal[i], permitidos);
            } else {
                c = new CanalPublico(i, nom, conteoPorCanal[i]);
            }
            canales.insertarFinal(c);
            indiceCanales.insertar(i, c);
            indiceCanalesNombre.insertar(nom, c);
            out << "C|" << i << "|" << nom << "|" << c->getTipo() << "|" << conteoPorCanal[i] << "\n";
        }
        nextCanalId = N_CHN + 1;

        // 3. Generar 100,000 Mensajes con contenido variado e hilos coherentes por canal
        vector<int> ultimoMensajePorCanal(N_CHN + 1, 0);
        for (int i = 1; i <= N_MSG; i++) {
            int autor = aleatorioRango(N_USR) + 1;
            int canal = canalDeMensaje[i];
            int padre = 0;
            if (ultimoMensajePorCanal[canal] > 0 && rand() % 6 == 0) {
                padre = ultimoMensajePorCanal[canal];
            }
            ultimoMensajePorCanal[canal] = i;

            int mes = (rand() % 3) + 9; // Septiembre - Noviembre
            int dia = (rand() % 28) + 1;
            int hora = rand() % 24;
            int min = rand() % 60;
            string fecha = "2026-" + dosDigitos(mes) + "-" + dosDigitos(dia) + " " +
                           dosDigitos(hora) + ":" + dosDigitos(min);

            string tipoCont;
            string contenido = generarContenidoAleatorio(i, N_USR, tipoCont);

            Mensaje m(i, autor, canal, padre, tipoCont, contenido, fecha);
            mensajes.insertarFinal(m);
            if (padre > 0) {
                hilos.insertarFinal(Hilo(nextHiloId++, padre));
            }
            out << "M|" << i << "|" << autor << "|" << canal << "|" << padre << "|" << tipoCont << "|" << contenido << "|" << fecha << "\n";
        }
        nextMensajeId = N_MSG + 1;

        // 4. Generar 24,500 Notificaciones en la Cola
        for (int i = 1; i <= N_NOT; i++) {
            int idU = aleatorioRango(N_USR) + 1;
            int idM = aleatorioRango(N_MSG) + 1;
            string tNotif = tiposNotif[rand() % 4];
            Notificacion n(i, idU, idM, tNotif, false);
            notificaciones.encolar(n);
            out << "N|" << i << "|" << idU << "|" << idM << "|" << tNotif << "|0\n";
        }
        nextNotifId = N_NOT + 1;

        // 5. Generar 15,000 Historiales de Ediciones en la Pila
        for (int i = 1; i <= N_EDI; i++) {
            int idMRef = aleatorioRango(N_MSG) + 1;
            int autor = aleatorioRango(N_USR) + 1;
            int canal = canalDeMensaje[idMRef];
            string tipoTmp;
            string textoPrevio = "[Borrador v" + to_string((rand() % 3) + 1) + "] " +
                                 generarContenidoAleatorio(idMRef, N_USR, tipoTmp);
            string fecha = "2026-11-" + dosDigitos((rand() % 20) + 1) + " " +
                           dosDigitos(rand() % 24) + ":" + dosDigitos(rand() % 60);

            Mensaje versionPrevia(idMRef, autor, canal, 0, "texto", textoPrevio, fecha);
            ediciones.push(versionPrevia);
            out << "E|" << idMRef << "|" << autor << "|" << canal << "|" << textoPrevio << "|" << fecha << "\n";
        }

        out.close();

        long totalDataset = (long)usuarios.getTamano() +
                            (long)canales.getTamano() +
                            (long)mensajes.getTamano() +
                            (long)notificaciones.getTamano() +
                            (long)ediciones.getTamano();

        registrarLog(1, "GENERACION_DATASET", "Dataset base creado con " + to_string(totalDataset) + " registros");

        cout << "Total dataset creado: " << totalDataset << " registros\n";
        cout << "  - Usuarios:               " << usuarios.getTamano() << "\n";
        cout << "  - Canales:                " << canales.getTamano() << "\n";
        cout << "  - Mensajes:               " << mensajes.getTamano() << "\n";
        cout << "  - Notificaciones:         " << notificaciones.getTamano() << "\n";
        cout << "  - Historial de ediciones: " << ediciones.getTamano() << "\n";
    }

    // ------------------------------------------------------------------------
    // OPCIÓN 1: MENÚ CANALES
    // ------------------------------------------------------------------------
    void menuCanales() {
        int op = 0;
        do {
            cout << "\nCANALES\n";
            auto listaC = canales.filtrar([](Canal*) { return true; }, -1, false);
            for (const auto& c : listaC) {
                cout << "[" << c->getId() << "] " << c->getNombre() << " | " << c->getTipo()
                     << " | " << c->getCantidadMensajes() << " mensajes\n";
            }
            cout << "1. Abrir canal\n";
            cout << "2. Crear canal\n";
            cout << "3. Agregar usuario a canal privado\n";
            cout << "4. Cambiar tipo publico/privado\n";
            cout << "5. Buscar canal por nombre (Hash String O(1))\n";
            cout << "0. Volver\n";
            cout << "Elige una opcion: ";
            op = leerEnteroSeguro();
            if (!cin) return;
            if (op == -1) continue;

            if (op == 1) {
                int idC, idU;
                cout << "ID del canal que deseas abrir: ";
                idC = leerEnteroSeguro();
                cout << "ID del usuario que ingresa: ";
                idU = leerEnteroSeguro();

                Canal** ptrC = indiceCanales.buscar(idC);
                if (ptrC == nullptr) {
                    cout << "Error: Canal no encontrado.\n";
                    continue;
                }
                Canal* canal = *ptrC;

                if (!canal->puedeEntrar(idU)) {
                    cout << "ACCESO DENEGADO: El canal " << canal->getNombre() << " es privado y el usuario #" << idU << " no tiene permiso.\n";
                    registrarLog(idU, "ACCESO_DENEGADO", "Intento abrir canal privado #" + to_string(idC));
                    continue;
                }

                cout << "\n" << canal->getNombre() << " (" << canal->getTipo() << ")\n";
                auto msgsCanal = mensajes.filtrar([idC](const Mensaje& m) {
                    return m.idCanal == idC;
                }, -1, false);

                if (!msgsCanal.empty()) {
                    mergeSort(msgsCanal, 0, (int)msgsCanal.size() - 1, [](const Mensaje& a, const Mensaje& b) {
                        if (a.fecha != b.fecha) return a.fecha < b.fecha;
                        return a.id < b.id;
                    });
                }

                for (const auto& m : msgsCanal) {
                    Usuario** u = indiceUsuarios.buscar(m.idAutor);
                    string nomAutor = (u != nullptr) ? (*u)->nombre : ("Usuario #" + to_string(m.idAutor));
                    cout << nomAutor << " | " << m.fecha;
                    if (m.idPadre > 0) cout << " | Respuesta a #" << m.idPadre;
                    cout << "\n  " << m.contenido << "\n";
                }
                registrarLog(idU, "ABRIR_CANAL", "Abrio " + canal->getNombre());
                cout << "Presiona Enter para volver a Canales...";
                cin.get();
            } else if (op == 2) {
                string nom;
                int tipoOp;
                cout << "Nombre del canal (ej. #devops): ";
                getline(cin, nom);
                if (!nom.empty() && nom[0] != '#') nom = "#" + nom;

                if (indiceCanalesNombre.buscar(nom) != nullptr) {
                    cout << "Error: Ya existe un canal con el nombre " << nom << ".\n";
                    continue;
                }

                cout << "Tipo (1 = Publico, 2 = Privado): ";
                tipoOp = leerEnteroSeguro();

                int idNuevo = nextCanalId++;
                Canal* nuevo = nullptr;
                if (tipoOp == 2) {
                    int idCreador;
                    cout << "ID del usuario inicial autorizado: ";
                    idCreador = leerEnteroSeguro();
                    nuevo = new CanalPrivado(idNuevo, nom, 0, vector<int>{idCreador});
                } else {
                    nuevo = new CanalPublico(idNuevo, nom, 0);
                }
                canales.insertarFinal(nuevo);
                indiceCanales.insertar(idNuevo, nuevo);
                indiceCanalesNombre.insertar(nom, nuevo);
                registrarLog(1, "CREAR_CANAL", "Creo canal " + nom + " (" + nuevo->getTipo() + ")");
                cout << "OK: Canal " << nom << " creado con ID #" << idNuevo << ".\n";
            } else if (op == 3) {
                int idC, idU;
                cout << "ID del canal privado: ";
                idC = leerEnteroSeguro();
                cout << "ID del usuario a autorizar: ";
                idU = leerEnteroSeguro();

                Canal** ptrC = indiceCanales.buscar(idC);
                if (ptrC != nullptr && (*ptrC)->getTipo() == "privado") {
                    if ((*ptrC)->agregarUsuario(idU)) {
                        registrarLog(idU, "AGREGAR_ACCESO", "Usuario #" + to_string(idU) + " agregado a canal #" + to_string(idC));
                        cout << "OK: Usuario #" << idU << " autorizado en " << (*ptrC)->getNombre() << ".\n";
                    } else {
                        cout << "El usuario ya tenia acceso.\n";
                    }
                } else {
                    cout << "El canal no existe o no es privado.\n";
                }
            } else if (op == 4) {
                int idC;
                cout << "ID del canal a cambiar tipo: ";
                idC = leerEnteroSeguro();

                Canal** ptrC = indiceCanales.buscar(idC);
                if (ptrC != nullptr) {
                    Canal* actual = *ptrC;
                    Canal* reemplazo = nullptr;
                    if (actual->getTipo() == "publico") {
                        reemplazo = new CanalPrivado(actual->getId(), actual->getNombre(), actual->getCantidadMensajes(), vector<int>{1});
                    } else {
                        reemplazo = new CanalPublico(actual->getId(), actual->getNombre(), actual->getCantidadMensajes());
                    }
                    *ptrC = reemplazo;
                    indiceCanalesNombre.insertar(reemplazo->getNombre(), reemplazo);
                    Canal** nodoEnLista = canales.buscarPtr([idC](Canal* c) { return c->getId() == idC; }, false);
                    if (nodoEnLista != nullptr) *nodoEnLista = reemplazo;
                    delete actual;
                    registrarLog(1, "CAMBIAR_TIPO_CANAL", "Canal #" + to_string(idC) + " cambiado a " + reemplazo->getTipo());
                    cout << "OK: Tipo de canal actualizado a '" << reemplazo->getTipo() << "'.\n";
                } else {
                    cout << "Canal no encontrado.\n";
                }
            } else if (op == 5) {
                string nomBuscar;
                cout << "Nombre exacto del canal (ej. #general-1): ";
                getline(cin, nomBuscar);
                if (!nomBuscar.empty() && nomBuscar[0] != '#') nomBuscar = "#" + nomBuscar;

                Canal** ptrC = indiceCanalesNombre.buscar(nomBuscar);
                if (ptrC != nullptr) {
                    Canal* c = *ptrC;
                    cout << "Encontrado -> [" << c->getId() << "] " << c->getNombre()
                         << " | " << c->getTipo() << " | " << c->getCantidadMensajes() << " mensajes\n";
                } else {
                    cout << "No se encontro ningun canal con el nombre " << nomBuscar << ".\n";
                }
            }
        } while (op != 0);
    }

    // ------------------------------------------------------------------------
    // OPCIÓN 2: MENÚ USUARIOS
    // ------------------------------------------------------------------------
    void menuUsuarios() {
        int op = 0;
        do {
            cout << "\n================ USUARIOS ================\n";
            cout << "1. Listar usuarios\n";
            cout << "2. Registrar nuevo usuario\n";
            cout << "3. Alternar rol (Admin / Miembro)\n";
            cout << "4. Alternar estado (activo / ausente)\n";
            cout << "5. Crear amistad entre dos usuarios (A - B)\n";
            cout << "0. Volver\n";
            cout << "Opcion: ";
            op = leerEnteroSeguro();
            if (!cin) return;
            if (op == -1) continue;

            if (op == 1) {
                auto listaU = usuarios.filtrar([](const Usuario&) { return true; }, -1, false);
                for (const auto& u : listaU) {
                    cout << "[" << u.id << "] " << u.nombre << " <" << u.email << "> (" << u.estado << ", " << u.rol << ")\n";
                }
            } else if (op == 2) {
                string nom, mail, est, rol;
                cout << "Nombre completo: "; getline(cin, nom);
                cout << "Correo: "; getline(cin, mail);
                cout << "Estado inicial (activo/ausente): "; getline(cin, est);
                cout << "Rol inicial (Admin/Miembro): "; getline(cin, rol);

                int id = nextUserId++;
                Usuario nuevo(id, nom, mail, est, rol);
                Usuario* ptrU = usuarios.insertarFinal(nuevo);
                indiceUsuarios.insertar(id, ptrU);
                estados.insertarFinal(Estado(id, id, est, est == "activo" ? ":online:" : ":away:"));
                registrarLog(id, "REGISTRO_USUARIO", "Se registro " + nom);
                cout << "OK: Usuario registrado con ID #" << id << ".\n";
            } else if (op == 3) {
                int idU;
                cout << "ID del usuario: ";
                idU = leerEnteroSeguro();
                Usuario** ptrU = indiceUsuarios.buscar(idU);
                if (ptrU != nullptr) {
                    Usuario* u = *ptrU;
                    u->rol = (u->rol == "Admin") ? "Miembro" : "Admin";
                    registrarLog(idU, "CAMBIO_ROL", "Nuevo rol: " + u->rol);
                    cout << "OK: Rol de " << u->nombre << " cambiado a " << u->rol << ".\n";
                } else {
                    cout << "Usuario no encontrado.\n";
                }
            } else if (op == 4) {
                int idU;
                cout << "ID del usuario: ";
                idU = leerEnteroSeguro();
                Usuario** ptrU = indiceUsuarios.buscar(idU);
                if (ptrU != nullptr) {
                    Usuario* u = *ptrU;
                    u->estado = (u->estado == "activo") ? "ausente" : "activo";
                    estados.insertarFinal(Estado(estados.getTamano() + 1, idU, u->estado, u->estado == "activo" ? ":online:" : ":away:"));
                    registrarLog(idU, "CAMBIO_ESTADO", "Nuevo estado: " + u->estado);
                    cout << "OK: Estado de " << u->nombre << " cambiado a " << u->estado << ".\n";
                } else {
                    cout << "Usuario no encontrado.\n";
                }
            } else if (op == 5) {
                int u1, u2;
                cout << "ID Usuario 1: "; u1 = leerEnteroSeguro();
                cout << "ID Usuario 2: "; u2 = leerEnteroSeguro();
                if (u1 == u2 || indiceUsuarios.buscar(u1) == nullptr || indiceUsuarios.buscar(u2) == nullptr) {
                    cout << "IDs invalidos o iguales.\n";
                } else {
                    auto existe = amistades.buscarPtr([u1, u2](const Amistad& a) {
                        return (a.idUsuario1 == u1 && a.idUsuario2 == u2) || (a.idUsuario1 == u2 && a.idUsuario2 == u1);
                    }, false);
                    if (existe != nullptr) {
                        cout << "La amistad entre #" << u1 << " y #" << u2 << " ya existe.\n";
                    } else {
                        amistades.insertarFinal(Amistad(nextAmistadId++, u1, u2));
                        registrarLog(u1, "NUEVA_AMISTAD", "Amistad entre #" + to_string(u1) + " y #" + to_string(u2));
                        cout << "OK: Amistad creada entre Usuario #" << u1 << " y Usuario #" << u2 << ".\n";
                    }
                }
            }
        } while (op != 0);
    }

    // ------------------------------------------------------------------------
    // OPCIÓN 3: MENÚ MENSAJES E HILOS
    // ------------------------------------------------------------------------
    void menuMensajes() {
        int op = 0;
        do {
            cout << "\n================ MENSAJES ================\n";
            cout << "1. Listar mensajes\n";
            cout << "2. Publicar mensaje (Texto, Archivo o Emoji)\n";
            cout << "3. Editar mensaje existente (Guarda en Pila)\n";
            cout << "4. Ver hilo (Recursivo)\n";
            cout << "0. Volver\n";
            cout << "Opcion: ";
            op = leerEnteroSeguro();
            if (!cin) return;
            if (op == -1) continue;

            if (op == 1) {
                auto ultimos = mensajes.filtrar([](const Mensaje&) { return true; }, -1, true);
                for (int i = (int)ultimos.size() - 1; i >= 0; i--) {
                    const auto& m = ultimos[i];
                    cout << "Msg#" << m.id << " [Canal:" << m.idCanal << ", Autor:" << m.idAutor << "] "
                         << m.fecha << ": " << m.contenido;
                    if (m.idPadre > 0) cout << " (Resp a #" << m.idPadre << ")";
                    cout << "\n";
                }
            } else if (op == 2) {
                int idCanal, idAutor, idPadre, tipoOp;
                cout << "ID del canal: "; idCanal = leerEnteroSeguro();
                cout << "ID del autor: "; idAutor = leerEnteroSeguro();

                Canal** ptrC = indiceCanales.buscar(idCanal);
                if (ptrC == nullptr) {
                    cout << "Canal no encontrado.\n";
                    continue;
                }
                if (indiceUsuarios.buscar(idAutor) == nullptr) {
                    cout << "Usuario autor no encontrado.\n";
                    continue;
                }
                if (!(*ptrC)->puedeEntrar(idAutor)) {
                    cout << "ACCESO DENEGADO: No tienes permiso para publicar en este canal privado.\n";
                    continue;
                }

                cout << "ID mensaje padre (0 si es mensaje nuevo, >0 si es respuesta en hilo): ";
                idPadre = leerEnteroSeguro();
                cout << "Tipo de contenido (1=Texto, 2=Archivo, 3=Emoji): ";
                tipoOp = leerEnteroSeguro();

                ContenidoMensaje* contObj = nullptr;
                if (tipoOp == 2) {
                    string nomArch;
                    cout << "Nombre del archivo (ej. informe.pdf): ";
                    getline(cin, nomArch);
                    contObj = new MensajeArchivo(nomArch);
                } else if (tipoOp == 3) {
                    string nomEmoji;
                    cout << "Nombre del emoji (ej. rocket): ";
                    getline(cin, nomEmoji);
                    contObj = new Emoji(nomEmoji);
                } else {
                    string txt;
                    cout << "Texto del mensaje: ";
                    getline(cin, txt);
                    contObj = new MensajeTexto(txt);
                }

                int idNuevo = nextMensajeId++;
                string contenidoFinal = contObj->generarContenido();
                string tipoFinal = contObj->getTipo();
                delete contObj;

                Mensaje nuevo(idNuevo, idAutor, idCanal, idPadre, tipoFinal, contenidoFinal, obtenerTimestampActual());
                mensajes.insertarFinal(nuevo);
                (*ptrC)->incrementarMensajes();

                if (idPadre > 0) {
                    hilos.insertarFinal(Hilo(nextHiloId++, idPadre));
                    Mensaje* msgPadre = mensajes.buscarPtr([idPadre](const Mensaje& m) { return m.id == idPadre; }, true);
                    if (msgPadre != nullptr && msgPadre->idAutor != idAutor) {
                        notificaciones.encolar(Notificacion(nextNotifId++, msgPadre->idAutor, idNuevo, "respuesta_hilo", false));
                    }
                }

                // Detectar mencion @ID en el texto
                int posArroba = -1;
                for (int i = 0; i < (int)contenidoFinal.length(); i++) {
                    if (contenidoFinal[i] == '@') {
                        posArroba = i;
                        break;
                    }
                }
                if (posArroba != -1 && posArroba + 1 < (int)contenidoFinal.length() &&
                    contenidoFinal[posArroba + 1] >= '0' && contenidoFinal[posArroba + 1] <= '9') {
                    int idMencionado = 0;
                    int k = posArroba + 1;
                    while (k < (int)contenidoFinal.length() &&
                           contenidoFinal[k] >= '0' && contenidoFinal[k] <= '9') {
                        idMencionado = idMencionado * 10 + (contenidoFinal[k] - '0');
                        k++;
                    }
                    notificaciones.encolar(Notificacion(nextNotifId++, idMencionado, idNuevo, "mencion", false));
                } else {
                    notificaciones.encolar(Notificacion(nextNotifId++, idAutor, idNuevo, "mensaje_nuevo", false));
                }

                registrarLog(idAutor, "PUBLICAR_MENSAJE", "Publico Msg#" + to_string(idNuevo) + " en Canal#" + to_string(idCanal));
                cout << "OK: Mensaje publicado con ID #" << idNuevo << " -> " << contenidoFinal << "\n";
            } else if (op == 3) {
                int idM;
                cout << "ID del mensaje a editar: ";
                idM = leerEnteroSeguro();

                Mensaje* m = mensajes.buscarPtr([idM](const Mensaje& x) { return x.id == idM; }, true);
                if (m == nullptr) {
                    cout << "Mensaje no encontrado.\n";
                } else {
                    cout << "Contenido actual: " << m->contenido << "\n";
                    cout << "Nuevo contenido: ";
                    string nuevoTxt;
                    getline(cin, nuevoTxt);

                    ediciones.push(*m);
                    m->contenido = nuevoTxt;
                    m->fecha = obtenerTimestampActual();
                    registrarLog(m->idAutor, "EDITAR_MENSAJE", "Edito Msg#" + to_string(idM));
                    cout << "OK: Mensaje actualizado. Version anterior guardada en la pila de ediciones.\n";
                }
            } else if (op == 4) {
                int idRaiz;
                cout << "ID del mensaje raiz: ";
                idRaiz = leerEnteroSeguro();

                Mensaje* raiz = mensajes.buscarPtr([idRaiz](const Mensaje& m) { return m.id == idRaiz; }, true);
                if (raiz == nullptr) {
                    cout << "Mensaje raiz no encontrado.\n";
                } else {
                    cout << "\n=== HILO DE CONVERSACION RECURSIVO ===\n";
                    cout << "[Raiz #" << raiz->id << "] " << raiz->contenido << "\n";
                    auto msgsCanal = mensajes.filtrar([raiz](const Mensaje& m) {
                        return m.idCanal == raiz->idCanal && m.idPadre > 0;
                    }, -1, false);
                    int totalResp = imprimirRespuestasRec(msgsCanal, idRaiz, 1);
                    cout << "Total respuestas acumuladas: " << totalResp << "\n";
                }
            }
        } while (op != 0);
    }

    // ------------------------------------------------------------------------
    // OPCIÓN 4: HISTORIAL DE EDICIONES (PILA LIFO)
    // ------------------------------------------------------------------------
    void menuEdiciones() {
        int op = 0;
        do {
            cout << "\n================ HISTORIAL DE EDICIONES (PILA) ================\n";
            cout << "Ediciones respaldadas en pila: " << ediciones.getTamano() << "\n";
            cout << "1. Ver pila de ediciones\n";
            cout << "2. Deshacer ultima edicion (Pop y restaurar)\n";
            cout << "3. Ver edicion mas antigua (verFondo)\n";
            cout << "4. Deshacer varias ediciones en lote (deshacerLote)\n";
            cout << "0. Volver\n";
            cout << "Opcion: ";
            op = leerEnteroSeguro();
            if (!cin) return;
            if (op == -1) continue;

            if (op == 1) {
                if (ediciones.estaVacia()) {
                    cout << "No hay ediciones en la pila.\n";
                } else {
                    for (const auto& m : ediciones.listarTodos(-1)) {
                        cout << "Msg#" << m.id << " [Autor:" << m.idAutor << "] (" << m.fecha << ") -> Version previa: " << m.contenido << "\n";
                    }
                }
            } else if (op == 2) {
                if (ediciones.estaVacia()) {
                    cout << "La pila esta vacia.\n";
                } else {
                    Mensaje previa = ediciones.top();
                    ediciones.pop();
                    Mensaje* actual = mensajes.buscarPtr([&previa](const Mensaje& m) { return m.id == previa.id; }, true);
                    if (actual != nullptr) {
                        actual->contenido = previa.contenido;
                        actual->fecha = previa.fecha;
                    }
                    registrarLog(previa.idAutor, "DESHACER_EDICION", "Restauro Msg#" + to_string(previa.id));
                    cout << "OK: Msg#" << previa.id << " restaurado a: \"" << previa.contenido << "\"\n";
                }
            } else if (op == 3) {
                if (ediciones.estaVacia()) cout << "La pila esta vacia.\n";
                else {
                    Mensaje fondo = ediciones.verFondo();
                    cout << "Edicion mas antigua en el fondo: Msg#" << fondo.id << " -> \"" << fondo.contenido << "\"\n";
                }
            } else if (op == 4) {
                if (ediciones.estaVacia()) {
                    cout << "La pila esta vacia.\n";
                } else {
                    int k;
                    cout << "Cantidad de ediciones a deshacer en lote: ";
                    k = leerEnteroSeguro();
                    auto lote = ediciones.deshacerLote(k);
                    for (const auto& previa : lote) {
                        Mensaje* actual = mensajes.buscarPtr([&previa](const Mensaje& m) { return m.id == previa.id; }, true);
                        if (actual != nullptr) {
                            actual->contenido = previa.contenido;
                            actual->fecha = previa.fecha;
                        }
                    }
                    registrarLog(1, "DESHACER_LOTE", "Restauro " + to_string(lote.size()) + " ediciones en lote");
                    cout << "OK: Se restauraron " << lote.size() << " ediciones de la pila.\n";
                }
            }
        } while (op != 0);
    }

    // ------------------------------------------------------------------------
    // OPCIÓN 5: NOTIFICACIONES (COLA FIFO)
    // ------------------------------------------------------------------------
    void menuNotificaciones() {
        int op = 0;
        do {
            cout << "\n================ NOTIFICACIONES ================\n";
            cout << "1. Ver notificaciones\n";
            cout << "2. Marcar como leida la siguiente\n";
            cout << "3. Procesar varias\n";
            cout << "4. Filtrar notificaciones por tipo (filtrarSi)\n";
            cout << "0. Volver\n";
            cout << "Opcion: ";
            op = leerEnteroSeguro();
            if (!cin) return;
            if (op == -1) continue;

            if (op == 1) {
                if (notificaciones.estaVacia()) {
                    cout << "No hay notificaciones pendientes.\n";
                } else {
                    for (const auto& n : notificaciones.listarTodos(-1)) {
                        cout << "  Notificacion#" << n.id << " [Usuario:" << n.idUsuario << "] "
                             << n.tipo << " [" << (n.leida ? "LEIDA" : "PENDIENTE") << "]\n";
                    }
                }
            } else if (op == 2) {
                if (notificaciones.estaVacia()) {
                    cout << "No hay notificaciones pendientes.\n";
                } else {
                    Notificacion n = notificaciones.obtenerFrente();
                    notificaciones.desencolar();
                    cout << "OK: Notificacion#" << n.id << " [Usuario:" << n.idUsuario << "] (" << n.tipo << ") marcada como LEIDA.\n";
                }
            } else if (op == 3) {
                int k;
                cout << "Cantidad de notificaciones a procesar en lote: ";
                k = leerEnteroSeguro();
                int proc = notificaciones.desencolarLote(k);
                cout << "OK: Se procesaron " << proc << " notificaciones de la cola.\n";
            } else if (op == 4) {
                string tipoBuscado;
                cout << "Tipo a filtrar (mencion / mensaje_nuevo / respuesta_hilo / cambio_estado): ";
                getline(cin, tipoBuscado);
                auto filtradas = notificaciones.filtrarSi([&tipoBuscado](const Notificacion& n) {
                    return n.tipo == tipoBuscado;
                });
                cout << "Encontradas " << filtradas.size() << " notificaciones de tipo '" << tipoBuscado << "':\n";
                for (const auto& n : filtradas) {
                    cout << "  Notificacion#" << n.id << " [Usuario:" << n.idUsuario << "] Msg#" << n.idMensaje << "\n";
                }
            }
        } while (op != 0);
    }

    // ------------------------------------------------------------------------
    // OPCIÓN 6: ORDENAR CANALES (QUICKSORT Y TIMSORT)
    // ------------------------------------------------------------------------
    void menuOrdenarCanales() {
        cout << "\n================ ORDENAR CANALES ================\n";
        cout << "1. Por actividad / cantidad de mensajes (QuickSort)\n";
        cout << "2. Alfabeticamente por nombre (TimSort)\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        int op = leerEnteroSeguro();
        if (op <= 0) return;

        vector<Canal*> vec = canales.aVector();
        if (op == 1) {
            if (!vec.empty()) {
                quickSort(vec, 0, (int)vec.size() - 1, [](Canal* a, Canal* b) {
                    return a->getCantidadMensajes() > b->getCantidadMensajes();
                });
            }
            cout << "\n>> Canales ordenados por actividad (QuickSort):\n";
        } else if (op == 2) {
            if (!vec.empty()) {
                timSort(vec, [](Canal* a, Canal* b) {
                    return a->getNombre() < b->getNombre();
                });
            }
            cout << "\n>> Canales ordenados alfabeticamente (TimSort):\n";
        } else {
            cout << "Opcion invalida.\n";
            return;
        }

        for (int i = 0; i < (int)vec.size(); i++) {
            cout << "Canal#" << vec[i]->getId() << " " << vec[i]->getNombre()
                 << " (" << vec[i]->getTipo() << ") - Msgs: " << vec[i]->getCantidadMensajes() << "\n";
        }
    }

    // ------------------------------------------------------------------------
    // OPCIÓN 7: ORDENAR USUARIOS (MERGESORT Y QUICKSORT)
    // ------------------------------------------------------------------------
    void menuOrdenarUsuarios() {
        cout << "\n================ ORDENAR USUARIOS ================\n";
        cout << "1. Alfabeticamente por nombre (MergeSort)\n";
        cout << "2. Por rol y nombre (QuickSort)\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        int op = leerEnteroSeguro();
        if (op <= 0) return;

        vector<Usuario> vec = usuarios.aVector();
        if (op == 1) {
            if (!vec.empty()) {
                mergeSort(vec, 0, (int)vec.size() - 1, [](const Usuario& a, const Usuario& b) {
                    return a.nombre <= b.nombre;
                });
            }
            cout << "\n>> Usuarios ordenados alfabeticamente (MergeSort):\n";
        } else if (op == 2) {
            if (!vec.empty()) {
                quickSort(vec, 0, (int)vec.size() - 1, [](const Usuario& a, const Usuario& b) {
                    if (a.rol != b.rol) return a.rol < b.rol;
                    return a.nombre < b.nombre;
                });
            }
            cout << "\n>> Usuarios ordenados por rol y nombre (QuickSort):\n";
        } else {
            cout << "Opcion invalida.\n";
            return;
        }

        for (int i = 0; i < (int)vec.size(); i++) {
            cout << "[" << vec[i].id << "] " << vec[i].nombre << " <" << vec[i].email
                 << "> (" << vec[i].estado << ", " << vec[i].rol << ")\n";
        }
    }

    // ------------------------------------------------------------------------
    // OPCIÓN 8: ORDENAR MENSAJES (HEAPSORT Y TIMSORT)
    // ------------------------------------------------------------------------
    void menuOrdenarMensajes() {
        cout << "\n================ ORDENAR MENSAJES ================\n";
        cout << "1. Cronologicamente por fecha (HeapSort)\n";
        cout << "2. Por canal y fecha (TimSort)\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        int op = leerEnteroSeguro();
        if (op <= 0) return;

        vector<Mensaje> vec = mensajes.aVector();
        if (op == 1) {
            if (!vec.empty()) {
                heapSort(vec, [](const Mensaje& a, const Mensaje& b) {
                    return a.fecha < b.fecha;
                });
            }
            cout << "\n>> Mensajes ordenados cronologicamente (HeapSort):\n";
        } else if (op == 2) {
            if (!vec.empty()) {
                timSort(vec, [](const Mensaje& a, const Mensaje& b) {
                    if (a.idCanal != b.idCanal) return a.idCanal < b.idCanal;
                    return a.fecha < b.fecha;
                });
            }
            cout << "\n>> Mensajes ordenados por canal y fecha (TimSort):\n";
        } else {
            cout << "Opcion invalida.\n";
            return;
        }

        for (int i = 0; i < (int)vec.size(); i++) {
            cout << "Msg#" << vec[i].id << " [Canal:" << vec[i].idCanal << ", Autor:" << vec[i].idAutor
                 << "] " << vec[i].fecha << ": " << vec[i].contenido << "\n";
        }
    }

    // ------------------------------------------------------------------------
    // OPCIÓN 9 Y 10: PERSISTENCIA EN ARCHIVOS .TXT
    // ------------------------------------------------------------------------
    void guardarDatos() {
        ofstream fU("usuarios.txt"), fC("canales.txt"), fM("mensajes.txt"),
                 fA("amistades.txt"), fAct("actividad.txt"),
                 fE("ediciones.txt"), fN("notificaciones.txt");

        for (const auto& u : usuarios.aVector()) {
            fU << u.id << "|" << u.nombre << "|" << u.email << "|" << u.estado << "|" << u.rol << "\n";
        }

        for (Canal* c : canales.aVector()) {
            fC << c->getId() << "|" << c->getNombre() << "|" << c->getTipo() << "|" << c->getCantidadMensajes() << "|";
            vector<int> accesos = c->getAccesos();
            for (int i = 0; i < (int)accesos.size(); i++) {
                if (i > 0) fC << ",";
                fC << accesos[i];
            }
            fC << "\n";
        }

        for (const auto& m : mensajes.aVector()) {
            fM << m.id << "|" << m.idAutor << "|" << m.idCanal << "|" << m.idPadre << "|"
               << m.tipoContenido << "|" << m.contenido << "|" << m.fecha << "\n";
        }

        for (const auto& am : amistades.aVector()) {
            fA << am.id << "|" << am.idUsuario1 << "|" << am.idUsuario2 << "\n";
        }

        for (const auto& ac : actividad.aVector()) {
            fAct << ac.id << "|" << ac.idUsuario << "|" << ac.fecha << "|" << ac.accion << "|" << ac.detalle << "\n";
        }

        // Guardar pila de ediciones desde el fondo hacia la cima para restaurarla en el mismo orden
        vector<Mensaje> listaEdiciones = ediciones.listarTodos(-1);
        for (int i = (int)listaEdiciones.size() - 1; i >= 0; i--) {
            const auto& e = listaEdiciones[i];
            fE << e.id << "|" << e.idAutor << "|" << e.idCanal << "|" << e.idPadre << "|"
               << e.tipoContenido << "|" << e.contenido << "|" << e.fecha << "\n";
        }

        for (const auto& n : notificaciones.listarTodos(-1)) {
            fN << n.id << "|" << n.idUsuario << "|" << n.idMensaje << "|" << n.tipo << "|" << (n.leida ? 1 : 0) << "\n";
        }

        cout << "\nOK: Datos guardados en usuarios.txt, canales.txt, mensajes.txt, amistades.txt, actividad.txt, ediciones.txt y notificaciones.txt\n";
    }

    void cargarDatos() {
        ifstream fU("usuarios.txt"), fC("canales.txt"), fM("mensajes.txt"),
                 fA("amistades.txt"), fAct("actividad.txt"),
                 fE("ediciones.txt"), fN("notificaciones.txt");
        if (!fU.is_open() || !fC.is_open() || !fM.is_open()) {
            cout << "No se encontraron archivos previos guardados. Use la opcion 9 primero.\n";
            return;
        }

        liberarCanales();
        usuarios.limpiar();
        indiceUsuarios.limpiar();
        mensajes.limpiar();
        hilos.limpiar();
        amistades.limpiar();
        actividad.limpiar();
        ediciones.limpiar();
        notificaciones.limpiar();

        int maxUserId = 0;
        int maxCanalId = 0;
        int maxMensajeId = 0;
        int maxAmistadId = 0;
        int maxActId = 0;
        int maxNotifId = 0;
        nextHiloId = 1;

        string linea;
        while (getline(fU, linea)) {
            if (linea.empty()) continue;
            auto p = separarCadena(linea, '|');
            if (p.size() >= 5) {
                int id = stoi(p[0]);
                Usuario u(id, p[1], p[2], p[3], p[4]);
                Usuario* ptrU = usuarios.insertarFinal(u);
                indiceUsuarios.insertar(id, ptrU);
                if (id > maxUserId) maxUserId = id;
            }
        }

        while (getline(fC, linea)) {
            if (linea.empty()) continue;
            auto p = separarCadena(linea, '|');
            if (p.size() >= 4) {
                int id = stoi(p[0]);
                Canal* c = nullptr;
                if (p[2] == "privado") {
                    vector<int> permitidos;
                    if (p.size() >= 5 && !p[4].empty()) {
                        auto ids = separarCadena(p[4], ',');
                        for (const auto& sId : ids) {
                            if (!sId.empty()) permitidos.push_back(stoi(sId));
                        }
                    }
                    if (permitidos.empty()) permitidos.push_back(1);
                    c = new CanalPrivado(id, p[1], stoi(p[3]), permitidos);
                } else {
                    c = new CanalPublico(id, p[1], stoi(p[3]));
                }
                canales.insertarFinal(c);
                indiceCanales.insertar(id, c);
                indiceCanalesNombre.insertar(c->getNombre(), c);
                if (id > maxCanalId) maxCanalId = id;
            }
        }

        while (getline(fM, linea)) {
            if (linea.empty()) continue;
            auto p = separarCadena(linea, '|');
            if (p.size() >= 7) {
                int id = stoi(p[0]);
                int padre = stoi(p[3]);
                mensajes.insertarFinal(Mensaje(id, stoi(p[1]), stoi(p[2]), padre, p[4], p[5], p[6]));
                if (padre > 0) {
                    hilos.insertarFinal(Hilo(nextHiloId++, padre));
                }
                if (id > maxMensajeId) maxMensajeId = id;
            }
        }

        if (fA.is_open()) {
            while (getline(fA, linea)) {
                if (linea.empty()) continue;
                auto p = separarCadena(linea, '|');
                if (p.size() >= 3) {
                    int id = stoi(p[0]);
                    amistades.insertarFinal(Amistad(id, stoi(p[1]), stoi(p[2])));
                    if (id > maxAmistadId) maxAmistadId = id;
                }
            }
        }

        if (fAct.is_open()) {
            while (getline(fAct, linea)) {
                if (linea.empty()) continue;
                auto p = separarCadena(linea, '|');
                if (p.size() >= 5) {
                    int id = stoi(p[0]);
                    actividad.insertarFinal(RegistroActividad(id, stoi(p[1]), p[2], p[3], p[4]));
                    if (id > maxActId) maxActId = id;
                }
            }
        }

        if (fE.is_open()) {
            while (getline(fE, linea)) {
                if (linea.empty()) continue;
                auto p = separarCadena(linea, '|');
                if (p.size() >= 7) {
                    ediciones.push(Mensaje(stoi(p[0]), stoi(p[1]), stoi(p[2]), stoi(p[3]), p[4], p[5], p[6]));
                }
            }
        }

        if (fN.is_open()) {
            while (getline(fN, linea)) {
                if (linea.empty()) continue;
                auto p = separarCadena(linea, '|');
                if (p.size() >= 5) {
                    int id = stoi(p[0]);
                    notificaciones.encolar(Notificacion(id, stoi(p[1]), stoi(p[2]), p[3], stoi(p[4]) != 0));
                    if (id > maxNotifId) maxNotifId = id;
                }
            }
        }

        nextUserId = maxUserId + 1;
        nextCanalId = maxCanalId + 1;
        nextMensajeId = maxMensajeId + 1;
        nextAmistadId = maxAmistadId + 1;
        nextActId = maxActId + 1;
        nextNotifId = maxNotifId + 1;

        cout << "\nOK: Colecciones reconstruidas desde los archivos .txt.\n";
    }

    // ------------------------------------------------------------------------
    // OPCIÓN 11: RESUMEN GENERAL
    // ------------------------------------------------------------------------
    void resumenGeneral() {
        long total = (long)usuarios.getTamano() +
                     (long)canales.getTamano() +
                     (long)mensajes.getTamano() +
                     (long)notificaciones.getTamano() +
                     (long)ediciones.getTamano();

        auto admins = indiceUsuarios.filtrar([](Usuario* u) { return u->rol == "Admin"; });
        auto privados = indiceCanales.filtrar([](Canal* c) { return c->getTipo() == "privado"; });

        // Uso del metodo propio recursivo de ListaDoble
        int publicosRec = canales.contarSiRecursivo([](Canal* c) {
            return c->getTipo() == "publico";
        }, 2000);
        int respuestasRecientes = mensajes.contarSiRecursivo([](const Mensaje& m) {
            return m.idPadre > 0;
        }, 2000);

        cout << "\n================ ESTADO DEL SISTEMA ================\n";
        cout << "Usuarios registrados: " << usuarios.getTamano() << " (Admins: " << admins.size() << ")\n";
        cout << "Canales activos:      " << canales.getTamano() << " (Publicos: " << publicosRec
             << ", Privados: " << privados.size() << ")\n";
        cout << "Mensajes totales:     " << mensajes.getTamano()
             << " (Respuestas en ultimos 2000: " << respuestasRecientes << ")\n";
        cout << "Ediciones en pila:    " << ediciones.getTamano() << "\n";
        cout << "Notificaciones cola:  " << notificaciones.getTamano() << "\n";
        cout << "Total registros base: " << total << "\n";
        cout << "--- Metricas TablaHash (Funcion Hash Propia) ---\n";
        cout << fixed << setprecision(3);
        cout << "Hash Usuarios (ID)     -> Factor carga: " << indiceUsuarios.obtenerFactorCarga()
             << " | Colisiones: " << indiceUsuarios.contarColisiones() << "\n";
        cout << "Hash Canales (ID)      -> Factor carga: " << indiceCanales.obtenerFactorCarga()
             << " | Colisiones: " << indiceCanales.contarColisiones() << "\n";
        cout << "Hash Canales (Nombre)  -> Factor carga: " << indiceCanalesNombre.obtenerFactorCarga()
             << " | Colisiones: " << indiceCanalesNombre.contarColisiones() << "\n";
    }

    // ------------------------------------------------------------------------
    // OPCIÓN 12: REGISTRO DE ACTIVIDAD
    // ------------------------------------------------------------------------
    void verRegistroActividad() {
        cout << "\n================ REGISTRO DE ACTIVIDAD ================\n";
        auto logs = actividad.filtrar([](const RegistroActividad&) { return true; }, -1, true);
        for (const auto& l : logs) {
            cout << "Log#" << l.id << " [" << l.fecha << "] Usuario:" << l.idUsuario
                 << " -> " << l.accion << ": " << l.detalle << "\n";
        }
    }

    // ------------------------------------------------------------------------
    // MENÚ PRINCIPAL DEL SISTEMA
    // ------------------------------------------------------------------------
    void ejecutarMenuPrincipal() {
        int opcion = 0;
        do {
            cout << "\n==========================================================\n";
            cout << "|                   SLACK - UPC 2026-2                   |\n";
            cout << "==========================================================\n";
            cout << "|  ESPACIO DE TRABAJO                                    |\n";
            cout << "|    1. Canales                                          |\n";
            cout << "|    2. Usuarios                                         |\n";
            cout << "|  ACTIVIDAD                                             |\n";
            cout << "|    3. Mensajes e hilos                                 |\n";
            cout << "|    4. Historial de ediciones                           |\n";
            cout << "|    5. Notificaciones                                   |\n";
            cout << "|  CONSULTAS (ORDENAMIENTOS AVANZADOS)                   |\n";
            cout << "|    6. Ordenar canales (QuickSort / TimSort)            |\n";
            cout << "|    7. Ordenar usuarios (MergeSort / QuickSort)         |\n";
            cout << "|    8. Ordenar mensajes (HeapSort / TimSort)            |\n";
            cout << "|  DATOS                                                 |\n";
            cout << "|    9. Guardar datos                                    |\n";
            cout << "|   10. Cargar datos                                     |\n";
            cout << "|   11. Resumen general                                  |\n";
            cout << "|   12. Registro de actividad                            |\n";
            cout << "|    0. Salir                                            |\n";
            cout << "==========================================================\n";
            cout << "Selecciona una opcion: ";

            opcion = leerEnteroSeguro();
            if (!cin) return;
            if (opcion == -1) continue;

            switch (opcion) {
                case 1: menuCanales(); break;
                case 2: menuUsuarios(); break;
                case 3: menuMensajes(); break;
                case 4: menuEdiciones(); break;
                case 5: menuNotificaciones(); break;
                case 6: menuOrdenarCanales(); break;
                case 7: menuOrdenarUsuarios(); break;
                case 8: menuOrdenarMensajes(); break;
                case 9: guardarDatos(); break;
                case 10: cargarDatos(); break;
                case 11: resumenGeneral(); break;
                case 12: verRegistroActividad(); break;
                case 0: cout << "Saliendo de Slack - UPC 2026-2...\n"; break;
                default: cout << "Opcion invalida.\n"; break;
            }
        } while (opcion != 0);
    }
};

int main() {
    SlackSistema app;
    app.ejecutarMenuPrincipal();
    return 0;
}