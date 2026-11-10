#pragma once
#include "Estructuras.h"
#include "Entidades.h"
#include "Algoritmos.h"
#include <memory>
#include <string>

class SlackSistema {
private:
    ListaDoble<Usuario> usuarios;
    ListaDoble<Mensaje> mensajes;
    ListaDoble<std::shared_ptr<Canal>> canales;
    ListaDoble<Hilo> hilos;
    ListaDoble<Rol> roles;
    ListaDoble<Estado> estados;
    ListaDoble<Amistad> amistades;
    ListaDoble<RegistroActividad> actividad;

    Pila<Mensaje> ediciones;
    Cola<Notificacion> notificaciones;

    int sigMsg;
    int sigNotif;
    int sigUsr;
    int sigCanal;
    int sigActividad;

    // Método recursivo 2 (Integrante 2): Recorrido de hilos anidados (Árbol de conversación)
    void imprimirRespuestasRec(int idPadre, int nivel, const std::vector<Mensaje>& todosLosMsgs);

    // Método recursivo 3 (Integrante 3): Conteo recursivo de actividad total en respuestas
    int contarRespuestasTotalesRec(int idPadre, const std::vector<Mensaje>& todosLosMsgs);

public:
    SlackSistema();
    void cargarDemo();

    void menuUsuarios();
    void menuMensajes();
    void menuEdiciones();
    void menuNotificaciones();
    void menuCanales();
    void crearCanal();
    void agregarUsuario();
    void hacerAmistad();
    void verRegistroActividad();
    void registrarActividad(int idUsuario, const std::string& accion, const std::string& detalle);
    void verHiloConversacion(int idRaiz);

    // Ordenamientos
    void ordenarCanalesPorActividad();
    void ordenarUsuariosPorNombre();
    void ordenarMensajesPorFecha();

    // Archivos
    void guardarDatos();
    void cargarDatos();

    void verResumen();
};