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
    // check for memory errors
    if(*m == NULL){
        exit(MAP_ERROR_MEMORIA);
    }
    (*m)->longitud_tabla = capacity;
    (*m)->cantidad_elementos = 0;
    (*m)->hash_code = fHash;
    (*m)->comparador = fComparacion;
    // create the bucket array.
    tLista hash_table [capacity];
    // insert empty lists in the map.
    for(int i = 0; i < capacity; i++){
        tLista list = NULL;
        crear_lista(&list);
        hash_table[i] = list;
    }
    (*m)->tabla_hash = hash_table;
}

void rehash(tMapeo * m){
    // reference the old list.
    tLista old_array = *(*m)->tabla_hash;
    unsigned int old_length = (*m)->longitud_tabla;
    (*m)->longitud_tabla *= 2;
    (*m)->cantidad_elementos = 0;
    // create the new bucket array.
    tLista hash_table [(*m)->longitud_tabla];
    (*m)->tabla_hash = hash_table;
    // iterate through the old list to insert every entry into the new map.
    for(int i = 0; i < old_length; i++){
        tEntrada entry = old_array[i].elemento;
        m_insertar(*m, entry->clave, entry->valor);
    }
}

tValor m_insertar(tMapeo m, tClave c, tValor v){
    // check if we will meet the hashing requirements.
    if(m->cantidad_elementos + 1 >= m->longitud_tabla * .75){
        // we don't, rehash the map.
        rehash(&m);
    }
    tValor old_v = NULL;
    tEntrada listed_entry;
    tEntrada entry;
    unsigned int index = m->hash_code(c);
    unsigned int replaced = 0;
    tLista indexed_list = *(m->tabla_hash + index);
    // look for the key in the list.
    // if the key is found in the list, then
    //      replace the its value
    // else
    //      insert a new entry at the end of the list.
    tPosicion indexed_list_pos = l_primera(indexed_list);
    tPosicion indexed_list_last = l_ultima(indexed_list);
    unsigned int loop = indexed_list_pos != NULL;
    while(loop){
        // determine if we will loop after we're done.
        loop = indexed_list_pos != indexed_list_last;
        // get the entry.
        listed_entry = indexed_list_pos->elemento;
        // compare the keys.
        if(m->comparador(listed_entry->clave, c)){
            // the key is the same, replace the value.
            // get the old value to return.
            old_v = listed_entry->valor;
            listed_entry->valor = v;
            replaced = 1;
            // deliberately stop looping.
            loop = 0;
        } else if(loop){
            // we determined we should loop, so let's get the next position.
            indexed_list_pos = l_siguiente(indexed_list, indexed_list_pos);
        }
    }
    // if we didn't replace a value, we should insert an entry with the new key and value at the end of the indexed list.
    if(!replaced){
        entry = (tEntrada) malloc(sizeof(struct entrada));
        // check for memory errors.
        if(entry == NULL){
            exit(MAP_ERROR_MEMORIA);
        }
        entry->clave = c;
        entry->valor = v;
        l_insertar(indexed_list, l_fin(indexed_list), entry);
        m->cantidad_elementos++;
    }
    return old_v;
}

void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    tPosicion indexed_list_pos;
    tPosicion indexed_list_last;
    tEntrada listed_entry;
    unsigned int index = m->hash_code(c);
    unsigned int loop = 1;
    tLista indexed_list = m->tabla_hash[index]->elemento;
    // look for the key in the list.
    // if the key is found in the list, then
    //      delete the entry containing it
    // else
    //      do nothing
    indexed_list_pos = l_primera(indexed_list);
    indexed_list_last = l_ultima(indexed_list);
    while(loop){
        // determine if we will loop after we're done.
        loop = indexed_list_pos != indexed_list_last;
        // get the entry.
        listed_entry = indexed_list_pos->elemento;
        // compare the keys.
        if(m->comparador(listed_entry->clave, c)){
            // the key is the same, delete the entry.
            // delete the key and value using the parameterized functions.
            fEliminarC(listed_entry->clave);
            fEliminarV(listed_entry->valor);
            // delete the entry freeing the memory space allocated for it.
            free(listed_entry);
            listed_entry = NULL;
            // decrease the size of the map.
            m->cantidad_elementos--;
            // deliberately stop looping.
            loop = 0;
        } else if(loop){
            // we determined we should loop, so let's get the next position.
            indexed_list_pos = l_siguiente(indexed_list, indexed_list_pos);
        }
    }
}

void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    tLista indexed_list;
    tPosicion indexed_pos;
    tEntrada listed_entry;
    for(int i = 0; i < (*m)->longitud_tabla; i++){
        indexed_list = (*m)->tabla_hash[i];
        indexed_pos = l_primera(indexed_list);
        // delete the first entry until the first entry in the list is null.
        while(indexed_pos != NULL){
            listed_entry = indexed_pos->elemento;
            // delete the key and value of this entry using the parameterized functions.
            fEliminarC(listed_entry->clave);
            fEliminarV(listed_entry->valor);
            // delete the entry freeing the memory space allocated for it.
            l_eliminar(indexed_list, indexed_pos, &fEliminar);
            // get the "next" position (it's actually the first in the list).
            indexed_pos = l_primera(indexed_list);
        }
        // destroy the indexed list
        l_destruir(&indexed_list, &fEliminar);
    }
    (*m)->tabla_hash = NULL;
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
    tPosicion indexed_list_pos;
    tPosicion indexed_list_last;
    tEntrada listed_entry;
    unsigned int index = m->hash_code(c);
    unsigned int loop = 1;
    tLista indexed_list = m->tabla_hash[index]->elemento;
    // look for the key in the list.
    // if the key is found in the list, then
    //      get the value to return it
    // else
    //      do nothing
    indexed_list_pos = l_primera(indexed_list);
    indexed_list_last = l_ultima(indexed_list);
    while(loop){
        // determine if we will loop after we're done.
        loop = indexed_list_pos != indexed_list_last;
        // get the entry.
        listed_entry = indexed_list_pos->elemento;
        // compare the keys.
        if(m->comparador(listed_entry->clave, c)){
            // the key is the same, get the value.
            v = listed_entry->valor;
            // deliberately stop looping.
            loop = 0;
        } else if(loop){
            // we determined we should loop, so let's get the next position.
            indexed_list_pos = l_siguiente(indexed_list, indexed_list_pos);
        }
    }
    return v;
}