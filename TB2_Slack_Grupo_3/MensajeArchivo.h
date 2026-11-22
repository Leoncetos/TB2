#pragma once
#include <string>
#include "ContenidoMensaje.h"

using namespace std;

class MensajeArchivo : public ContenidoMensaje {
private:
    string nombreArchivo;

public:
    MensajeArchivo(string arch = "") { nombreArchivo = arch; }
    string generarContenido() const override { return "[" + nombreArchivo + "]"; }
    string getTipo() const override { return "archivo"; }
};
