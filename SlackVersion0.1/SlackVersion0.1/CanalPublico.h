#pragma once
#include "Canal.h"
using namespace std;

class CanalPublico : public Canal
{
public:
    CanalPublico(int i = 0, string n = "", int mensajes = 0)
        : Canal(i, n, mensajes) {}

    // Funciones virtuales
    string getTipo() const override { return "publico"; }
    bool puedeEntrar(int) const override { return true; }
    bool agregarUsuario(int) override { return true; }
    string getAccesos() const override { return ""; }
};