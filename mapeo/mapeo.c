#include <stdlib.h>
#include "mapeo.h"

/**
 * Determina y retorna el valor más alto entre val1 y val2.
 * @param val1 el primer valor a comparar.
 * @param val2 el segundo valor a comparar.
 * @return val1 si val1 es mayor a val2, val2 en caso contrario.
 */
int max(int val1, int val2){
    return val1 > val2 ? val1 : val2;
}

void crear_mapeo(tMapeo * m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)){
    int capacity = max(ci, 10);
    *m = (tMapeo) malloc(sizeof(struct mapeo));
    if (*m == NULL){
        exit(MAP_ERROR_MEMORIA);
    }
    (*m)->longitud_tabla = capacity;
    (*m)->cantidad_elementos = 0;
    (*m)->hash_code = fHash;
    (*m)->comparador = fComparacion;
    (*m)->tabla_hash = NULL;
    crear_lista((*m)->tabla_hash);
}

tValor m_insertar(tMapeo m, tClave c, tValor v){

}

void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){

}

void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){

}

tValor m_recuperar(tMapeo m, tClave c){

}