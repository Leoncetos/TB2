#pragma once
#include "Canal.h"

class CanalPublico : public Canal {
    public:
        CanalPublico(int id = 0, string nombre = "", int cantMsgs = 0)
            : Canal(id, nombre, cantMsgs) {
        }

        string getTipo() const override { return "publico"; }
        bool puedeEntrar(int) const override {
            return true;
        }
};