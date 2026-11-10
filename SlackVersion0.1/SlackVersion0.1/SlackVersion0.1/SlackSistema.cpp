#include "SlackSistema.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <regex>
#include <memory>

SlackSistema::SlackSistema() : sigMsg(1), sigNotif(1), sigUsr(1), sigCanal(1), sigActividad(1) {
    cargarDemo();
}

void SlackSistema::cargarDemo() {
    roles.insertarFinal(Rol(1, "Admin", "Acceso root"));
    roles.insertarFinal(Rol(2, "Miembro", "Acceso regular"));

    usuarios.insertarFinal(Usuario(1, "John Arias", "john@upc.edu.pe", "activo", "Admin"));
    usuarios.insertarFinal(Usuario(2, "Rodrigo Mallqui", "rodrigo@utec.edu.pe", "activo", "Miembro"));
    usuarios.insertarFinal(Usuario(3, "Abraham Sopla", "abraham@utp.edu.pe", "ausente", "Miembro"));
    sigUsr = 4;

    canales.insertarFinal(std::make_shared<CanalPublico>(1, "general", 2));
    canales.insertarFinal(std::make_shared<CanalPublico>(2, "proyecto-tb1", 3));
    canales.insertarFinal(std::make_shared<CanalPublico>(3, "dudas-algoritmos", 0));
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

// ---------------- MÉTODOS RECURSIVOS ----------------
// Método recursivo 2 (Integrante 2): Recorre el árbol de mensajes e imprime la jerarquía visual
void SlackSistema::imprimirRespuestasRec(int idPadre, int nivel, const std::vector<Mensaje>& todosLosMsgs) {
    for (const auto& m : todosLosMsgs) {
        if (m.getIdPadre() == idPadre) {
            for (int i = 0; i < nivel; ++i) std::cout << "    |--- ";
            std::cout << "[Respuesta #" << m.getId() << "] Autor " << m.getIdAutor() << ": " << m.getContenido() << "\n";
            imprimirRespuestasRec(m.getId(), nivel + 1, todosLosMsgs);
        }
    }
}

// Método recursivo 3 (Integrante 3): Calcula el total acumulado de respuestas hijas y sub-hijas
int SlackSistema::contarRespuestasTotalesRec(int idPadre, const std::vector<Mensaje>& todosLosMsgs) {
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
        std::cout << "Mensaje raiz no encontrado.\n";
        return;
    }
    std::cout << "\n=== HILO DE CONVERSACION RECURSIVO ===\n";
    std::cout << "[Raiz #" << raiz->getId() << "] " << raiz->getContenido() << "\n";

    // Lambda 1 (Integrante 1): Extracción de datos para procesamiento
    std::vector<Mensaje> buffer;
    mensajes.mostrar([&buffer](const Mensaje& m) { buffer.push_back(m); });

    imprimirRespuestasRec(idRaiz, 1, buffer);
    std::cout << "Total respuestas acumuladas: " << contarRespuestasTotalesRec(idRaiz, buffer) << "\n";
}

// ---------------- MENÚS Y FUNCIONALIDADES ----------------
void SlackSistema::menuUsuarios() {
    int op = -1;
    while (op != 0) {
        std::cout << "\n================ USUARIOS ================\n1. Listar usuarios\n2. Agregar usuario\n3. Cambiar rol miembro/admin\n4. Cambiar estado activo/ausente\n5. Haz amistad con un usuario\n6. Ver amistades\n0. Volver\nOpcion: ";
        if (!(std::cin >> op)) { std::cin.clear(); std::cin.ignore(10000, '\n'); std::cout << "Opcion no valida.\n"; continue; }
        std::cin.ignore(10000, '\n');
        if (op == 1) usuarios.mostrar([](const Usuario& u) { std::cout << "  " << u << "\n"; });
        else if (op == 2) agregarUsuario();
        else if (op == 5) hacerAmistad();
        else if (op == 6) {
            if (amistades.vacia()) std::cout << "Todavia no hay amistades registradas.\n";
            amistades.mostrar([](const Amistad& a) { std::cout << "  " << a << "\n"; });
        }
        else if (op == 3 || op == 4) {
            int id; std::cout << "ID del usuario: ";
            if (!(std::cin >> id)) { std::cin.clear(); std::cin.ignore(10000, '\n'); std::cout << "ID no valido.\n"; continue; }
            std::cin.ignore(10000, '\n');
            Usuario* usuario = usuarios.buscar(Usuario(id));
            if (!usuario) { std::cout << "No existe un usuario con ese ID.\n"; continue; }
            if (op == 3) {
                usuario->setRol(usuario->getRol() == "Admin" ? "Miembro" : "Admin");
                registrarActividad(id, "Cambio de rol", usuario->getRol());
                std::cout << "Rol actualizado: " << usuario->getRol() << ".\n";
            } else {
                usuario->setEstado(usuario->getEstado() == "activo" ? "ausente" : "activo");
                notificaciones.encolar(Notificacion(sigNotif++, id, 0, "cambio_estado"));
                registrarActividad(id, "Cambio de estado", usuario->getEstado());
                std::cout << "Estado actualizado: " << usuario->getEstado() << ".\n";
            }
        } else if (op != 0) std::cout << "Opcion no valida.\n";
    }
}

