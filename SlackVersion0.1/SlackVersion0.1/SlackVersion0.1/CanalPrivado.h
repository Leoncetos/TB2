#pragma once
#include "Canal.h"
using namespace std;

class CanalPrivado : public Canal
{
private:
    vector<int> usuariosPermitidos;

public:
    CanalPrivado(int i = 0, string n = "", int mensajes = 0,
                 const vector<int>& permitidos = {})
        : Canal(i, n, mensajes) {
        usuariosPermitidos = permitidos;
    }

    // Funciones virtuales
    string getTipo() const override { return "privado"; }

    bool puedeEntrar(int idUsuario) const override {
        return find(usuariosPermitidos.begin(), usuariosPermitidos.end(), idUsuario)
            != usuariosPermitidos.end();
    }

    bool agregarUsuario(int idUsuario) override {
        if (puedeEntrar(idUsuario)) return false;
        usuariosPermitidos.push_back(idUsuario);
        return true;
    }

    string getAccesos() const override {
        string ids;
        for (int idUsuario : usuariosPermitidos) {
            if (!ids.empty()) ids += ",";
            ids += to_string(idUsuario);
        }
        return ids;
    }
};