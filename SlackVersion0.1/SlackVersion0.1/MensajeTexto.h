#pragma once
#include "ContenidoMensaje.h"
using namespace std;

class MensajeTexto : public ContenidoMensaje
{
private:
    string texto;

public:
    MensajeTexto(string contenido = "") { texto = contenido; }
    string generarContenido() const override { return texto; }
    string getTipo() const override { return "texto"; }
};