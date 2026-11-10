#include "SlackSistema.h"
#include <iostream>

int main() {
    SlackSistema slack;
    int op;
    do {
        std::cout << "\n";
        std::cout << "  ==================================================\n";
        std::cout << "  |             SLACK - UPC 2026-2                 |\n";
        std::cout << "  ==================================================\n";
        std::cout << "  |  ESPACIO DE TRABAJO                            |\n";
        std::cout << "  |   1. Canales                                   |\n";
        std::cout << "  |   2. Usuarios                                  |\n";
        std::cout << "  |  ACTIVIDAD                                     |\n";
        std::cout << "  |   3. Mensajes e hilos                          |\n";
        std::cout << "  |   4. Historial de ediciones                    |\n";
        std::cout << "  |   5. Notificaciones                            |\n";
        std::cout << "  |  CONSULTAS                                     |\n";
        std::cout << "  |   6. Ordenar canales por actividad             |\n";
        std::cout << "  |   7. Ordenar usuarios por nombre               |\n";
        std::cout << "  |   8. Ordenar mensajes por fecha                |\n";
        std::cout << "  |  DATOS                                         |\n";
        std::cout << "  |   9. Guardar datos                             |\n";
        std::cout << "  |  10. Cargar datos                              |\n";
        std::cout << "  |  11. Resumen general                           |\n";
        std::cout << "  |  12. Registro de actividad                      |\n";
        std::cout << "  |   0. Salir                                     |\n";
        std::cout << "  ==================================================\n";
        std::cout << "  Selecciona una opcion: ";

        if (!(std::cin >> op)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Entrada no valida. Escribe el numero de una opcion.\n";
            continue;
        }
        std::cin.ignore(10000, '\n');

        switch (op) {
        case 1:  slack.menuCanales(); break;
        case 2:  slack.menuUsuarios(); break;
        case 3:  slack.menuMensajes(); break;
        case 4:  slack.menuEdiciones(); break;
        case 5:  slack.menuNotificaciones(); break;
        case 6:  slack.ordenarCanalesPorActividad(); break;
        case 7:  slack.ordenarUsuariosPorNombre(); break;
        case 8:  slack.ordenarMensajesPorFecha(); break;
        case 9:  slack.guardarDatos(); break;
        case 10: slack.cargarDatos(); break;
        case 11: slack.verResumen(); break;
        case 12: slack.verRegistroActividad(); break;
        case 0:  std::cout << "\nCerrando sesion en Slack...\n"; break;
        default: std::cout << "\nOpcion no valida. Elige un numero del menu.\n";
        }
    } while (op != 0);

    return 0;
}