void SlackSistema::hacerAmistad() {
    int id1, id2;
    std::cout << "ID del primer usuario: ";
    if (!(std::cin >> id1)) { std::cin.clear(); std::cin.ignore(10000, '\n'); std::cout << "ID no valido.\n"; return; }
    std::cout << "ID del usuario con quien quieres hacer amistad: ";
    if (!(std::cin >> id2)) { std::cin.clear(); std::cin.ignore(10000, '\n'); std::cout << "ID no valido.\n"; return; }
    std::cin.ignore(10000, '\n');
    if (id1 == id2) { std::cout << "No puedes agregar tu propio usuario como amigo.\n"; return; }
    if (!usuarios.buscar(Usuario(id1)) || !usuarios.buscar(Usuario(id2))) { std::cout << "Ambos IDs deben pertenecer a usuarios registrados.\n"; return; }
    if (amistades.buscar(Amistad(id1, id2))) { std::cout << "Estos usuarios ya son amigos.\n"; return; }
    amistades.insertarFinal(Amistad(id1, id2));
    registrarActividad(id1, "Nueva amistad", "Usuario " + std::to_string(id1) + " y usuario " + std::to_string(id2));
    std::cout << "Amistad registrada entre los usuarios " << id1 << " y " << id2 << ".\n";
}

void SlackSistema::agregarUsuario() {
    std::string nombre, apellido, email, estado;
    std::cout << "Nombre: "; std::getline(std::cin, nombre);
    std::cout << "Apellido: "; std::getline(std::cin, apellido);
    std::cout << "Correo electronico: "; std::getline(std::cin, email);
    if (nombre.empty() || apellido.empty() || email.empty()) { std::cout << "Nombre, apellido y correo son obligatorios.\n"; return; }
    int op; std::cout << "Estado inicial: 1. Activo  2. Ausente: ";
    if (!(std::cin >> op)) { std::cin.clear(); std::cin.ignore(10000, '\n'); std::cout << "Opcion no valida.\n"; return; }
    std::cin.ignore(10000, '\n');
    if (op != 1 && op != 2) { std::cout << "Opcion no valida.\n"; return; }
    estado = op == 1 ? "activo" : "ausente";
    int idNuevo = sigUsr++;
    usuarios.insertarFinal(Usuario(idNuevo, nombre + " " + apellido, email, estado, "Miembro"));
    registrarActividad(idNuevo, "Usuario agregado", nombre + " " + apellido);
    std::cout << "Usuario agregado como Miembro. ID asignado: " << idNuevo << ".\n";
}

void SlackSistema::registrarActividad(int idUsuario, const std::string& accion, const std::string& detalle) {
    std::time_t ahora = std::time(nullptr);
    std::tm horaLocal;
    localtime_s(&horaLocal, &ahora);
    char fecha[20];
    std::strftime(fecha, sizeof(fecha), "%Y-%m-%d %H:%M:%S", &horaLocal);
    std::string detalleLimpio = detalle;
    std::replace(detalleLimpio.begin(), detalleLimpio.end(), '|', '/');
    actividad.insertarFinal(RegistroActividad(sigActividad++, idUsuario, fecha, accion, detalleLimpio));
}

