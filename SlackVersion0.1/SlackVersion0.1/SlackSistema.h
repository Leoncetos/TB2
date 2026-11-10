#pragma once
#include "Estructuras.h"
#include "Entidades.h"
#include "Algoritmos.h"
#include <memory>
#include <string>
using namespace std;

class SlackSistema {
private:
    ListaDoble<Usuario> usuarios;
    ListaDoble<Mensaje> mensajes;
    ListaDoble<shared_ptr<Canal>> canales;
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

    // Metodo recursivo 2 (In2): Recorrido de hilos anidados (Arbol de conversaci�n)
    void imprimirRespuestasRec(int idPadre, int nivel, const vector<Mensaje>& todosLosMsgs);

    // Metodo recursivo 3 (In3): Conteo recursivo de actividad total en respuestas
    int contarRespuestasTotalesRec(int idPadre, const vector<Mensaje>& todosLosMsgs);

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
    void registrarActividad(int idUsuario, const string& accion, const string& detalle);
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