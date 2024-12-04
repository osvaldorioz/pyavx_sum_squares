#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <immintrin.h> // Instrucciones AVX
#include <vector>
#include <iostream>

namespace py = pybind11;

// Función para calcular la suma de cuadrados utilizando AVX
float sum_of_squares_avx(const std::vector<float>& data) {
    size_t size = data.size();
    const size_t avx_size = 8; // AVX opera en bloques de 8 floats
    size_t i = 0;
    __m256 sum = _mm256_setzero_ps(); // Vector para almacenar la suma parcial

    // Procesar en bloques de 8 elementos
    for (; i + avx_size <= size; i += avx_size) {
        __m256 vec = _mm256_loadu_ps(&data[i]);   // Cargar 8 elementos en un vector AVX
        __m256 square = _mm256_mul_ps(vec, vec); // Elevar cada elemento al cuadrado
        sum = _mm256_add_ps(sum, square);        // Acumular la suma parcial
    }

    // Reducir el vector AVX a un único valor escalar
    alignas(32) float sum_array[avx_size];
    _mm256_store_ps(sum_array, sum);
    float result = 0.0f;
    for (size_t j = 0; j < avx_size; ++j) {
        result += sum_array[j];
    }

    // Procesar los elementos restantes (si el tamaño no es múltiplo de 8)
    for (; i < size; ++i) {
        result += data[i] * data[i];
    }

    return result;
}

// Exposición de la funcionalidad con Pybind11
py::array_t<float> sum_of_squares(py::array_t<float> input_array) {
    py::buffer_info buf = input_array.request();
    if (buf.ndim != 1) {
        throw std::runtime_error("Input debe ser un array unidimensional.");
    }

    // Convertir el buffer de entrada a un vector de floats
    float* ptr = static_cast<float*>(buf.ptr);
    size_t size = buf.shape[0];
    std::vector<float> data(ptr, ptr + size);

    // Calcular la suma de cuadrados usando AVX
    float result = sum_of_squares_avx(data);

    // Devolver el resultado en un array de un elemento
    return py::array_t<float>({1}, {sizeof(float)}, &result);
}

// Crear el módulo Pybind11
PYBIND11_MODULE(avx_sum_of_squares, m) {
    m.def("sum_of_squares", &sum_of_squares, "Calcula la suma de cuadrados usando AVX");
}
