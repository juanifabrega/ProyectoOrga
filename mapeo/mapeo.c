#include <stdlib.h>
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
 * Gets the index of a hashed key
 * @param hash_code the function to use for key hashing
 * @param k the key being hashed
 * @param l the length of the hash table
 * @return an unsign integer that represents the index based on the length of the hash table
 */
unsigned int hash_index(int (*hash_code)(void *), tClave k, unsigned int l){
    return hash_code(k) % l;
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
    // check for memory errors.
    if(*m == NULL){
        exit(MAP_ERROR_MEMORIA);
    }
    // set the initial values.
    (*m)->longitud_tabla = capacity;
    (*m)->cantidad_elementos = 0;
    (*m)->hash_code = fHash;
    (*m)->comparador = fComparacion;
    // create the bucket array.
    (*m)->tabla_hash = (tLista *) malloc(capacity * sizeof(tLista));
    if((*m)->tabla_hash == NULL){
        exit(MAP_ERROR_MEMORIA);
    }
    // insert empty lists in the map.
    for(int i = 0; i < capacity; i++){
        crear_lista(&(*m)->tabla_hash[i]);
    }
}

void rehash(tMapeo * m){
    // reference the old array.
    tLista * old_hash_table = (*m)->tabla_hash;
    unsigned int old_length = (*m)->longitud_tabla;
    (*m)->longitud_tabla *= 2;
    // create the new bucket array.
    tLista * hash_table = malloc((*m)->longitud_tabla * sizeof(tLista));
    tLista old_list;
    unsigned int old_list_length;
    tPosicion old_list_pos;
    tEntrada old_list_entry;
    unsigned int index;
    // iterate through the old array to insert every entry into the new array.
    for(int i = 0; i < old_length; i++){
        // insert the entries
        old_list = *(old_hash_table + i);
        old_list_length = l_longitud(old_list);
        if(old_list_length > 0){
            old_list_pos = l_primera(old_list);
            while(old_list_length > 0){
                old_list_entry = l_recuperar(old_list, old_list_pos);
                index = hash_index((*m)->hash_code, old_list_entry->clave, (*m)->longitud_tabla);
                l_insertar(*(hash_table + index), l_primera(*(hash_table + index)), old_list_entry);
                old_list_pos = l_siguiente(old_list, old_list_pos);
                old_list_length--;
            }
        }
    }
    free(old_hash_table);
    old_hash_table = NULL;
    (*m)->tabla_hash = hash_table;
}

tValor m_insertar(tMapeo m, tClave c, tValor v){
    // check if we will meet the hashing requirements.
    int needs_rehash = m->cantidad_elementos + 1 >= m->longitud_tabla * .75;
    if(needs_rehash){
        // we won't, rehash the map.
        rehash(&m);
    }
    tValor old_v = NULL;
    tEntrada listed_entry;
    tEntrada entry;
    unsigned int loop;
    unsigned int index = hash_index(m->hash_code, c, m->longitud_tabla);
    unsigned int replaced = 0;
    tLista indexed_list = *(m->tabla_hash + index);
    unsigned int size = l_longitud(indexed_list);
    if(size > 0){
        // look for the key in the list.
        // if the key is found in the list, then
        //      replace the its value
        // else
        //      insert a new entry at the end of the list.
        tPosicion indexed_list_pos = l_primera(indexed_list);
        tPosicion indexed_list_last = l_ultima(indexed_list);
        loop = indexed_list_pos != NULL;
        while(loop){
            // get the entry.
            listed_entry = l_recuperar(indexed_list, indexed_list_pos);
            // determine if we will loop after we're done.
            loop = indexed_list_pos != indexed_list_last && listed_entry != NULL;
            // compare the keys.
            if(listed_entry != NULL && m->comparador(listed_entry->clave, c)){
                // the key is the same, replace the value.
                // get the old value to return.
                old_v = listed_entry->valor;
                listed_entry->valor = v;
                replaced = 1;
                // deliberately stop looping.
                loop = 0;
            }
            if(loop){
                // we determined we should loop, so let's get the next position.
                indexed_list_pos = l_siguiente(indexed_list, indexed_list_pos);
            }
        }
    }
    // if we didn't replace a value, we should insert an entry with the new key and value at the beginning of the indexed list.
    if(!replaced){
        entry = (tEntrada) malloc(sizeof(struct entrada));
        // check for memory errors.
        if(entry == NULL){
            exit(MAP_ERROR_MEMORIA);
        }
        // add the key and value to the entry.
        entry->clave = c;
        entry->valor = v;
        // inset the entry.
        l_insertar(indexed_list, l_primera(indexed_list), entry);
        // increase the size of the map.
        m->cantidad_elementos++;
    }
    return old_v;
}

