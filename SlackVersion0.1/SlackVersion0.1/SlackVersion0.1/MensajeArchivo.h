#pragma once
#include "ContenidoMensaje.h"
using namespace std;

class MensajeArchivo : public ContenidoMensaje
{
private:
    string nombreArchivo;

public:
    MensajeArchivo(string nombre = "") { nombreArchivo = nombre; }
    string generarContenido() const override { return "[" + nombreArchivo + "]"; }
    string getTipo() const override { return "archivo"; }
};