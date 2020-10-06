#include <stdlib.h>
#include "lista.h"

void crear_lista(tLista* l){
    *l = (tLista) malloc(sizeof(struct celda));
    if(l == NULL){
        exit(LST_ERROR_MEMORIA);
    }
    (*l)->elemento = NULL;
    (*l)->siguiente = NULL;
}

void l_insertar(tLista l, tPosicion p, tElemento e){
    tPosicion nuevaCelda = (tPosicion) malloc(sizeof(struct celda));
    if(nuevaCelda == NULL){
        exit(LST_ERROR_MEMORIA);
    }
    nuevaCelda->elemento = e;
    nuevaCelda->siguiente = p->siguiente;
    p->siguiente = nuevaCelda;
}

void l_eliminar(tLista l, tPosicion p, void(* fEliminar)(tElemento)){
    tPosicion aux;
    if(p->siguiente == NULL){
        exit(LST_POSICION_INVALIDA);
    }
    fEliminar(p->siguiente->elemento);
    aux = p->siguiente;
    p->siguiente = p->siguiente->siguiente;
    aux->siguiente = NULL;
    free(aux);
    aux = NULL;
}

void l_destruirAux(tPosicion p, void(* fEliminar)(tElemento)){
    tPosicion aux;
    if(p->siguiente->siguiente != NULL){
        l_destruirAux(p->siguiente, fEliminar);
    }
    fEliminar(p->siguiente->elemento);
    aux = p->siguiente;
    p->siguiente = p->siguiente->siguiente;
    aux->siguiente = NULL;
    free(aux);
    aux = NULL;
}

void l_destruir(tLista* l, void(* fEliminar)(tElemento)){
    tPosicion primera = l_primera(*l);
    if(primera->siguiente != NULL){
        l_destruirAux(primera, fEliminar);
    }
    free(*l);
    *l = NULL;
}

tElemento l_recuperar(tLista l, tPosicion p){
    if(p->siguiente == NULL){
        exit(LST_POSICION_INVALIDA);
    }
    return p->siguiente->elemento;
}

tPosicion l_primera(tLista l){
    return l;
}

tPosicion l_siguiente(tLista l, tPosicion p){
    if(p->siguiente == NULL){
        exit(LST_NO_EXISTE_SIGUIENTE);
    }
    return p->siguiente;

}

tPosicion l_anterior(tLista l, tPosicion p){
    tPosicion pos;
    if(p == l){
        exit(LST_NO_EXISTE_ANTERIOR);
    }
    pos = l_primera(l);
    while(pos->siguiente != p){
        pos = pos->siguiente;
    }
    return pos;
}

tPosicion l_ultima(tLista l){
    tPosicion primera = l_primera(l);
    tPosicion pos;
    if(primera->siguiente == NULL || primera->siguiente->siguiente == NULL ){
        return l;
    }
    pos = primera;
    while(pos->siguiente->siguiente != NULL){
        pos = pos->siguiente;
    }
    return pos;
}

tPosicion l_fin(tLista l){
    if(l_primera(l)->siguiente == NULL){
        return l;
    }
    return l_ultima(l)->siguiente;
}

int l_longitud(tLista l){
    tPosicion pos = l_primera(l);
    int longitud;
    if(pos->siguiente == NULL){
        return 0;
    }
    longitud = 1;
    pos = pos->siguiente;
    while(pos->siguiente != NULL){
        longitud++;
        pos = pos->siguiente;
    }
    return longitud;
}