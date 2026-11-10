#include "SlackSistema.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <regex>
#include <memory>
using namespace std;

SlackSistema::SlackSistema() : sigMsg(1), sigNotif(1), sigUsr(1), sigCanal(1), sigActividad(1) {
    cargarDemo();
}

void SlackSistema::cargarDemo() {
    roles.insertarFinal(Rol(1, "Admin", "Acceso root"));
    roles.insertarFinal(Rol(2, "Miembro", "Acceso regular"));

    usuarios.insertarFinal(Usuario(1, "John Arias", "john@upc.edu.pe", "activo", "Admin"));
    usuarios.insertarFinal(Usuario(2, "Rodrigo Mallqui", "rodrigo@upc.edu.pe", "activo", "Miembro"));
    usuarios.insertarFinal(Usuario(3, "Abraham Sopla", "abraham@upc.edu.pe", "ausente", "Miembro"));
    sigUsr = 4;

    canales.insertarFinal(make_shared<CanalPublico>(1, "general", 2));
    canales.insertarFinal(make_shared<CanalPublico>(2, "proyecto-tb1", 3));
    canales.insertarFinal(make_shared<CanalPublico>(3, "dudas-algoritmos", 0));
    sigCanal = 4;

    // Mensajes con estructura de hilos
    mensajes.insertarFinal(Mensaje(sigMsg++, 1, 2, "Inicio de coordinacion TB1", "2026-09-20 09:00", 0)); // ID 1
    mensajes.insertarFinal(Mensaje(sigMsg++, 2, 2, "Revisando el diagrama UML", "2026-09-20 09:10", 1));  // ID 2 (Responde a 1)
    mensajes.insertarFinal(Mensaje(sigMsg++, 3, 2, "Listo, subo las clases hoy", "2026-09-20 09:15", 2)); // ID 3 (Responde a 2)
    mensajes.insertarFinal(Mensaje(sigMsg++, 1, 1, "Bienvenidos al ciclo 2026-2", "2026-09-20 08:00", 0)); // ID 4
    mensajes.insertarFinal(Mensaje(sigMsg++, 2, 1, "Gracias, atentos a avisos", "2026-09-20 08:30", 4));   // ID 5 (Responde a 4)

    hilos.insertarFinal(Hilo(1, 1));
    hilos.insertarFinal(Hilo(2, 4));

    notificaciones.encolar(Notificacion(sigNotif++, 2, 1, "mencion"));
    notificaciones.encolar(Notificacion(sigNotif++, 3, 2, "mensaje_nuevo"));
}

// ---------------- METODOS RECURSIVOS ----------------
// Metodo recursivo 2: Recorre todos de mensajes e imprime la jerarquia visual
void SlackSistema::imprimirRespuestasRec(int idPadre, int nivel, const vector<Mensaje>& todosLosMsgs) {
    for (const auto& m : todosLosMsgs) {
        if (m.getIdPadre() == idPadre) {
            for (int i = 0; i < nivel; ++i) cout << "    |--- ";
            cout << "[Respuesta #" << m.getId() << "] Autor " << m.getIdAutor() << ": " << m.getContenido() << "\n";
            imprimirRespuestasRec(m.getId(), nivel + 1, todosLosMsgs);
        }
    }
}

// Metodo recursivo 3: Total de respuestas
int SlackSistema::contarRespuestasTotalesRec(int idPadre, const vector<Mensaje>& todosLosMsgs) {
    int total = 0;
    for (const auto& m : todosLosMsgs) {
        if (m.getIdPadre() == idPadre) {
            total += 1 + contarRespuestasTotalesRec(m.getId(), todosLosMsgs);
        }
    }
    return total;
}

void SlackSistema::verHiloConversacion(int idRaiz) {
    Mensaje* raiz = mensajes.buscar(Mensaje(idRaiz));
    if (!raiz) {
        cout << "Mensaje raiz no encontrado.\n";
        return;
    }
    cout << "\n=== HILO DE CONVERSACION RECURSIVO ===\n";
    cout << "[Raiz #" << raiz->getId() << "] " << raiz->getContenido() << "\n";

    // Lambda 1: Extraccion datos
    vector<Mensaje> buffer;
    mensajes.mostrar([&buffer](const Mensaje& m) { buffer.push_back(m); });

    imprimirRespuestasRec(idRaiz, 1, buffer);
    cout << "Total respuestas acumuladas: " << contarRespuestasTotalesRec(idRaiz, buffer) << "\n";
}

