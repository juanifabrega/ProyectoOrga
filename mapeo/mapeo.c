#include <stdlib.h>
#include "mapeo.h"

void crear_mapeo(tMapeo * m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)){

}

tValor m_insertar(tMapeo m, tClave c, tValor v){

}

void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){

}

void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){

}

tValor m_recuperar(tMapeo m, tClave c){

}