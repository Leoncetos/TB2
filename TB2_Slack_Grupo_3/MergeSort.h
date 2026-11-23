#pragma once
#include <vector>

using namespace std;


template <typename T, typename Comparador>
void merge(vector<T>& arr, int izq, int medio, int der, Comparador comp) {
    int n1 = medio - izq + 1;
    int n2 = der - medio;
    vector<T> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[izq + i];
    for (int j = 0; j < n2; j++) R[j] = arr[medio + 1 + j];

    int i = 0, j = 0, k = izq;
    while (i < n1 && j < n2) {
        if (comp(L[i], R[j])) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

template <typename T, typename Comparador>
void mergeSort(vector<T>& arr, int izq, int der, Comparador comp) {
    if (izq < der) {
        int medio = izq + (der - izq) / 2;
        mergeSort(arr, izq, medio, comp);
        mergeSort(arr, medio + 1, der, comp);
        merge(arr, izq, medio, der, comp);
    }
}