// ===========================================
// Particionar el arreglo
// ===========================================
template <typename T, typename Comparador>
int particionQuick(vector<T>& arr, int bajo, int alto, Comparador comp) {
    // Seleccion de pivote central para evitar peor caso O(n^2) en arreglos ordenados
    int medio = bajo + (alto - bajo) / 2;
    swap(arr[medio], arr[alto]);

    // Elegimos el último elemento como pivote
    T pivote = arr[alto];
    int i = bajo - 1;
    for (int j = bajo; j < alto; j++) {
        // Si el elemento cumple el criterio respecto al pivote
        if (comp(arr[j], pivote)) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    // Colocar el pivote en su posición correcta
    swap(arr[i + 1], arr[alto]);
    return i + 1;
}
// ===========================================
// QuickSort
// ===========================================
template <typename T, typename Comparador>
void quickSort(vector<T>& arr, int bajo, int alto, Comparador comp) {
    // Caso base
    if (bajo >= alto)
        return;
    // Obtener posición del pivote
    int pi = particionQuick(arr, bajo, alto, comp);
    // Ordenar lado izquierdo
    quickSort(arr, bajo, pi - 1, comp);
    // Ordenar lado derecho
    quickSort(arr, pi + 1, alto, comp);
}