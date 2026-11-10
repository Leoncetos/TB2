#pragma once
#include <vector>
#include <utility>
// Algoritmos de ordenamiento
// Algoritmo 1 (Integrante 1): QuickSort
template <typename T, typename Compare>
void quickSort(std::vector<T>& a, int lo, int hi, Compare cmp) {
    if (lo >= hi) return;
    T piv = a[hi];
    int i = lo - 1;
    for (int j = lo; j < hi; j++) {
        if (cmp(a[j], piv)) {
            i++;
            std::swap(a[i], a[j]);
        }
    }
    std::swap(a[i + 1], a[hi]);
    int p = i + 1;
    quickSort(a, lo, p - 1, cmp);
    quickSort(a, p + 1, hi, cmp);
}

// Algoritmo 2 (Integrante 2): MergeSort
template <typename T, typename Compare>
void mergeSort(std::vector<T>& a, int lo, int hi, Compare cmp) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    mergeSort(a, lo, mid, cmp);
    mergeSort(a, mid + 1, hi, cmp);

    std::vector<T> tmp;
    int i = lo, j = mid + 1;
    while (i <= mid && j <= hi) {
        if (cmp(a[i], a[j])) tmp.push_back(a[i++]);
        else tmp.push_back(a[j++]);
    }
    while (i <= mid) tmp.push_back(a[i++]);
    while (j <= hi) tmp.push_back(a[j++]);
    for (size_t k = 0; k < tmp.size(); k++) a[lo + k] = tmp[k];
}

// Algoritmo 3 (Integrante 3): HeapSort
template <typename T, typename Compare>
void heapify(std::vector<T>& a, int n, int i, Compare cmp) {
    int may = i, l = 2 * i + 1, r = 2 * i + 2;
    if (l < n && cmp(a[may], a[l])) may = l;
    if (r < n&& cmp(a[may], a[r])) may = r;
    if (may != i) {
        std::swap(a[i], a[may]);
        heapify(a, n, may, cmp);
    }
}

template <typename T, typename Compare>
void heapSort(std::vector<T>& a, Compare cmp) {
    int n = static_cast<int>(a.size());
    for (int i = n / 2 - 1; i >= 0; i--) heapify(a, n, i, cmp);
    for (int i = n - 1; i > 0; i--) {
        std::swap(a[0], a[i]);
        heapify(a, i, 0, cmp);
    }
}