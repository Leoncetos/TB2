#pragma once
// Funcion hash compleja para claves enteras: mezcla multiplicativa con primos y corrimiento de bits
unsigned long long calcularHashPropio(int clave) {
    unsigned long long x = (clave < 0) ? (unsigned long long)(-(long long)clave) : (unsigned long long)clave;
    x = (x ^ (x >> 16)) * 2654435761ULL;
    x = (x ^ (x >> 13)) * 2246822519ULL;
    x = x ^ (x >> 16);
    return x;
}

// Funcion hash compleja para cadenas: acumulacion polinomica (Horner), ponderacion por posicion y mezcla
unsigned long long calcularHashPropio(const string& clave) {
    unsigned long long h = 5381ULL;
    const unsigned long long base = 31ULL;
    const unsigned long long primo = 1000000007ULL;
    for (int i = 0; i < (int)clave.length(); i++) {
        unsigned long long c = (unsigned char)clave[i];
        h = (h * base + c * (unsigned long long)(i + 1)) % primo;
        h = h ^ (h >> 7);
    }
    h = (h ^ (h >> 16)) * 2654435761ULL;
    return h ^ (h >> 13);
}