void SlackSistema::verRegistroActividad() {
    std::cout << "\n================ REGISTRO DE ACTIVIDAD ================\n";
    if (actividad.vacia()) { std::cout << "Todavia no hay actividad registrada.\n"; return; }
    actividad.mostrar([](const RegistroActividad& registro) { std::cout << "  " << registro << "\n"; });
}
void SlackSistema::menuMensajes() {
    int op = -1;
    while (op != 0) {
        std::cout << "\n================ MENSAJES ================\n1. Listar mensajes\n2. Publicar mensaje\n3. Editar mensaje existente\n4. Ver hilo\n0. Volver\nOpcion: ";
        if (!(std::cin >> op)) { std::cin.clear(); std::cin.ignore(10000, '\n'); std::cout << "Opcion no valida.\n"; continue; }
        std::cin.ignore(10000, '\n');
        if (op == 1) mensajes.mostrar([](const Mensaje& m) { std::cout << "  " << m << "\n"; });
        else if (op == 2) {
            int autor, idC, padre, tipo;
            std::cout << "ID del usuario que envia: "; if (!(std::cin >> autor)) { std::cin.clear(); std::cin.ignore(10000, '\n'); continue; }
            std::cout << "ID del canal: "; if (!(std::cin >> idC)) { std::cin.clear(); std::cin.ignore(10000, '\n'); continue; }
            std::cout << "ID del mensaje al que responde (0 si es nuevo): "; if (!(std::cin >> padre)) { std::cin.clear(); std::cin.ignore(10000, '\n'); continue; }
            if (!usuarios.buscar(Usuario(autor))) { std::cin.ignore(10000, '\n'); std::cout << "No existe ese usuario.\n"; continue; }
            std::shared_ptr<Canal> canal;
            canales.mostrar([&](const std::shared_ptr<Canal>& c) { if (c->getId() == idC) canal = c; });
            if (!canal) { std::cin.ignore(10000, '\n'); std::cout << "No existe ese canal.\n"; continue; }
            if (!canal->puedeEntrar(autor)) { std::cin.ignore(10000, '\n'); std::cout << "Ese usuario no tiene acceso al canal privado.\n"; continue; }
            if (padre != 0) {
                Mensaje* original = mensajes.buscar(Mensaje(padre));
                if (!original || original->getIdCanal() != idC) { std::cin.ignore(10000, '\n'); std::cout << "El mensaje padre no existe en ese canal.\n"; continue; }
            }
            std::cout << "Tipo de mensaje: 1. Texto  2. Archivo  3. Emoji: ";
            if (!(std::cin >> tipo)) { std::cin.clear(); std::cin.ignore(10000, '\n'); std::cout << "Opcion no valida.\n"; continue; }
            std::cin.ignore(10000, '\n');
            std::unique_ptr<ContenidoMensaje> contenido;
            std::string valor;
            if (tipo == 1) {
                std::cout << "Texto: "; std::getline(std::cin, valor);
                contenido.reset(new MensajeTexto(valor));
            } else if (tipo == 2) {
                std::cout << "Nombre del archivo (ejemplo: TB1.pdf): "; std::getline(std::cin, valor);
                contenido.reset(new MensajeArchivo(valor));
            } else if (tipo == 3) {
                std::cout << "Nombre del emoji sin dos puntos (ejemplo: feliz): "; std::getline(std::cin, valor);
                contenido.reset(new Emoji(valor));
            } else { std::cout << "Tipo de mensaje no valido.\n"; continue; }
            if (valor.empty()) { std::cout << "El contenido no puede estar vacio.\n"; continue; }

            std::time_t ahora = std::time(nullptr); std::tm horaLocal; localtime_s(&horaLocal, &ahora);
            char fecha[20]; std::strftime(fecha, sizeof(fecha), "%Y-%m-%d %H:%M:%S", &horaLocal);
            std::string textoGuardado = contenido->generarContenido();
            Mensaje nuevo(sigMsg++, autor, idC, textoGuardado, fecha, padre);
            mensajes.insertarFinal(nuevo);
            canal->incrementarMensajes();
            usuarios.mostrar([&](const Usuario& u) {
                if (u.getId() != autor && canal->puedeEntrar(u.getId()))
                    notificaciones.encolar(Notificacion(sigNotif++, u.getId(), nuevo.getId(), "mensaje_nuevo"));
            });
            std::regex mencion("@([0-9]+)");
            for (std::sregex_iterator it(textoGuardado.begin(), textoGuardado.end(), mencion), fin; it != fin; ++it) {
                int mencionado = std::stoi((*it)[1].str());
                if (usuarios.buscar(Usuario(mencionado)) && canal->puedeEntrar(mencionado))
                    notificaciones.encolar(Notificacion(sigNotif++, mencionado, nuevo.getId(), "mencion"));
            }
            registrarActividad(autor, "Mensaje publicado", "Mensaje #" + std::to_string(nuevo.getId()) + " (" + contenido->getTipo() + ") en #" + canal->getNombre());
            std::cout << "Mensaje publicado como " << contenido->getTipo() << ": " << textoGuardado << "\n";
        }
        else if (op == 3) {
            if (mensajes.vacia()) { std::cout << "No hay mensajes para editar.\n"; continue; }
            mensajes.mostrar([](const Mensaje& m) { std::cout << "  ID " << m.getId() << " | canal " << m.getIdCanal() << " | " << m.getFecha() << " | " << m.getContenido() << "\n"; });
            int id; std::cout << "ID del mensaje que quieres editar: ";
            if (!(std::cin >> id)) { std::cin.clear(); std::cin.ignore(10000, '\n'); continue; }
            std::cin.ignore(10000, '\n');
            Mensaje* m = mensajes.buscar(Mensaje(id));
            if (!m) { std::cout << "No existe un mensaje con ese ID.\n"; continue; }
            std::string texto; std::cout << "Texto nuevo: "; std::getline(std::cin, texto);
            if (texto.empty() || texto == m->getContenido()) { std::cout << "El texto debe tener contenido y ser distinto al actual.\n"; continue; }
            ediciones.push(*m);
            m->setContenido(texto);
            notificaciones.encolar(Notificacion(sigNotif++, m->getIdAutor(), m->getId(), "mensaje_editado"));
            registrarActividad(m->getIdAutor(), "Mensaje editado", "Mensaje #" + std::to_string(m->getId()));
            std::cout << "Mensaje actualizado. La version anterior quedo en el historial de ediciones.\n";
        }
        else if (op == 4) {
            int id; std::cout << "ID del mensaje raiz: ";
            if (!(std::cin >> id)) { std::cin.clear(); std::cin.ignore(10000, '\n'); continue; }
            std::cin.ignore(10000, '\n'); verHiloConversacion(id);
        } else if (op != 0) std::cout << "Opcion no valida.\n";
    }
}
void SlackSistema::menuEdiciones() {
    int op;
    do {
        std::cout << "\n--- HISTORIAL DE EDICIONES (Pila) ---\n1. Listar historial\n2. Deshacer ultima edicion\n3. Ver edicion mas antigua (Fondo)\n0. Volver\nOpcion: ";
        std::cin >> op;
        if (op == 1) {
            ediciones.mostrar([](const Mensaje& m) { std::cout << "  [Historico] " << m << "\n"; });
        }
        else if (op == 2) {
            Mensaje revertido;
            if (ediciones.pop(revertido)) {
                mensajes.modificarSi(
                    [revertido](const Mensaje& m) { return m.getId() == revertido.getId(); },
                    [revertido](Mensaje& m) { m.setContenido(revertido.getContenido()); }
                );
                std::cout << "Mensaje revertido a: " << revertido.getContenido() << "\n";
            }
            else {
                std::cout << "Pila vacia.\n";
            }
        }
        else if (op == 3) {
            Mensaje fondo;
            if (ediciones.verFondo(fondo)) {
                std::cout << "Edicion inicial resguardada: " << fondo << "\n";
            }
            else {
                std::cout << "Pila vacia.\n";
            }
        }
    } while (op != 0);
}