// ---------------- MENUS Y FUNCIONALIDADES ----------------
void SlackSistema::menuUsuarios() {
    int op = -1;
    while (op != 0) {
        cout << "\n================ USUARIOS ================\n1. Listar usuarios\n2. Agregar usuario\n3. Cambiar rol miembro/admin\n4. Cambiar estado activo/ausente\n5. Haz amistad con un usuario\n6. Ver amistades\n0. Volver\nOpcion: ";
        if (!(cin >> op)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Opcion no valida.\n"; continue; }
        cin.ignore(10000, '\n');
        if (op == 1) usuarios.mostrar([](const Usuario& u) { cout << "  " << u << "\n"; });
        else if (op == 2) agregarUsuario();
        else if (op == 5) hacerAmistad();
        else if (op == 6) {
            if (amistades.vacia()) cout << "Todavia no hay amistades registradas.\n";
            amistades.mostrar([](const Amistad& a) { cout << "  " << a << "\n"; });
        }
        else if (op == 3 || op == 4) {
            int id; cout << "ID del usuario: ";
            if (!(cin >> id)) { cin.clear(); cin.ignore(10000, '\n'); cout << "ID no valido.\n"; continue; }
            cin.ignore(10000, '\n');
            Usuario* usuario = usuarios.buscar(Usuario(id));
            if (!usuario) { cout << "No existe un usuario con ese ID.\n"; continue; }
            if (op == 3) {
                usuario->setRol(usuario->getRol() == "Admin" ? "Miembro" : "Admin");
                registrarActividad(id, "Cambio de rol", usuario->getRol());
                cout << "Rol actualizado: " << usuario->getRol() << ".\n";
            } else {
                usuario->setEstado(usuario->getEstado() == "activo" ? "ausente" : "activo");
                notificaciones.encolar(Notificacion(sigNotif++, id, 0, "cambio_estado"));
                registrarActividad(id, "Cambio de estado", usuario->getEstado());
                cout << "Estado actualizado: " << usuario->getEstado() << ".\n";
            }
        } else if (op != 0) cout << "Opcion no valida.\n";
    }
}

void SlackSistema::hacerAmistad() {
    int id1, id2;
    cout << "ID del primer usuario: ";
    if (!(cin >> id1)) { cin.clear(); cin.ignore(10000, '\n'); cout << "ID no valido.\n"; return; }
    cout << "ID del usuario con quien quieres hacer amistad: ";
    if (!(cin >> id2)) { cin.clear(); cin.ignore(10000, '\n'); cout << "ID no valido.\n"; return; }
    cin.ignore(10000, '\n');
    if (id1 == id2) { cout << "No puedes agregar tu propio usuario como amigo.\n"; return; }
    if (!usuarios.buscar(Usuario(id1)) || !usuarios.buscar(Usuario(id2))) { cout << "Ambos IDs deben pertenecer a usuarios registrados.\n"; return; }
    if (amistades.buscar(Amistad(id1, id2))) { cout << "Estos usuarios ya son amigos.\n"; return; }
    amistades.insertarFinal(Amistad(id1, id2));
    registrarActividad(id1, "Nueva amistad", "Usuario " + to_string(id1) + " y usuario " + to_string(id2));
    cout << "Amistad registrada entre los usuarios " << id1 << " y " << id2 << ".\n";
}

void SlackSistema::agregarUsuario() {
    string nombre, apellido, email, estado;
    cout << "Nombre: "; getline(cin, nombre);
    cout << "Apellido: "; getline(cin, apellido);
    cout << "Correo electronico: "; getline(cin, email);
    if (nombre.empty() || apellido.empty() || email.empty()) { cout << "Nombre, apellido y correo son obligatorios.\n"; return; }
    int op; cout << "Estado inicial: 1. Activo  2. Ausente: ";
    if (!(cin >> op)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Opcion no valida.\n"; return; }
    cin.ignore(10000, '\n');
    if (op != 1 && op != 2) { cout << "Opcion no valida.\n"; return; }
    estado = op == 1 ? "activo" : "ausente";
    int idNuevo = sigUsr++;
    usuarios.insertarFinal(Usuario(idNuevo, nombre + " " + apellido, email, estado, "Miembro"));
    registrarActividad(idNuevo, "Usuario agregado", nombre + " " + apellido);
    cout << "Usuario agregado como Miembro. ID asignado: " << idNuevo << ".\n";
}

void SlackSistema::registrarActividad(int idUsuario, const string& accion, const string& detalle) {
    time_t ahora = time(nullptr);
    tm horaLocal;
    localtime_s(&horaLocal, &ahora);
    char fecha[20];
    strftime(fecha, sizeof(fecha), "%Y-%m-%d %H:%M:%S", &horaLocal);
    string detalleLimpio = detalle;
    replace(detalleLimpio.begin(), detalleLimpio.end(), '|', '/');
    actividad.insertarFinal(RegistroActividad(sigActividad++, idUsuario, fecha, accion, detalleLimpio));
}

void SlackSistema::verRegistroActividad() {
    cout << "\n================ REGISTRO DE ACTIVIDAD ================\n";
    if (actividad.vacia()) { cout << "Todavia no hay actividad registrada.\n"; return; }
    actividad.mostrar([](const RegistroActividad& registro) { cout << "  " << registro << "\n"; });
}
void SlackSistema::menuMensajes() {
    int op = -1;
    while (op != 0) {
        cout << "\n================ MENSAJES ================\n1. Listar mensajes\n2. Publicar mensaje\n3. Editar mensaje existente\n4. Ver hilo\n0. Volver\nOpcion: ";
        if (!(cin >> op)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Opcion no valida.\n"; continue; }
        cin.ignore(10000, '\n');
        if (op == 1) mensajes.mostrar([](const Mensaje& m) { cout << "  " << m << "\n"; });
        else if (op == 2) {
            int autor, idC, padre, tipo;
            cout << "ID del usuario que envia: "; if (!(cin >> autor)) { cin.clear(); cin.ignore(10000, '\n'); continue; }
            cout << "ID del canal: "; if (!(cin >> idC)) { cin.clear(); cin.ignore(10000, '\n'); continue; }
            cout << "ID del mensaje al que responde (0 si es nuevo): "; if (!(cin >> padre)) { cin.clear(); cin.ignore(10000, '\n'); continue; }
            if (!usuarios.buscar(Usuario(autor))) { cin.ignore(10000, '\n'); cout << "No existe ese usuario.\n"; continue; }
            shared_ptr<Canal> canal;
            canales.mostrar([&](const shared_ptr<Canal>& c) { if (c->getId() == idC) canal = c; });
            if (!canal) { cin.ignore(10000, '\n'); cout << "No existe ese canal.\n"; continue; }
            if (!canal->puedeEntrar(autor)) { cin.ignore(10000, '\n'); cout << "Ese usuario no tiene acceso al canal privado.\n"; continue; }
            if (padre != 0) {
                Mensaje* original = mensajes.buscar(Mensaje(padre));
                if (!original || original->getIdCanal() != idC) { cin.ignore(10000, '\n'); cout << "El mensaje padre no existe en ese canal.\n"; continue; }
            }
            cout << "Tipo de mensaje: 1. Texto  2. Archivo  3. Emoji: ";
            if (!(cin >> tipo)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Opcion no valida.\n"; continue; }
            cin.ignore(10000, '\n');
            unique_ptr<ContenidoMensaje> contenido;
            string valor;
            if (tipo == 1) {
                cout << "Texto: "; getline(cin, valor);
                contenido.reset(new MensajeTexto(valor));
            } else if (tipo == 2) {
                cout << "Nombre del archivo (ejemplo: TB1.pdf): "; getline(cin, valor);
                contenido.reset(new MensajeArchivo(valor));
            } else if (tipo == 3) {
                cout << "Nombre del emoji sin dos puntos (ejemplo: feliz): "; getline(cin, valor);
                contenido.reset(new Emoji(valor));
            } else { cout << "Tipo de mensaje no valido.\n"; continue; }
            if (valor.empty()) { cout << "El contenido no puede estar vacio.\n"; continue; }

            time_t ahora = time(nullptr); tm horaLocal; localtime_s(&horaLocal, &ahora);
            char fecha[20]; strftime(fecha, sizeof(fecha), "%Y-%m-%d %H:%M:%S", &horaLocal);
            string textoGuardado = contenido->generarContenido();
            Mensaje nuevo(sigMsg++, autor, idC, textoGuardado, fecha, padre);
            mensajes.insertarFinal(nuevo);
            canal->incrementarMensajes();
            usuarios.mostrar([&](const Usuario& u) {
                if (u.getId() != autor && canal->puedeEntrar(u.getId()))
                    notificaciones.encolar(Notificacion(sigNotif++, u.getId(), nuevo.getId(), "mensaje_nuevo"));
            });
            regex mencion("@([0-9]+)");
            for (sregex_iterator it(textoGuardado.begin(), textoGuardado.end(), mencion), fin; it != fin; ++it) {
                int mencionado = stoi((*it)[1].str());
                if (usuarios.buscar(Usuario(mencionado)) && canal->puedeEntrar(mencionado))
                    notificaciones.encolar(Notificacion(sigNotif++, mencionado, nuevo.getId(), "mencion"));
            }
            registrarActividad(autor, "Mensaje publicado", "Mensaje #" + to_string(nuevo.getId()) + " (" + contenido->getTipo() + ") en #" + canal->getNombre());
            cout << "Mensaje publicado como " << contenido->getTipo() << ": " << textoGuardado << "\n";
        }
        else if (op == 3) {
            if (mensajes.vacia()) { cout << "No hay mensajes para editar.\n"; continue; }
            mensajes.mostrar([](const Mensaje& m) { cout << "  ID " << m.getId() << " | canal " << m.getIdCanal() << " | " << m.getFecha() << " | " << m.getContenido() << "\n"; });
            int id; cout << "ID del mensaje que quieres editar: ";
            if (!(cin >> id)) { cin.clear(); cin.ignore(10000, '\n'); continue; }
            cin.ignore(10000, '\n');
            Mensaje* m = mensajes.buscar(Mensaje(id));
            if (!m) { cout << "No existe un mensaje con ese ID.\n"; continue; }
            string texto; cout << "Texto nuevo: "; getline(cin, texto);
            if (texto.empty() || texto == m->getContenido()) { cout << "El texto debe tener contenido y ser distinto al actual.\n"; continue; }
            ediciones.push(*m);
            m->setContenido(texto);
            notificaciones.encolar(Notificacion(sigNotif++, m->getIdAutor(), m->getId(), "mensaje_editado"));
            registrarActividad(m->getIdAutor(), "Mensaje editado", "Mensaje #" + to_string(m->getId()));
            cout << "Mensaje actualizado. La version anterior quedo en el historial de ediciones.\n";
        }
        else if (op == 4) {
            int id; cout << "ID del mensaje raiz: ";
            if (!(cin >> id)) { cin.clear(); cin.ignore(10000, '\n'); continue; }
            cin.ignore(10000, '\n'); verHiloConversacion(id);
        } else if (op != 0) cout << "Opcion no valida.\n";
    }
}
void SlackSistema::menuEdiciones() {
    int op;
    do {
        cout << "\n--- HISTORIAL DE EDICIONES (Pila) ---\n1. Listar historial\n2. Deshacer ultima edicion\n3. Ver edicion mas antigua (Fondo)\n0. Volver\nOpcion: ";
        cin >> op;
        if (op == 1) {
            ediciones.mostrar([](const Mensaje& m) { cout << "  [Historico] " << m << "\n"; });
        }
        else if (op == 2) {
            Mensaje revertido;
            if (ediciones.pop(revertido)) {
                mensajes.modificarSi(
                    [revertido](const Mensaje& m) { return m.getId() == revertido.getId(); },
                    [revertido](Mensaje& m) { m.setContenido(revertido.getContenido()); }
                );
                cout << "Mensaje revertido a: " << revertido.getContenido() << "\n";
            }
            else {
                cout << "Pila vacia.\n";
            }
        }
        else if (op == 3) {
            Mensaje fondo;
            if (ediciones.verFondo(fondo)) {
                cout << "Edicion inicial resguardada: " << fondo << "\n";
            }
            else {
                cout << "Pila vacia.\n";
            }
        }
    } while (op != 0);
}

void SlackSistema::menuNotificaciones() {
    int op = -1;
    while (op != 0) {
        cout << "\n================ NOTIFICACIONES ================\n1. Ver notificaciones\n2. Marcar como leida la siguiente\n3. Procesar varias\n0. Volver\nOpcion: ";
        if (!(cin >> op)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Opcion no valida.\n"; continue; }
        cin.ignore(10000, '\n');
        if (op == 1) {
            if (notificaciones.vacia()) cout << "No tienes notificaciones pendientes.\n";
            notificaciones.mostrar([](const Notificacion& n) { cout << "  " << n << "\n"; });
        } else if (op == 2) {
            Notificacion n;
            if (notificaciones.desencolar(n)) { n.marcarLeida(); cout << "Atendida: " << n << "\n"; }
            else cout << "No hay notificaciones pendientes.\n";
        } else if (op == 3) {
            int cantidad; cout << "Cantidad a procesar: ";
            if (!(cin >> cantidad)) { cin.clear(); cin.ignore(10000, '\n'); continue; }
            cin.ignore(10000, '\n');
            vector<Notificacion> lote;
            int total = notificaciones.desencolarLote(cantidad, lote);
            for (auto& n : lote) n.marcarLeida();
            cout << "Se procesaron " << total << " notificaciones.\n";
        } else if (op != 0) cout << "Opcion no valida.\n";
    }
}
void SlackSistema::menuCanales() {
    int opcion = -1;
    while (opcion != 0) {
        cout << "\n==================================================\n  CANALES\n==================================================\n";
        canales.mostrar([](const shared_ptr<Canal>& c) {
            cout << "  [" << c->getId() << "] #" << c->getNombre() << " | " << c->getTipo() << " | " << c->getCantidadMensajes() << " mensajes\n";
        });
        cout << "\n  1. Abrir canal\n  2. Crear canal\n  3. Agregar usuario a canal privado\n  4. Cambiar tipo publico/privado\n  0. Volver\nElige una opcion: ";
        if (!(cin >> opcion)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Opcion no valida.\n"; continue; }
        cin.ignore(10000, '\n');
        if (opcion == 0) break;
        if (opcion == 2) { crearCanal(); continue; }
        if (opcion == 3) {
            int idC, idU; cout << "ID del canal privado: "; if (!(cin >> idC)) { cin.clear(); cin.ignore(10000, '\n'); continue; }
            cout << "ID del usuario que podra entrar: "; if (!(cin >> idU)) { cin.clear(); cin.ignore(10000, '\n'); continue; } cin.ignore(10000, '\n');
            if (!usuarios.buscar(Usuario(idU))) { cout << "No existe ese usuario.\n"; continue; }
            bool agregado = false;
            canales.mostrar([&](const shared_ptr<Canal>& c) { if (c->getId() == idC && c->getTipo() == "privado") agregado = c->agregarUsuario(idU); });
            if (agregado) { notificaciones.encolar(Notificacion(sigNotif++, idU, idC, "acceso_canal")); registrarActividad(idU, "Acceso a canal privado", "Canal #" + to_string(idC)); }
            cout << (agregado ? "Acceso agregado.\n" : "No se agrego: canal inexistente, publico o usuario ya autorizado.\n"); continue;
        }
        if (opcion == 4) {
            int idC; cout << "ID del canal: "; if (!(cin >> idC)) { cin.clear(); cin.ignore(10000, '\n'); continue; } cin.ignore(10000, '\n');
            shared_ptr<Canal> actual;
            canales.mostrar([&](const shared_ptr<Canal>& c) { if (c->getId() == idC) actual = c; });
            if (!actual) { cout << "No existe ese canal.\n"; continue; }
            if (actual->getTipo() == "publico") {
                int idU; cout << "ID del usuario que tendra acceso al nuevo canal privado: ";
                if (!(cin >> idU)) { cin.clear(); cin.ignore(10000, '\n'); continue; } cin.ignore(10000, '\n');
                if (!usuarios.buscar(Usuario(idU))) { cout << "No existe ese usuario.\n"; continue; }
                vector<int> permitidos{ idU };
                canales.modificarSi([idC](const shared_ptr<Canal>& c) { return c->getId() == idC; }, [&](shared_ptr<Canal>& c) { c = make_shared<CanalPrivado>(idC, c->getNombre(), c->getCantidadMensajes(), permitidos); });
            } else {
                canales.modificarSi([idC](const shared_ptr<Canal>& c) { return c->getId() == idC; }, [](shared_ptr<Canal>& c) { c = make_shared<CanalPublico>(c->getId(), c->getNombre(), c->getCantidadMensajes()); });
            }
            string nuevoTipo = actual->getTipo() == "publico" ? "privado" : "publico";
            registrarActividad(0, "Tipo de canal cambiado", "Canal #" + to_string(idC) + " ahora es " + nuevoTipo);
            cout << "Tipo de canal actualizado.\n"; continue;
        }
        if (opcion != 1) { cout << "Opcion no valida.\n"; continue; }
        int idC; cout << "ID del canal que deseas abrir: ";
        if (!(cin >> idC)) { cin.clear(); cin.ignore(10000, '\n'); continue; } cin.ignore(10000, '\n');
        shared_ptr<Canal> elegido;
        canales.mostrar([&](const shared_ptr<Canal>& c) { if (c->getId() == idC) elegido = c; });
        if (!elegido) { cout << "No existe ese canal.\n"; continue; }
        int idUsuario; cout << "ID del usuario que ingresa: ";
        if (!(cin >> idUsuario)) { cin.clear(); cin.ignore(10000, '\n'); continue; }
        cin.ignore(10000, '\n');
        if (!usuarios.buscar(Usuario(idUsuario))) { cout << "No existe ese usuario.\n"; continue; }
        if (!elegido->puedeEntrar(idUsuario)) { cout << "Acceso denegado: ese usuario no pertenece al canal privado.\n"; continue; }
        vector<Mensaje> lista;
        mensajes.mostrar([&](const Mensaje& m) { if (m.getIdCanal() == idC) lista.push_back(m); });
        stable_sort(lista.begin(), lista.end(), [](const Mensaje& a, const Mensaje& b) { return a.getFecha() == b.getFecha() ? a.getId() < b.getId() : a.getFecha() < b.getFecha(); });
        cout << "\n---------------- #" << elegido->getNombre() << " (" << elegido->getTipo() << ") ----------------\n";
        if (lista.empty()) cout << "Este canal aun no tiene mensajes.\n";
        for (const auto& m : lista) {
            string autor = "Usuario " + to_string(m.getIdAutor());
            usuarios.mostrar([&](const Usuario& u) { if (u.getId() == m.getIdAutor()) autor = u.getNombre(); });
            cout << "\n" << autor << " | " << m.getFecha();
            if (m.getIdPadre()) cout << " | Respuesta a #" << m.getIdPadre();
            cout << "\n  " << m.getContenido() << "\n";
        }
        cout << "\nPresiona Enter para volver a Canales..."; cin.get();
    }
}

void SlackSistema::crearCanal() {
    string nombre; int tipo;
    cout << "Nombre del canal (sin #): "; getline(cin, nombre);
    if (nombre.empty()) { cout << "El nombre no puede estar vacio.\n"; return; }
    cout << "Tipo: 1. Publico  2. Privado: ";
    if (!(cin >> tipo)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Opcion no valida.\n"; return; }
    cin.ignore(10000, '\n');
    if (tipo == 1) { int idNuevo = sigCanal++; canales.insertarFinal(make_shared<CanalPublico>(idNuevo, nombre)); registrarActividad(0, "Canal publico creado", nombre); }
    else if (tipo == 2) {
        int idU; cout << "ID del usuario que tendra acceso: ";
        if (!(cin >> idU)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Opcion no valida.\n"; return; }
        cin.ignore(10000, '\n');
        if (!usuarios.buscar(Usuario(idU))) { cout << "No existe ese usuario.\n"; return; }
        int idNuevo = sigCanal++;
        canales.insertarFinal(make_shared<CanalPrivado>(idNuevo, nombre, 0, vector<int>{idU}));
        notificaciones.encolar(Notificacion(sigNotif++, idU, idNuevo, "invitacion_canal"));
        registrarActividad(idU, "Canal privado creado", nombre);
    } else { cout << "Tipo de canal no valido.\n"; return; }
    cout << "Canal creado con exito.\n";
}
void SlackSistema::ordenarCanalesPorActividad() {
    vector<shared_ptr<Canal>> v;
    canales.mostrar([&v](const shared_ptr<Canal>& c) { v.push_back(c); });
    if (v.empty()) return;
    auto cmpDesc = [](const shared_ptr<Canal>& a, const shared_ptr<Canal>& b) { return a->getCantidadMensajes() > b->getCantidadMensajes(); };
    quickSort(v, 0, static_cast<int>(v.size()) - 1, cmpDesc);
    cout << "\n>> Canales ordenados por actividad:\n";
    for (const auto& c : v) cout << "  " << *c << "\n";
}
void SlackSistema::ordenarUsuariosPorNombre() {
    vector<Usuario> v;
    usuarios.mostrar([&v](const Usuario& u) { v.push_back(u); });
    if (v.empty()) return;

    // Lambda 7 (In3): Comparador MergeSort alfabetico
    auto cmpNom = [](const Usuario& a, const Usuario& b) {
        return a.getNombre() < b.getNombre();
        };
    mergeSort(v, 0, static_cast<int>(v.size()) - 1, cmpNom);

    cout << "\n>> Usuarios ordenados alfabeticamente (MergeSort):\n";
    for (const auto& u : v) cout << "  " << u << "\n";
}

void SlackSistema::ordenarMensajesPorFecha() {
    vector<Mensaje> v;
    mensajes.mostrar([&v](const Mensaje& m) { v.push_back(m); });
    if (v.empty()) return;

    // Lambda 8 (In3): Comparador HeapSort por fecha
    auto cmpFecha = [](const Mensaje& a, const Mensaje& b) {
        return a.getFecha() < b.getFecha();
        };
    heapSort(v, cmpFecha);

    cout << "\n>> Mensajes ordenados cronologicamente (HeapSort):\n";
    for (const auto& m : v) cout << "  " << m << "\n";
}

// ---------------- PERSISTENCIA DE ARCHIVOS ----------------
void SlackSistema::guardarDatos() {
    // 1. Usuarios
    ofstream fu("usuarios.txt");
    if (fu) {
        // Lambda 9 (In3): Escritura formateada
        usuarios.mostrar([&fu](const Usuario& u) {
            fu << u.getId() << "|" << u.getNombre() << "|" << u.getEmail() << "|" << u.getEstado() << "|" << u.getRol() << "\n";
            });
        fu.close();
    }

    // 2. Canales
    ofstream fc("canales.txt");
    if (fc) {
        canales.mostrar([&fc](const shared_ptr<Canal>& c) {
            fc << c->getId() << "|" << c->getNombre() << "|" << c->getTipo() << "|" << c->getCantidadMensajes() << "|" << c->getAccesos() << "\n";
        });
        fc.close();
    }
    // 3. Amistades
    ofstream fam("amistades.txt");
    if (fam) {
        amistades.mostrar([&fam](const Amistad& a) { fam << a.getIdUsuario1() << "|" << a.getIdUsuario2() << "\n"; });
        fam.close();
    }

    // 4. Registro de actividad
    ofstream fact("actividad.txt");
    if (fact) {
        actividad.mostrar([&fact](const RegistroActividad& r) {
            fact << r.getId() << "|" << r.getIdUsuario() << "|" << r.getFecha() << "|" << r.getAccion() << "|" << r.getDetalle() << "\n";
        });
        fact.close();
    }

    // 5. Mensajes
    ofstream fm("mensajes.txt");
    if (fm) {
        mensajes.mostrar([&fm](const Mensaje& m) {
            fm << m.getId() << "|" << m.getIdAutor() << "|" << m.getIdCanal() << "|" << m.getIdPadre() << "|" << m.getFecha() << "|" << m.getContenido() << "\n";
            });
        fm.close();
    }
    cout << "OK: Datos guardados en usuarios.txt, canales.txt, mensajes.txt, amistades.txt y actividad.txt\n";
}

void SlackSistema::cargarDatos() {
    ifstream fu("usuarios.txt");
    if (fu) {
        usuarios.vaciar();
        string l;
        while (getline(fu, l)) {
            stringstream ss(l);
            string id, n, e, es, r;
            if (getline(ss, id, '|') && getline(ss, n, '|') && getline(ss, e, '|') && getline(ss, es, '|') && getline(ss, r)) {
                int uid = stoi(id);
                usuarios.insertarFinal(Usuario(uid, n, e, es, r));
                if (uid >= sigUsr) sigUsr = uid + 1;
            }
        }
        fu.close();
    }

    ifstream fam("amistades.txt");
    if (fam) {
        amistades.vaciar();
        int id1, id2; char separador;
        while (fam >> id1 >> separador >> id2) {
            if (usuarios.buscar(Usuario(id1)) && usuarios.buscar(Usuario(id2))) amistades.insertarFinal(Amistad(id1, id2));
        }
    }

    ifstream fact("actividad.txt");
    if (fact) {
        actividad.vaciar();
        string linea;
        while (getline(fact, linea)) {
            stringstream ss(linea);
            string id, usuario, fecha, accion, detalle;
            if (getline(ss, id, '|') && getline(ss, usuario, '|') && getline(ss, fecha, '|') && getline(ss, accion, '|') && getline(ss, detalle)) {
                int idRegistro = stoi(id);
                actividad.insertarFinal(RegistroActividad(idRegistro, stoi(usuario), fecha, accion, detalle));
                if (idRegistro >= sigActividad) sigActividad = idRegistro + 1;
            }
        }
    }
    ifstream fc("canales.txt");
    if (fc) {
        canales.vaciar();
        string l;
        while (getline(fc, l)) {
            stringstream ss(l);
            string id, n, t, cm, accesos;
            if (getline(ss, id, '|') && getline(ss, n, '|') && getline(ss, t, '|') && getline(ss, cm, '|')) {
                getline(ss, accesos);
                int cid = stoi(id), cantidad = stoi(cm);
                if (t == "privado") {
                    vector<int> permitidos; stringstream ids(accesos); string token;
                    while (getline(ids, token, ',')) if (!token.empty()) permitidos.push_back(stoi(token));
                    canales.insertarFinal(make_shared<CanalPrivado>(cid, n, cantidad, permitidos));
                } else canales.insertarFinal(make_shared<CanalPublico>(cid, n, cantidad));
                if (cid >= sigCanal) sigCanal = cid + 1;
            }
        }
        fc.close();
    }
    ifstream fm("mensajes.txt");
    if (fm) {
        mensajes.vaciar();
        string l;
        while (getline(fm, l)) {
            stringstream ss(l);
            string id, a, c, p, fe, co;
            if (getline(ss, id, '|') && getline(ss, a, '|') && getline(ss, c, '|') && getline(ss, p, '|') && getline(ss, fe, '|') && getline(ss, co)) {
                int mid = stoi(id);
                mensajes.insertarFinal(Mensaje(mid, stoi(a), stoi(c), co, fe, stoi(p)));
                if (mid >= sigMsg) sigMsg = mid + 1;
            }
        }
        fm.close();
    }
    cout << "OK: Registros cargados desde el almacenamiento.\n";
}

void SlackSistema::verResumen() {
    cout << "\n================ ESTADO DEL SISTEMA ================\n";
    cout << "Usuarios registrados: " << usuarios.size() << "\n";
    cout << "Canales activos:      " << canales.size() << "\n";
    cout << "Mensajes totales:     " << mensajes.size() << "\n";
    cout << "Ediciones en pila:    " << ediciones.size() << "\n";
    cout << "Notificaciones cola:  " << notificaciones.size() << "\n";
    cout << "====================================================\n";
}