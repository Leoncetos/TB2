#pragma once
#include <vector>
#include <algorithm>

using namespace std;

template <typename T, typename Comparador>
void heapify(vector<T>& arr, int n, int i, Comparador comp) {
    int extremo = i;
    int izq = 2 * i + 1;
    int der = 2 * i + 2;

    if (izq < n && comp(arr[extremo], arr[izq])) extremo = izq;
    if (der < n&& comp(arr[extremo], arr[der])) extremo = der;

    if (extremo != i) {
        swap(arr[i], arr[extremo]);
        heapify(arr, n, extremo, comp);
    }
}

template <typename T, typename Comparador>
void heapSort(vector<T>& arr, Comparador comp) {
    int n = (int)arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i, comp);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0, comp);
    }
}