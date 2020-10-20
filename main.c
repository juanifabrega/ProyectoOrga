#include <stdio.h>
#include <string.h>
#include "mapeo/mapeo.h"

/**
 * Elimina el elemento e
 * @param e el elemento
 */
void fEliminarCV(void * e){
    e = NULL;
}

int hashCode(void * key){
    return (int) strlen(key);
}

int comparator(void * k1, void * k2){
    return strcmp(k1, k2) == 0;
}

int main() {
    tMapeo m = NULL;
    crear_mapeo(&m, 101, hashCode, comparator);
    m_insertar(m, "Hi", "Bye");
    printf("Hi: %s\n", m_recuperar(m, "Hi"));
    m_insertar(m, "Hola", "Chau");
    printf("Hola: %s\n", m_recuperar(m, "Hola"));
    m_insertar(m, "Hi", "Hola");
    printf("Hi in Spanish: %s\n", m_recuperar(m, "Hi"));
    m_eliminar(m, "Hi", &fEliminarCV, &fEliminarCV);
    printf("Hi in Spanish deleted: %s\n", m_recuperar(m, "Hi"));
    m_destruir(&m, &fEliminarCV, &fEliminarCV);
    if(m != NULL){
        return 2;
    }
    return 0;
}
