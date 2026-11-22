#pragma once
#include "Canal.h"

class CanalPrivado : public Canal {
    private:
        vector<int> usuariosPermitidos;

    public:
        CanalPrivado(int id = 0, string nombre = "", int cantMsgs = 0, vector<int> permitidos = {})
            : Canal(id, nombre, cantMsgs) {
            usuariosPermitidos = permitidos;
        }

        string getTipo() const override { return "privado"; }

        bool puedeEntrar(int idUsuario) const override {
            for (int u : usuariosPermitidos) {
                if (u == idUsuario) return true;
            }
            return false;
        }

        bool agregarUsuario(int idUsuario) override {
            if (!puedeEntrar(idUsuario)) {
                usuariosPermitidos.push_back(idUsuario);
                return true;
            }
            return false;
        }

        vector<int> getAccesos() const override { return usuariosPermitidos; }
};