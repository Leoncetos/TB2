#pragma once
class Usuario {
    public:
        int id;
        string nombre;
        string email;
        string estado;
        string rol;

        Usuario(int id = 0, string nom = "", string mail = "", string est = "activo", string r = "Miembro") {
            this->id = id;
            this->nombre = nom;
            this->email = mail;
            this->estado = est;
            this->rol = r;
        }
};