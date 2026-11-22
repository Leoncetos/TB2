#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <ctime>
//Clases
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
//Funciones extra
#include "FuncionesHash.h"
#include "TablaHash.h"

void ejecutarMenuPrincipal() {

    // Generador del dataset para las 150k datos
    string generarContenidoAleatorio(int idMensaje, int nUsuarios, string & tipoOut) {
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