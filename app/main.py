from fastapi import FastAPI
import avx_sum_of_squares
import numpy as np
import time
from pydantic import BaseModel
from typing import List
import json
import random

def listaAleatorios(n: int):
      lista = [0]  * n
      for i in range(n):
          lista[i] = random.randint(0, 10000)* 0.0001
      return lista

app = FastAPI()

# Definir el modelo para el vector
class VectorF(BaseModel):
    vector: List[float]

@app.post("/avxsumsqr")
async def calculo(array_size: int):
    start = time.time()

    # Crear un gran arreglo de prueba
    data = np.random.rand(array_size).astype(np.float32)

    # Calcular la suma de cuadrados
    result = avx_sum_of_squares.sum_of_squares(data)
    #print("Suma de cuadrados:", data)
    #print("Suma de cuadrados:", result[0])

    end = time.time()

    var1 = end - start

    j1 = {
        "Time taken in seconds": var1,
        "Data": data,
        "Resultado esperado (Avx Sum Squares)": result
    }
    jj = json.dumps(str(j1))

    return jj