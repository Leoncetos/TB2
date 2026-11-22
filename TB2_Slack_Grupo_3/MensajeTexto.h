#pragma once
#include <iostream>

class MensajeTexto : public ContenidoMensaje {
private:
    string texto;

public:
    MensajeTexto(string t = "") { texto = t; }
    string generarContenido() const override { return texto; }
    string getTipo() const override { return "texto"; }
};