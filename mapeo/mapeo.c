#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

/**
 * Elimina el elemento e
 * @param e el elemento
 */
void fEliminar(tElemento e){
    free(e);
    e = NULL;
}

void crear_mapeo(tMapeo * m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)){
    int capacity = max(ci, 10);
    *m = (tMapeo) malloc(sizeof(struct mapeo));
    if(*m == NULL){
        exit(MAP_ERROR_MEMORIA);
    }
    (*m)->longitud_tabla = capacity;
    (*m)->cantidad_elementos = 0;
    (*m)->hash_code = fHash;
    (*m)->comparador = fComparacion;
    tLista hash_table;
    crear_lista(&hash_table);
    (*m)->tabla_hash = &hash_table;
    for(int i = 0; i < capacity; i++){
        tLista list;
        crear_lista(&list);
        l_insertar(hash_table, l_ultima(hash_table), list);
    }
}

tValor m_insertar(tMapeo m, tClave c, tValor v){
    tValor old_v = NULL;
    int index = m->hash_code(c);
    int indexed_list_length;
    tLista indexed_list;
    tPosicion indexed_list_position;
    tPosicion pos = l_primera(*m->tabla_hash);
    tPosicion pos_last = l_ultima(*m->tabla_hash);
    tEntrada entry = (tEntrada) malloc(sizeof(struct entrada));
    if(entry == NULL){
        exit(MAP_ERROR_MEMORIA);
    }
    entry->clave = c;
    entry->valor = v;
    for(int i = 0, found = 0; !found && i < m->longitud_tabla; i++){
        found = i == index;
        if(found){
            indexed_list = l_recuperar(*m->tabla_hash, pos);
            indexed_list_length = l_longitud(indexed_list);
            indexed_list_position = l_primera(indexed_list);
            for(int j = 0, found_indexed = 0; !found_indexed && j < indexed_list_length; j++){
                tEntrada indexed_entry = l_recuperar(indexed_list, indexed_list_position);
                found_indexed = m->comparador(indexed_entry->clave, c);
                if(found_indexed){
                    old_v = indexed_entry->valor;
                    l_eliminar(indexed_list, indexed_list_position, fEliminar);
                } else {
                    indexed_list_position = l_siguiente(indexed_list, indexed_list_position);
                }
            }
            l_insertar(indexed_list, indexed_list_position, entry); // For some reason, this insertion resets the hash table to only 1 cell instead of 101.
            m->cantidad_elementos++;
        } else if(pos != pos_last){
            pos = l_siguiente(*m->tabla_hash, pos);
        }
    }
    return old_v;
}

void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    int index = m->hash_code(c);
    tPosicion pos = l_primera(*m->tabla_hash);
    tPosicion pos_last = l_ultima(*m->tabla_hash);
    tLista indexed_list;
    int indexed_list_length;
    tPosicion indexed_list_position;
    for(int i = 0, found = 0; !found && i < m->longitud_tabla; i++){
        found = i == index;
        if(found){
            indexed_list = l_recuperar(*m->tabla_hash, pos);
            indexed_list_length = l_longitud(indexed_list);
            indexed_list_position = l_primera(indexed_list);
            for(int j = 0, found_indexed = 0; !found_indexed && j < indexed_list_length; j++){
                tEntrada indexed_entry = l_recuperar(indexed_list, indexed_list_position);
                found_indexed = m->comparador(indexed_entry->clave, c);
                if(found_indexed){
                    fEliminarC(indexed_entry->clave);
                    fEliminarV(indexed_entry->valor);
                    l_eliminar(indexed_list, indexed_list_position, fEliminar);
                } else {
                    indexed_list_position = l_siguiente(indexed_list, indexed_list_position);
                }
            }
        } else if(pos != pos_last){
            pos = l_siguiente(*m->tabla_hash, pos);
        }
    }
}

void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    tPosicion pos = l_primera(*(*m)->tabla_hash);
    tLista indexed_list;
    tEntrada indexed_entry;
    tPosicion indexed_list_position;
    while(pos != NULL){
        indexed_list = l_recuperar(*(*m)->tabla_hash, pos);
        indexed_list_position = l_primera(indexed_list);
        while(indexed_list_position != NULL){
            indexed_entry = l_recuperar(indexed_list, indexed_list_position);
            fEliminarC(indexed_entry->clave);
            fEliminarV(indexed_entry->valor);
            l_eliminar(indexed_list, indexed_list_position, fEliminar);
            indexed_list_position = l_primera(indexed_list);
        }
        pos = l_siguiente(*(*m)->tabla_hash, pos);
    }
    l_destruir((*m)->tabla_hash, fEliminar);
    (*m)->longitud_tabla = 0;
    (*m)->cantidad_elementos = 0;
    (*m)->hash_code = NULL;
    (*m)->comparador = NULL;
    (*m)->tabla_hash = NULL;
    free(*m);
    *m = NULL;
}

tValor m_recuperar(tMapeo m, tClave c){
    tValor v = NULL;
    int index = m->hash_code(c);
    tPosicion pos = l_primera(*m->tabla_hash);
    tLista indexed_list;
    int indexed_list_length;
    tPosicion indexed_list_position;
    for(int i = 0, found = 0; !found && i < m->longitud_tabla; i++){
        found = i == index;
        if(found){
            indexed_list = l_recuperar(*m->tabla_hash, pos);
            indexed_list_length = l_longitud(indexed_list);
            indexed_list_position = l_primera(indexed_list);
            for(int j = 0, found_indexed = 0; !found_indexed && j < indexed_list_length; j++){
                tEntrada indexed_entry = l_recuperar(indexed_list, indexed_list_position);
                found_indexed = m->comparador(indexed_entry->clave, c);
                if(found_indexed){
                    v = indexed_entry->valor;
                } else {
                    indexed_list_position = l_siguiente(indexed_list, indexed_list_position);
                }
            }
        } else if(pos != NULL){
            pos = l_siguiente(*m->tabla_hash, pos);
        }
    }
    return v;
}