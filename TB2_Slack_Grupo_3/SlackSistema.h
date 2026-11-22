#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <ctime>

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

        if (opcion == -1) continue;
        cin.ignore();

        switch (opcion) {
        case 1: /*Canales*/ break;
        case 2: /*Usuarios*/ break;
        case 3: /*Mensajes e hilos*/ break;
        case 4: /*Historial de ediciones*/ break;
        case 5: /*Notificaciones*/ break;
        case 6: /*Ord canales*/ break;
        case 7: /*Ord usuarios*/ break;
        case 8: /*Ord mensajes*/ break;
        case 9: /*Guardar datos*/ break;
        case 10: /*Cargar datos*/ break;
        case 11: /*Resumen general*/; break;
        case 12: /*Registro de actividad*/ break;
        case 0: cout << "Saliendo de Slack - UPC 2026-2...\n"; break;
        default: cout << "Opcion invalida.\n"; break;
        }
    } while (opcion != 0);
}
};