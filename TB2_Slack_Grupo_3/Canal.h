#pragma once
class Canal {
    protected:
        int id;
        string nombre;
        int cantidadMensajes;

    public:
        Canal(int id = 0, string nombre = "", int cantMsgs = 0) {
            this->id = id;
            this->nombre = nombre;
            this->cantidadMensajes = cantMsgs;
        }
        virtual ~Canal() {}

        int getId() const { return id; }
        string getNombre() const { return nombre; }
        int getCantidadMensajes() const { return cantidadMensajes; }
        void incrementarMensajes() { cantidadMensajes++; }

        virtual string getTipo() const = 0;
        virtual bool puedeEntrar(int idUsuario) const = 0;
        virtual bool agregarUsuario(int) { return true; }
        virtual vector<int> getAccesos() const { return {}; }
};