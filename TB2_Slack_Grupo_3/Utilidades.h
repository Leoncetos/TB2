#pragma once
#include <iostream>
#include <string>

using namespace std;

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