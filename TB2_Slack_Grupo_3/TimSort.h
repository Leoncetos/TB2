#pragma once
#include <vector>

using namespace std;


template <typename T, typename Comparador>
void insertionSortRango(vector<T>& arr, int inicio, int fin, Comparador comp) {
    for (int i = inicio + 1; i <= fin; i++) {
        T actual = arr[i];
        int j = i - 1;
        while (j >= inicio && comp(actual, arr[j])) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = actual;
    }
}

template <typename T, typename Comparador>
void timSort(vector<T>& arr, Comparador comp) {
    int n = (int)arr.size();
    const int TAMANIO_RUN = 32;

    // 1. Ordenar bloques pequenos (runs) con Insertion Sort
    for (int inicio = 0; inicio < n; inicio += TAMANIO_RUN) {
        int fin = inicio + TAMANIO_RUN - 1;
        if (fin >= n) fin = n - 1;
        insertionSortRango(arr, inicio, fin, comp);
    }

    // 2. Mezclar los runs ordenados de forma iterativa con Merge
    for (int tam = TAMANIO_RUN; tam < n; tam = tam * 2) {
        for (int inicio = 0; inicio < n; inicio += tam * 2) {
            int medio = inicio + tam - 1;
            int fin = inicio + tam * 2 - 1;
            if (medio >= n - 1) continue;
            if (fin >= n) fin = n - 1;
            merge(arr, inicio, medio, fin, comp);
        }
    }
}