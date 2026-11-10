#include "SlackSistema.h"
#include <iostream>
using namespace std;

int main() {
    SlackSistema slack;
    int op;
    do {
        cout << "\n";
        cout << "  ==================================================\n";
        cout << "  |             SLACK - UPC 2026-2                 |\n";
        cout << "  ==================================================\n";
        cout << "  |  ESPACIO DE TRABAJO                            |\n";
        cout << "  |   1. Canales                                   |\n";
        cout << "  |   2. Usuarios                                  |\n";
        cout << "  |  ACTIVIDAD                                     |\n";
        cout << "  |   3. Mensajes e hilos                          |\n";
        cout << "  |   4. Historial de ediciones                    |\n";
        cout << "  |   5. Notificaciones                            |\n";
        cout << "  |  CONSULTAS                                     |\n";
        cout << "  |   6. Ordenar canales por actividad             |\n";
        cout << "  |   7. Ordenar usuarios por nombre               |\n";
        cout << "  |   8. Ordenar mensajes por fecha                |\n";
        cout << "  |  DATOS                                         |\n";
        cout << "  |   9. Guardar datos                             |\n";
        cout << "  |  10. Cargar datos                              |\n";
        cout << "  |  11. Resumen general                           |\n";
        cout << "  |  12. Registro de actividad                     |\n";
        cout << "  |   0. Salir                                     |\n";
        cout << "  ==================================================\n";
        cout << "  Selecciona una opcion: ";

        if (!(cin >> op)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Entrada no valida. Escribe el numero de una opcion.\n";
            continue;
        }
        cin.ignore(10000, '\n');

        switch (op) {
        case 1:system("cls");  slack.menuCanales(); system("cls"); break;
        case 2:system("cls");  slack.menuUsuarios(); system("cls"); break;
        case 3:system("cls");  slack.menuMensajes(); system("cls"); break;
        case 4:system("cls");  slack.menuEdiciones(); system("cls"); break;
        case 5:system("cls");  slack.menuNotificaciones(); system("cls"); break;
        case 6:system("cls");  slack.ordenarCanalesPorActividad(); system("pause>0"); system("cls"); break;
        case 7:system("cls");  slack.ordenarUsuariosPorNombre(); system("pause>0"); system("cls"); break;
        case 8:system("cls");  slack.ordenarMensajesPorFecha(); system("pause>0"); system("cls"); break;
        case 9:system("cls");  slack.guardarDatos(); system("pause>0"); system("cls"); break;
        case 10:system("cls"); slack.cargarDatos(); system("pause>0"); system("cls"); break;
        case 11:system("cls"); slack.verResumen(); system("pause>0"); system("cls"); break;
        case 12:system("cls"); slack.verRegistroActividad(); system("pause>0"); system("cls"); break;
        case 0:  cout << "\nCerrando sesion en Slack...\n"; break;
        default: cout << "\nOpcion no valida. Elige un numero del menu.\n";
        }
    } while (op != 0);

    return 0;
}