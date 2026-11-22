#pragma once
class Amistad {
    public:
        int id;
        int idUsuario1;
        int idUsuario2;

        Amistad(int id = 0, int u1 = 0, int u2 = 0) {
            this->id = id;
            this->idUsuario1 = u1;
            this->idUsuario2 = u2;
        }
};