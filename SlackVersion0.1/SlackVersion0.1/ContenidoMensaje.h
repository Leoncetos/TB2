#pragma once
#include <string>
using namespace std;

class ContenidoMensaje
{
public:
    virtual ~ContenidoMensaje() = default;
    virtual string generarContenido() const = 0;
    virtual string getTipo() const = 0;
};