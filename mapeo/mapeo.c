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

/**
 * No elimina el elemento e
 * @param e el elemento
 */
void fNoEliminar(tElemento e){

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
        l_insertar(hash_table, l_fin(hash_table), NULL);
    }
}

void rehash(tMapeo * m){
    // reference the old list.
    tLista old_list = *(*m)->tabla_hash;
    int old_length = l_longitud(old_list);
    tPosicion old_pos = l_primera(old_list);
    // create the new list.
    tLista hash_table;
    crear_lista(&hash_table);
    // assign the new list and information.
    (*m)->tabla_hash = &hash_table;
    (*m)->longitud_tabla *= 2;
    (*m)->cantidad_elementos = 0;
    // iterate through the old list to insert every entry into the new map.
    for(int i = 0; i < old_length; i++){
        tEntrada entry = l_recuperar(old_list, old_pos);
        // if the entry is not null, insert it into the map.
        if(entry != NULL){
            m_insertar(*m, entry->clave, entry->valor);
        }
    }
    // destroy the old list freeing the memory allocated for it.
    l_destruir(&old_list, &fEliminar);
}

tValor m_insertar(tMapeo m, tClave c, tValor v){
    if(m->cantidad_elementos >= m->longitud_tabla * .75) {
        rehash(&m);
    }
    tValor old_v = NULL;
    int index = m->hash_code(c);
    tPosicion pos = l_primera(*m->tabla_hash);
    tPosicion pos_last = l_ultima(*m->tabla_hash);
    tEntrada indexed_entry;
    tEntrada entry = (tEntrada) malloc(sizeof(struct entrada));
    if(entry == NULL){
        exit(MAP_ERROR_MEMORIA);
    }
    entry->clave = c;
    entry->valor = v;
    for(int i = 0, found = 0; !found && i < m->longitud_tabla; i++){
        found = i == index;
        if(found){
            indexed_entry = l_recuperar(*m->tabla_hash, pos);
            if(indexed_entry == NULL){
                // the slot is empty, insert the new entry.
                l_insertar(*m->tabla_hash, pos, entry);
                // delete the position but do not delete the element because it is NULL and no memory was allocated.
                l_eliminar(*m->tabla_hash, l_siguiente(*m->tabla_hash, pos), &fNoEliminar);
            } else {
                // the slot is not empty, check if the key is the same or find a new slot using linear probing.
                if(m->comparador(indexed_entry->clave, c)){
                    // the key is the same, replace the value.
                    // get the old value to return.
                    old_v = indexed_entry->valor;
                    indexed_entry->valor = v;
                } else {
                    // the key is not the same, find a new slot using linear probing
                    int j = index + 1;
                    int index_found;
                    pos = l_siguiente(*m->tabla_hash, pos);
                    do {
                        indexed_entry = l_recuperar(*m->tabla_hash, pos);
                        index_found = indexed_entry == NULL;
                        if(!index_found){
                            j = (j + 1) % (int) m->longitud_tabla;
                            if(pos == pos_last){
                                // reached the end of the table, keep looking from the beginning of the table
                                pos = l_primera(*m->tabla_hash);
                            } else {
                                // set up the next position to check
                                pos = l_siguiente(*m->tabla_hash, pos);
                            }
                        }
                    } while (!index_found && index != j);
                    // found an empty slot, insert the new entry.
                    l_insertar(*m->tabla_hash, pos, entry);
                    // delete the position but do not delete the element because it is NULL and no memory was allocated.
                    l_eliminar(*m->tabla_hash, l_siguiente(*m->tabla_hash, pos), &fNoEliminar);
                }
            }
            m->cantidad_elementos++;
        } else if(pos != pos_last){
            pos = l_siguiente(*m->tabla_hash, pos); // TODO: fix - this assignment changes the memory pointer for *m->tabla_hash
        }
    }
    //printf("Hash table length after inserting: %d\n", l_longitud(*m->tabla_hash));
    return old_v;
}

