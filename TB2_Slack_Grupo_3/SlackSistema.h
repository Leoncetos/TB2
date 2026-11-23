#pragma once

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

// Clases
#include "Amistad.h"
#include "Canal.h"
#include "CanalPrivado.h"
#include "CanalPublico.h"
#include "ContenidoMensaje.h"
#include "Emoji.h"
#include "Estado.h"
#include "Hilo.h"
#include "Mensaje.h"
#include "MensajeTexto.h"
#include "MensajeArchivo.h"
#include "notificacion.h"
#include "RegistroActividad.h"
#include "Rol.h"
#include "Usuario.h"

// Estructuras
#include "FuncionesHash.h"
#include "TablaHash.h"
#include "Utilidades.h"
#include "ListaDoble.h"
#include "Cola.h"
#include "Pila.h"

//Ordenamientos avanzados
#include "QuickSort.h"
#include "HeapSort.h"

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

    int nextUserId, nextCanalId, nextMensajeId, nextHiloId, nextNotifId, nextActId, nextAmistadId;

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

    // Generador del dataset para las 150k datos
    string generarContenidoAleatorio(int idMensaje, int nUsuarios, string& tipoOut) {
        const char* verbos[] = {
            "Revisando", "Subiendo", "Validando", "Corrigiendo", "Compilando", "Probando",
            "Optimizando", "Documentando", "Analizando", "Desplegando", "Integrando", "Verificando",
            "Actualizando", "Depurando", "Coordinando", "Compartiendo", "Exportando", "Consultando",
            "Codificando", "Produciendo", "Implementando", "Ejecutando", "Recopilando"
        };
        const char* sustantivos[] = {
            "el diagrama UML", "la lista doble", "el algoritmo QuickSort", "la pila de ediciones",
            "la cola de notificaciones", "el historial en txt", "los canales privados", "el hilo recursivo",
            "el modulo de usuarios", "la tabla hash", "el reporte de complejidad", "los permisos de acceso",
            "la interfaz en consola", "el commit final", "las pruebas unitarias", "el parser de archivos",
            "el ordenamiento MergeSort", "el monticulo HeapSort", "la estructura de datos", "el sprint actual",
            "las historias de usuario", "las Funcionales", "las Epicas de la aplicacion", "el feedback de la reunion"
        };
        const char* complementos[] = {
            "para la entrega de hoy", "sin errores de memoria", "segun lo acordado en reunion",
            "antes del cierre del bloque", "con datos de prueba", "en el entorno local",
            "con complejidad logaritmica", "para revisar en equipo", "listo para validar",
            "con todos los casos borde", "en la rama principal", "con los filtros lambda",
            "verificado en consola", "pendiente de aprobacion", "con formato correcto",
            "para la reunion", "en la aplicacion", "con las Epicas", "del sprint actual"
        };
        const char* archivos[] = {
            "diagrama_uml.png", "informe_tb2.pdf", "main.cpp", "estructuras.h",
            "capturas_consola.zip", "analisis_bigo.docx", "dataset_prueba.txt", "notas_reunion.md",
            "informe_tb1.pdf", "algoritmos_avanzado.h", "funcionalidades.cpp", "tb2.zip"
        };
        const char* emojis[] = {
            "rocket", "white_check_mark", "eyes", "thumbsup", "fire", "computer", "clap", "bulb", "kisses",
            "tree", "water", "heart", "monkey", "joy", "happy", "sad", "crying"
        };

        int modo = rand() % 10;
        if (modo == 0) {
            tipoOut = "archivo";
            return string("[") + archivos[rand() % 8] + "]";
        }
        else if (modo == 1) {
            tipoOut = "emoji";
            return string(":") + emojis[rand() % 8] + ":";
        }
        else {
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
            }
            else if (linea[i] != '\r') {
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
                vector<int> permitidos = { 1, 2, 3, aleatorioRango(N_USR) + 1 };
                c = new CanalPrivado(i, nom, conteoPorCanal[i], permitidos);
            }
            else {
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
            //notificaciones.encolar(n);
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

                /*if (!msgsCanal.empty()) {
                    mergeSort(msgsCanal, 0, (int)msgsCanal.size() - 1, [](const Mensaje& a, const Mensaje& b) {
                        if (a.fecha != b.fecha) return a.fecha < b.fecha;
                        return a.id < b.id;
                        });
                }*/

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
            }
            else if (op == 2) {
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
                }
                else {
                    nuevo = new CanalPublico(idNuevo, nom, 0);
                }
                canales.insertarFinal(nuevo);
                indiceCanales.insertar(idNuevo, nuevo);
                indiceCanalesNombre.insertar(nom, nuevo);
                registrarLog(1, "CREAR_CANAL", "Creo canal " + nom + " (" + nuevo->getTipo() + ")");
                cout << "OK: Canal " << nom << " creado con ID #" << idNuevo << ".\n";
            }
            else if (op == 3) {
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
                    }
                    else {
                        cout << "El usuario ya tenia acceso.\n";
                    }
                }
                else {
                    cout << "El canal no existe o no es privado.\n";
                }
            }
            else if (op == 4) {
                int idC;
                cout << "ID del canal a cambiar tipo: ";
                idC = leerEnteroSeguro();

                Canal** ptrC = indiceCanales.buscar(idC);
                if (ptrC != nullptr) {
                    Canal* actual = *ptrC;
                    Canal* reemplazo = nullptr;
                    if (actual->getTipo() == "publico") {
                        reemplazo = new CanalPrivado(actual->getId(), actual->getNombre(), actual->getCantidadMensajes(), vector<int>{1});
                    }
                    else {
                        reemplazo = new CanalPublico(actual->getId(), actual->getNombre(), actual->getCantidadMensajes());
                    }
                    *ptrC = reemplazo;
                    indiceCanalesNombre.insertar(reemplazo->getNombre(), reemplazo);
                    Canal** nodoEnLista = canales.buscarPtr([idC](Canal* c) { return c->getId() == idC; }, false);
                    if (nodoEnLista != nullptr) *nodoEnLista = reemplazo;
                    delete actual;
                    registrarLog(1, "CAMBIAR_TIPO_CANAL", "Canal #" + to_string(idC) + " cambiado a " + reemplazo->getTipo());
                    cout << "OK: Tipo de canal actualizado a '" << reemplazo->getTipo() << "'.\n";
                }
                else {
                    cout << "Canal no encontrado.\n";
                }
            }
            else if (op == 5) {
                string nomBuscar;
                cout << "Nombre exacto del canal (ej. #general-1): ";
                getline(cin, nomBuscar);
                if (!nomBuscar.empty() && nomBuscar[0] != '#') nomBuscar = "#" + nomBuscar;

                Canal** ptrC = indiceCanalesNombre.buscar(nomBuscar);
                if (ptrC != nullptr) {
                    Canal* c = *ptrC;
                    cout << "Encontrado -> [" << c->getId() << "] " << c->getNombre()
                        << " | " << c->getTipo() << " | " << c->getCantidadMensajes() << " mensajes\n";
                }
                else {
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
            }
            else if (op == 2) {
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
            }
            else if (op == 3) {
                int idU;
                cout << "ID del usuario: ";
                idU = leerEnteroSeguro();
                Usuario** ptrU = indiceUsuarios.buscar(idU);
                if (ptrU != nullptr) {
                    Usuario* u = *ptrU;
                    u->rol = (u->rol == "Admin") ? "Miembro" : "Admin";
                    registrarLog(idU, "CAMBIO_ROL", "Nuevo rol: " + u->rol);
                    cout << "OK: Rol de " << u->nombre << " cambiado a " << u->rol << ".\n";
                }
                else {
                    cout << "Usuario no encontrado.\n";
                }
            }
            else if (op == 4) {
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
                }
                else {
                    cout << "Usuario no encontrado.\n";
                }
            }
            else if (op == 5) {
                int u1, u2;
                cout << "ID Usuario 1: "; u1 = leerEnteroSeguro();
                cout << "ID Usuario 2: "; u2 = leerEnteroSeguro();
                if (u1 == u2 || indiceUsuarios.buscar(u1) == nullptr || indiceUsuarios.buscar(u2) == nullptr) {
                    cout << "IDs invalidos o iguales.\n";
                }
                else {
                    auto existe = amistades.buscarPtr([u1, u2](const Amistad& a) {
                        return (a.idUsuario1 == u1 && a.idUsuario2 == u2) || (a.idUsuario1 == u2 && a.idUsuario2 == u1);
                        }, false);
                    if (existe != nullptr) {
                        cout << "La amistad entre #" << u1 << " y #" << u2 << " ya existe.\n";
                    }
                    else {
                        amistades.insertarFinal(Amistad(nextAmistadId++, u1, u2));
                        registrarLog(u1, "NUEVA_AMISTAD", "Amistad entre #" + to_string(u1) + " y #" + to_string(u2));
                        cout << "OK: Amistad creada entre Usuario #" << u1 << " y Usuario #" << u2 << ".\n";
                    }
                }
            }
        } while (op != 0);
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
            if (opcion == -1) {
                cout << "Entrada invalida. Ingresa un numero.\n";
                continue;
            }

            switch (opcion) {
            case 1: menuCanales(); break;
            case 2: menuUsuarios(); break;
            case 3: /*Mensajes e hilos*/ break;
            case 4: /*Historial de ediciones*/ break;
            case 5: /*Notificaciones*/ break;
            case 6: /*Ord canales*/ break;
            case 7: /*Ord usuarios*/ break;
            case 8: /*Ord mensajes*/ break;
            case 9: /*Guardar datos*/ break;
            case 10: /*Cargar datos*/ break;
            case 11: resumenGeneral(); break;
            case 12: verRegistroActividad(); break;
            case 0: cout << "Saliendo de Slack - UPC 2026-2...\n"; break;
            default: cout << "Opcion invalida.\n"; break;
            }
        } while (opcion != 0);
    }
};