void SlackSistema::menuNotificaciones() {
    int op = -1;
    while (op != 0) {
        std::cout << "\n================ NOTIFICACIONES ================\n1. Ver notificaciones\n2. Marcar como leida la siguiente\n3. Procesar varias\n0. Volver\nOpcion: ";
        if (!(std::cin >> op)) { std::cin.clear(); std::cin.ignore(10000, '\n'); std::cout << "Opcion no valida.\n"; continue; }
        std::cin.ignore(10000, '\n');
        if (op == 1) {
            if (notificaciones.vacia()) std::cout << "No tienes notificaciones pendientes.\n";
            notificaciones.mostrar([](const Notificacion& n) { std::cout << "  " << n << "\n"; });
        } else if (op == 2) {
            Notificacion n;
            if (notificaciones.desencolar(n)) { n.marcarLeida(); std::cout << "Atendida: " << n << "\n"; }
            else std::cout << "No hay notificaciones pendientes.\n";
        } else if (op == 3) {
            int cantidad; std::cout << "Cantidad a procesar: ";
            if (!(std::cin >> cantidad)) { std::cin.clear(); std::cin.ignore(10000, '\n'); continue; }
            std::cin.ignore(10000, '\n');
            std::vector<Notificacion> lote;
            int total = notificaciones.desencolarLote(cantidad, lote);
            for (auto& n : lote) n.marcarLeida();
            std::cout << "Se procesaron " << total << " notificaciones.\n";
        } else if (op != 0) std::cout << "Opcion no valida.\n";
    }
}
void SlackSistema::menuCanales() {
    int opcion = -1;
    while (opcion != 0) {
        std::cout << "\n==================================================\n  CANALES\n==================================================\n";
        canales.mostrar([](const std::shared_ptr<Canal>& c) {
            std::cout << "  [" << c->getId() << "] #" << c->getNombre() << " | " << c->getTipo() << " | " << c->getCantidadMensajes() << " mensajes\n";
        });
        std::cout << "\n  1. Abrir canal\n  2. Crear canal\n  3. Agregar usuario a canal privado\n  4. Cambiar tipo publico/privado\n  0. Volver\nElige una opcion: ";
        if (!(std::cin >> opcion)) { std::cin.clear(); std::cin.ignore(10000, '\n'); std::cout << "Opcion no valida.\n"; continue; }
        std::cin.ignore(10000, '\n');
        if (opcion == 0) break;
        if (opcion == 2) { crearCanal(); continue; }
        if (opcion == 3) {
            int idC, idU; std::cout << "ID del canal privado: "; if (!(std::cin >> idC)) { std::cin.clear(); std::cin.ignore(10000, '\n'); continue; }
            std::cout << "ID del usuario que podra entrar: "; if (!(std::cin >> idU)) { std::cin.clear(); std::cin.ignore(10000, '\n'); continue; } std::cin.ignore(10000, '\n');
            if (!usuarios.buscar(Usuario(idU))) { std::cout << "No existe ese usuario.\n"; continue; }
            bool agregado = false;
            canales.mostrar([&](const std::shared_ptr<Canal>& c) { if (c->getId() == idC && c->getTipo() == "privado") agregado = c->agregarUsuario(idU); });
            if (agregado) { notificaciones.encolar(Notificacion(sigNotif++, idU, idC, "acceso_canal")); registrarActividad(idU, "Acceso a canal privado", "Canal #" + std::to_string(idC)); }
            std::cout << (agregado ? "Acceso agregado.\n" : "No se agrego: canal inexistente, publico o usuario ya autorizado.\n"); continue;
        }
        if (opcion == 4) {
            int idC; std::cout << "ID del canal: "; if (!(std::cin >> idC)) { std::cin.clear(); std::cin.ignore(10000, '\n'); continue; } std::cin.ignore(10000, '\n');
            std::shared_ptr<Canal> actual;
            canales.mostrar([&](const std::shared_ptr<Canal>& c) { if (c->getId() == idC) actual = c; });
            if (!actual) { std::cout << "No existe ese canal.\n"; continue; }
            if (actual->getTipo() == "publico") {
                int idU; std::cout << "ID del usuario que tendra acceso al nuevo canal privado: ";
                if (!(std::cin >> idU)) { std::cin.clear(); std::cin.ignore(10000, '\n'); continue; } std::cin.ignore(10000, '\n');
                if (!usuarios.buscar(Usuario(idU))) { std::cout << "No existe ese usuario.\n"; continue; }
                std::vector<int> permitidos{ idU };
                canales.modificarSi([idC](const std::shared_ptr<Canal>& c) { return c->getId() == idC; }, [&](std::shared_ptr<Canal>& c) { c = std::make_shared<CanalPrivado>(idC, c->getNombre(), c->getCantidadMensajes(), permitidos); });
            } else {
                canales.modificarSi([idC](const std::shared_ptr<Canal>& c) { return c->getId() == idC; }, [](std::shared_ptr<Canal>& c) { c = std::make_shared<CanalPublico>(c->getId(), c->getNombre(), c->getCantidadMensajes()); });
            }
            std::string nuevoTipo = actual->getTipo() == "publico" ? "privado" : "publico";
            registrarActividad(0, "Tipo de canal cambiado", "Canal #" + std::to_string(idC) + " ahora es " + nuevoTipo);
            std::cout << "Tipo de canal actualizado.\n"; continue;
        }
        if (opcion != 1) { std::cout << "Opcion no valida.\n"; continue; }
        int idC; std::cout << "ID del canal que deseas abrir: ";
        if (!(std::cin >> idC)) { std::cin.clear(); std::cin.ignore(10000, '\n'); continue; } std::cin.ignore(10000, '\n');
        std::shared_ptr<Canal> elegido;
        canales.mostrar([&](const std::shared_ptr<Canal>& c) { if (c->getId() == idC) elegido = c; });
        if (!elegido) { std::cout << "No existe ese canal.\n"; continue; }
        int idUsuario; std::cout << "ID del usuario que ingresa: ";
        if (!(std::cin >> idUsuario)) { std::cin.clear(); std::cin.ignore(10000, '\n'); continue; }
        std::cin.ignore(10000, '\n');
        if (!usuarios.buscar(Usuario(idUsuario))) { std::cout << "No existe ese usuario.\n"; continue; }
        if (!elegido->puedeEntrar(idUsuario)) { std::cout << "Acceso denegado: ese usuario no pertenece al canal privado.\n"; continue; }
        std::vector<Mensaje> lista;
        mensajes.mostrar([&](const Mensaje& m) { if (m.getIdCanal() == idC) lista.push_back(m); });
        std::stable_sort(lista.begin(), lista.end(), [](const Mensaje& a, const Mensaje& b) { return a.getFecha() == b.getFecha() ? a.getId() < b.getId() : a.getFecha() < b.getFecha(); });
        std::cout << "\n---------------- #" << elegido->getNombre() << " (" << elegido->getTipo() << ") ----------------\n";
        if (lista.empty()) std::cout << "Este canal aun no tiene mensajes.\n";
        for (const auto& m : lista) {
            std::string autor = "Usuario " + std::to_string(m.getIdAutor());
            usuarios.mostrar([&](const Usuario& u) { if (u.getId() == m.getIdAutor()) autor = u.getNombre(); });
            std::cout << "\n" << autor << " | " << m.getFecha();
            if (m.getIdPadre()) std::cout << " | Respuesta a #" << m.getIdPadre();
            std::cout << "\n  " << m.getContenido() << "\n";
        }
        std::cout << "\nPresiona Enter para volver a Canales..."; std::cin.get();
    }
}