void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    tPosicion indexed_list_pos;
    tPosicion indexed_list_last;
    tEntrada listed_entry;
    unsigned int loop;
    unsigned int index = hash_index(m->hash_code, c, m->longitud_tabla);
    tLista indexed_list = m->tabla_hash[index];
    if(l_longitud(indexed_list) > 0){
        // look for the key in the list.
        // if the key is found in the list, then
        //      delete the entry containing it
        // else
        //      do nothing
        indexed_list_pos = l_primera(indexed_list);
        indexed_list_last = l_ultima(indexed_list);
        loop = indexed_list_pos != NULL;
        while(loop){
            // get the entry.
            listed_entry = l_recuperar(indexed_list, indexed_list_pos);
            // determine if we will loop after we're done.
            loop = indexed_list_pos != indexed_list_last && listed_entry != NULL;
            // compare the keys.
            if(listed_entry != NULL && m->comparador(listed_entry->clave, c)){
                // the key is the same, delete the entry.
                // delete the key and value using the parameterized functions.
                fEliminarC(listed_entry->clave);
                fEliminarV(listed_entry->valor);
                // delete the entry from the list, freeing the memory allocated for it.
                l_eliminar(indexed_list, indexed_list_pos, &fEliminar);
                // decrease the size of the map.
                m->cantidad_elementos--;
                // deliberately stop looping.
                loop = 0;
            }
            if(loop){
                // we determined we should loop, so let's get the next position.
                indexed_list_pos = l_siguiente(indexed_list, indexed_list_pos);
            }
        }
    }
}

void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    tLista indexed_list;
    tPosicion indexed_list_pos;
    tEntrada listed_entry;
    unsigned int indexed_list_length;
    for(int i = 0; i < (*m)->longitud_tabla; i++){
        indexed_list = *((*m)->tabla_hash + i);
        indexed_list_length = l_longitud(indexed_list);
        if(indexed_list_length > 0){
            indexed_list_pos = l_primera(indexed_list);
            // delete the first entry until the first entry in the list is null.
            while(indexed_list_length > 0){
                listed_entry = l_recuperar(indexed_list, indexed_list_pos);
                // delete the key and value of this entry using the parameterized functions.
                fEliminarC(listed_entry->clave);
                fEliminarV(listed_entry->valor);
                // delete the entry freeing the memory space allocated for it.
                l_eliminar(indexed_list, indexed_list_pos, &fEliminar);
                // get the "next" position (it's actually the first in the list).
                indexed_list_pos = l_primera(indexed_list);
                indexed_list_length--;
            }
        }
        // destroy the indexed list
        l_destruir(&indexed_list, &fEliminar);
    }
    // null all variables
    free((*m)->tabla_hash);
    (*m)->tabla_hash = NULL;
    (*m)->longitud_tabla = 0;
    (*m)->cantidad_elementos = 0;
    (*m)->hash_code = NULL;
    (*m)->comparador = NULL;
    (*m)->tabla_hash = NULL;
    // free the memory allocated for the map
    free(*m);
    *m = NULL;
}

tValor m_recuperar(tMapeo m, tClave c){
    tValor v = NULL;
    tPosicion indexed_list_pos;
    tPosicion indexed_list_last;
    tEntrada listed_entry;
    unsigned int index = hash_index(m->hash_code, c, m->longitud_tabla);
    tLista indexed_list = *(m->tabla_hash + index);
    // look for the key in the list.
    // if the key is found in the list, then
    //      get the value to return it
    // else
    //      do nothing
    unsigned int l = l_longitud(indexed_list);
    if(l > 0){
        indexed_list_pos = l_primera(indexed_list);
        indexed_list_last = l_ultima(indexed_list);
        unsigned int loop = indexed_list_pos != NULL;
        while(loop){
            // get the entry.
            listed_entry = l_recuperar(indexed_list, indexed_list_pos);
            // determine if we will loop after we're done.
            loop = indexed_list_pos != indexed_list_last && listed_entry != NULL;
            // compare the keys.
            if(listed_entry != NULL && m->comparador(listed_entry->clave, c)){
                // the key is the same, get the value.
                v = listed_entry->valor;
                // deliberately stop looping.
                loop = 0;
            }
            if(loop){
                // we determined we should loop, so let's get the next position.
                indexed_list_pos = l_siguiente(indexed_list, indexed_list_pos);
            }
        }
    }
    return v;
}