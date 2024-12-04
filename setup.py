from setuptools import setup, Extension
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "avx_sum_of_squares",
        ["avx_sum_of_squares.cpp"],
        extra_compile_args=["-march=native", "-O3"],  # Optimización y soporte AVX
    ),
]

setup(
    name="avx_sum_of_squares",
    version="1.0",
    author="Tu Nombre",
    description="Cálculo de suma de cuadrados usando AVX y Pybind11",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
)