void SlackSistema::crearCanal() {
    std::string nombre; int tipo;
    std::cout << "Nombre del canal (sin #): "; std::getline(std::cin, nombre);
    if (nombre.empty()) { std::cout << "El nombre no puede estar vacio.\n"; return; }
    std::cout << "Tipo: 1. Publico  2. Privado: ";
    if (!(std::cin >> tipo)) { std::cin.clear(); std::cin.ignore(10000, '\n'); std::cout << "Opcion no valida.\n"; return; }
    std::cin.ignore(10000, '\n');
    if (tipo == 1) { int idNuevo = sigCanal++; canales.insertarFinal(std::make_shared<CanalPublico>(idNuevo, nombre)); registrarActividad(0, "Canal publico creado", nombre); }
    else if (tipo == 2) {
        int idU; std::cout << "ID del usuario que tendra acceso: ";
        if (!(std::cin >> idU)) { std::cin.clear(); std::cin.ignore(10000, '\n'); std::cout << "Opcion no valida.\n"; return; }
        std::cin.ignore(10000, '\n');
        if (!usuarios.buscar(Usuario(idU))) { std::cout << "No existe ese usuario.\n"; return; }
        int idNuevo = sigCanal++;
        canales.insertarFinal(std::make_shared<CanalPrivado>(idNuevo, nombre, 0, std::vector<int>{idU}));
        notificaciones.encolar(Notificacion(sigNotif++, idU, idNuevo, "invitacion_canal"));
        registrarActividad(idU, "Canal privado creado", nombre);
    } else { std::cout << "Tipo de canal no valido.\n"; return; }
    std::cout << "Canal creado con exito.\n";
}
void SlackSistema::ordenarCanalesPorActividad() {
    std::vector<std::shared_ptr<Canal>> v;
    canales.mostrar([&v](const std::shared_ptr<Canal>& c) { v.push_back(c); });
    if (v.empty()) return;
    auto cmpDesc = [](const std::shared_ptr<Canal>& a, const std::shared_ptr<Canal>& b) { return a->getCantidadMensajes() > b->getCantidadMensajes(); };
    quickSort(v, 0, static_cast<int>(v.size()) - 1, cmpDesc);
    std::cout << "\n>> Canales ordenados por actividad:\n";
    for (const auto& c : v) std::cout << "  " << *c << "\n";
}
void SlackSistema::ordenarUsuariosPorNombre() {
    std::vector<Usuario> v;
    usuarios.mostrar([&v](const Usuario& u) { v.push_back(u); });
    if (v.empty()) return;

    // Lambda 7 (Integrante 3): Comparador MergeSort alfabético
    auto cmpNom = [](const Usuario& a, const Usuario& b) {
        return a.getNombre() < b.getNombre();
        };
    mergeSort(v, 0, static_cast<int>(v.size()) - 1, cmpNom);

    std::cout << "\n>> Usuarios ordenados alfabeticamente (MergeSort):\n";
    for (const auto& u : v) std::cout << "  " << u << "\n";
}

