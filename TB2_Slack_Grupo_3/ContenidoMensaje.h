#pragma once
#include <iostream>

class ContenidoMensaje {
public:
    virtual ~ContenidoMensaje() {}
    virtual string generarContenido() const = 0;
    virtual string getTipo() const = 0;
};