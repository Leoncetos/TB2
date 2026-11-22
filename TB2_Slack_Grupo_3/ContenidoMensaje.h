#pragma once
#include <string>

using namespace std;

class ContenidoMensaje {
public:
    virtual ~ContenidoMensaje() {}
    virtual string generarContenido() const = 0;
    virtual string getTipo() const = 0;
};