void SlackSistema::ordenarMensajesPorFecha() {
    std::vector<Mensaje> v;
    mensajes.mostrar([&v](const Mensaje& m) { v.push_back(m); });
    if (v.empty()) return;

    // Lambda 8 (Integrante 3): Comparador HeapSort por fecha
    auto cmpFecha = [](const Mensaje& a, const Mensaje& b) {
        return a.getFecha() < b.getFecha();
        };
    heapSort(v, cmpFecha);

    std::cout << "\n>> Mensajes ordenados cronologicamente (HeapSort):\n";
    for (const auto& m : v) std::cout << "  " << m << "\n";
}

// ---------------- PERSISTENCIA DE ARCHIVOS ----------------
void SlackSistema::guardarDatos() {
    // 1. Usuarios
    std::ofstream fu("usuarios.txt");
    if (fu) {
        // Lambda 9 (Integrante 3): Escritura formateada
        usuarios.mostrar([&fu](const Usuario& u) {
            fu << u.getId() << "|" << u.getNombre() << "|" << u.getEmail() << "|" << u.getEstado() << "|" << u.getRol() << "\n";
            });
        fu.close();
    }

    // 2. Canales
    std::ofstream fc("canales.txt");
    if (fc) {
        canales.mostrar([&fc](const std::shared_ptr<Canal>& c) {
            fc << c->getId() << "|" << c->getNombre() << "|" << c->getTipo() << "|" << c->getCantidadMensajes() << "|" << c->getAccesos() << "\n";
        });
        fc.close();
    }
    // 3. Amistades
    std::ofstream fam("amistades.txt");
    if (fam) {
        amistades.mostrar([&fam](const Amistad& a) { fam << a.getIdUsuario1() << "|" << a.getIdUsuario2() << "\n"; });
        fam.close();
    }

    // 4. Registro de actividad
    std::ofstream fact("actividad.txt");
    if (fact) {
        actividad.mostrar([&fact](const RegistroActividad& r) {
            fact << r.getId() << "|" << r.getIdUsuario() << "|" << r.getFecha() << "|" << r.getAccion() << "|" << r.getDetalle() << "\n";
        });
        fact.close();
    }

    // 5. Mensajes
    std::ofstream fm("mensajes.txt");
    if (fm) {
        mensajes.mostrar([&fm](const Mensaje& m) {
            fm << m.getId() << "|" << m.getIdAutor() << "|" << m.getIdCanal() << "|" << m.getIdPadre() << "|" << m.getFecha() << "|" << m.getContenido() << "\n";
            });
        fm.close();
    }
    std::cout << "OK: Datos guardados en usuarios.txt, canales.txt, mensajes.txt, amistades.txt y actividad.txt\n";
}

