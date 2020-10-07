#include <stdio.h>
#include <string.h>
#include "mapeo/mapeo.h"

#define SIZE 101

/**
 * Elimina el elemento e
 * @param e el elemento
 */
void fEliminarCV(void * e){
    e = NULL;
}

int hashCode(tClave key){
    if(key < 0){
        return -(strlen(key) % SIZE);
    }
    return strlen(key) % SIZE;
}

int comparator(tClave k1, tClave k2){
    if(k1 < k2){
        return 1;
    }
    return 0;
}

int main() {
    tMapeo m = NULL;
    crear_mapeo(&m, SIZE, hashCode, comparator);
    tClave c1 = "Hi";
    tValor v1 = "Bye";
    tClave c2 = "Hola";
    tValor v2 = "Chau";
    m_insertar(m, c1, v1);
    m_insertar(m, c2, v2);
    printf("Hi: %s\n", m_recuperar(m, c1));
    printf("Hola: %s\n", m_recuperar(m, c2));
    m_insertar(m, c1, c2);
    printf("Hi in Spanish: %s\n", m_recuperar(m, c1));
    m_eliminar(m, c1, fEliminarCV, fEliminarCV);
    printf("Hi in Spanish deleted: %s\n", m_recuperar(m, c1));
    return 0;
}
