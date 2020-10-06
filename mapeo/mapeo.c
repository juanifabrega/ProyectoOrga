#include <stdlib.h>
#include "mapeo.h"

/**
 * Determina y retorna el valor más alto entre val1 y val2
 * @param val1 el primer valor a comparar
 * @param val2 el segundo valor a comparar
 * @return val1 si val1 es mayor a val2, val2 en caso contrario.
 */
int max(int val1, int val2){
    return val1 > val2 ? val1 : val2;
}

void crear_mapeo(tMapeo * m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)){
    int capacity = max(ci, 10);
}

tValor m_insertar(tMapeo m, tClave c, tValor v){

}

void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){

}

void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){

}

tValor m_recuperar(tMapeo m, tClave c){

}