void SlackSistema::cargarDatos() {
    std::ifstream fu("usuarios.txt");
    if (fu) {
        usuarios.vaciar();
        std::string l;
        while (std::getline(fu, l)) {
            std::stringstream ss(l);
            std::string id, n, e, es, r;
            if (std::getline(ss, id, '|') && std::getline(ss, n, '|') && std::getline(ss, e, '|') && std::getline(ss, es, '|') && std::getline(ss, r)) {
                int uid = std::stoi(id);
                usuarios.insertarFinal(Usuario(uid, n, e, es, r));
                if (uid >= sigUsr) sigUsr = uid + 1;
            }
        }
        fu.close();
    }

    std::ifstream fam("amistades.txt");
    if (fam) {
        amistades.vaciar();
        int id1, id2; char separador;
        while (fam >> id1 >> separador >> id2) {
            if (usuarios.buscar(Usuario(id1)) && usuarios.buscar(Usuario(id2))) amistades.insertarFinal(Amistad(id1, id2));
        }
    }

    std::ifstream fact("actividad.txt");
    if (fact) {
        actividad.vaciar();
        std::string linea;
        while (std::getline(fact, linea)) {
            std::stringstream ss(linea);
            std::string id, usuario, fecha, accion, detalle;
            if (std::getline(ss, id, '|') && std::getline(ss, usuario, '|') && std::getline(ss, fecha, '|') && std::getline(ss, accion, '|') && std::getline(ss, detalle)) {
                int idRegistro = std::stoi(id);
                actividad.insertarFinal(RegistroActividad(idRegistro, std::stoi(usuario), fecha, accion, detalle));
                if (idRegistro >= sigActividad) sigActividad = idRegistro + 1;
            }
        }
    }
    std::ifstream fc("canales.txt");
    if (fc) {
        canales.vaciar();
        std::string l;
        while (std::getline(fc, l)) {
            std::stringstream ss(l);
            std::string id, n, t, cm, accesos;
            if (std::getline(ss, id, '|') && std::getline(ss, n, '|') && std::getline(ss, t, '|') && std::getline(ss, cm, '|')) {
                std::getline(ss, accesos);
                int cid = std::stoi(id), cantidad = std::stoi(cm);
                if (t == "privado") {
                    std::vector<int> permitidos; std::stringstream ids(accesos); std::string token;
                    while (std::getline(ids, token, ',')) if (!token.empty()) permitidos.push_back(std::stoi(token));
                    canales.insertarFinal(std::make_shared<CanalPrivado>(cid, n, cantidad, permitidos));
                } else canales.insertarFinal(std::make_shared<CanalPublico>(cid, n, cantidad));
                if (cid >= sigCanal) sigCanal = cid + 1;
            }
        }
        fc.close();
    }
    std::ifstream fm("mensajes.txt");
    if (fm) {
        mensajes.vaciar();
        std::string l;
        while (std::getline(fm, l)) {
            std::stringstream ss(l);
            std::string id, a, c, p, fe, co;
            if (std::getline(ss, id, '|') && std::getline(ss, a, '|') && std::getline(ss, c, '|') && std::getline(ss, p, '|') && std::getline(ss, fe, '|') && std::getline(ss, co)) {
                int mid = std::stoi(id);
                mensajes.insertarFinal(Mensaje(mid, std::stoi(a), std::stoi(c), co, fe, std::stoi(p)));
                if (mid >= sigMsg) sigMsg = mid + 1;
            }
        }
        fm.close();
    }
    std::cout << "OK: Registros cargados desde el almacenamiento.\n";
}

void SlackSistema::verResumen() {
    std::cout << "\n================ ESTADO DEL SISTEMA ================\n";
    std::cout << "Usuarios registrados: " << usuarios.size() << "\n";
    std::cout << "Canales activos:      " << canales.size() << "\n";
    std::cout << "Mensajes totales:     " << mensajes.size() << "\n";
    std::cout << "Ediciones en pila:    " << ediciones.size() << "\n";
    std::cout << "Notificaciones cola:  " << notificaciones.size() << "\n";
    std::cout << "====================================================\n";
}