void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    int index = m->hash_code(c);
    tPosicion pos = l_primera(*m->tabla_hash);
    tPosicion pos_last = l_ultima(*m->tabla_hash);
    tEntrada indexed_entry;
    for(int i = 0, found = 0; !found && i < m->longitud_tabla; i++){
        found = i == index;
        if(found){
            indexed_entry = l_recuperar(*m->tabla_hash, pos);
            if(indexed_entry != NULL){
                // the slot is not empty, therefore the key exists. Check if the key is the same or find it using linear probing.
                if(!m->comparador(indexed_entry->clave, c)){
                    // the key is not the same, find the correct slot using linear probing.
                    int j = index + 1;
                    int index_found;
                    pos = l_siguiente(*m->tabla_hash, pos);
                    do {
                        indexed_entry = l_recuperar(*m->tabla_hash, pos);
                        index_found = indexed_entry != NULL && m->comparador(indexed_entry->clave, c);
                        if(!index_found){
                            j = (j + 1) % (int) m->longitud_tabla;
                            if(pos == pos_last){
                                // reached the end of the table, keep looking from the beginning of the table.
                                pos = l_primera(*m->tabla_hash);
                            } else {
                                // set up the next position to check.
                                pos = l_siguiente(*m->tabla_hash, pos);
                            }
                        }
                    } while (!index_found && index != j);
                }
                // insert a NULL entry to replace the one to be deleted.
                l_insertar(*m->tabla_hash, pos, NULL);
                // delete the position next to the current position, which has the key we were looking for, so that the key is fully deleted from the map.
                l_eliminar(*m->tabla_hash, l_siguiente(*m->tabla_hash, pos), &fEliminar);
            }
        } else if(pos != pos_last){
            pos = l_siguiente(*m->tabla_hash, pos);
        }
    }
}

void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    tEntrada indexed_entry;
    tPosicion pos = l_primera(*(*m)->tabla_hash);
    while(pos != NULL){
        indexed_entry = l_recuperar(*(*m)->tabla_hash, pos);
        if(indexed_entry != NULL){
            // delete the key and value using the parameterized functions.
            fEliminarC(indexed_entry->clave);
            fEliminarV(indexed_entry->valor);
            // delete the entry freeing the memory space allocated for it.
            l_eliminar(*(*m)->tabla_hash, pos, &fEliminar);
        } else {
            // delete the cell but not the entry because it is NULL and no memory was allocated.
            l_eliminar(*(*m)->tabla_hash, pos, &fNoEliminar);
        }
        pos = l_primera(*(*m)->tabla_hash);
    }
    l_destruir((*m)->tabla_hash, &fEliminar);
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
    tPosicion pos_last = l_ultima(*m->tabla_hash);
    tEntrada indexed_entry;
    for(int i = 0, found = 0; !found && i < m->longitud_tabla; i++){
        found = i == index;
        if(found){
            indexed_entry = l_recuperar(*m->tabla_hash, pos);
            if(indexed_entry != NULL){
                // the slot is not empty, therefore the key exists. Check if the key is the same or find it using linear probing.
                if(!m->comparador(indexed_entry->clave, c)){
                    // the key is not the same, find the correct slot using linear probing
                    int j = index + 1;
                    int index_found;
                    pos = l_siguiente(*m->tabla_hash, pos);
                    do {
                        indexed_entry = l_recuperar(*m->tabla_hash, pos);
                        index_found = indexed_entry != NULL && m->comparador(indexed_entry->clave, c);
                        if(!index_found){
                            j = (j + 1) % (int) m->longitud_tabla;
                            if(pos == pos_last){
                                // reached the end of the table, keep looking from the beginning of the table
                                pos = l_primera(*m->tabla_hash);
                            } else {
                                // set up the next position to check
                                pos = l_siguiente(*m->tabla_hash, pos);
                            }
                        }
                    } while (!index_found && index != j);
                }
                // get the found value
                v = indexed_entry->valor;
            }
        } else if(pos != pos_last){
            pos = l_siguiente(*m->tabla_hash, pos);
        }
    }
    return v;
}

void m_show(tMapeo m){
    tPosicion pos = l_primera(*m->tabla_hash);
    tPosicion pos_last = l_ultima(*m->tabla_hash);
    tEntrada indexed_entry;
    for(int i = 0; i < m->longitud_tabla; i++){
        indexed_entry = l_recuperar(*m->tabla_hash, pos);
        if(indexed_entry != NULL){
            printf("Index: %i.\nKey: %s.\nValue: %s.\n\n", i, indexed_entry->clave, indexed_entry->valor);
        }
        if(pos != pos_last){
            pos = l_siguiente(*m->tabla_hash, pos);
        }
    }
}