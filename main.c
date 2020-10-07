#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mapeo/mapeo.h"

#define SIZE 101

int hashCode(tClave key){
    if(key < 0){
        return -(key % SIZE);
    }
    return key % SIZE;
}

int comparator(tClave k1, tClave k2){
    if(k1 < k2){
        return 1;
    }
    return 0;
}

int main() {
    tMapeo m = (tMapeo) malloc(sizeof(struct mapeo));
    crear_mapeo(&m, SIZE, hashCode, comparator);
    m_insertar(m, "Hi", "Bye");
    printf("Hi: %s", m_recuperar(m, "Hi"));
    return 0;
}
