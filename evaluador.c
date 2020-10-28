/*
 * Evaluador (implementation)
 *
 * @author Juan Ignacio Fabrega
 * @author Franco Caramelli Poeta
 * @author Ezequiel Ramírez Beltrán
 * 10/28/20
 */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "evaluador.h"
#include "mapeo/mapeo.h"

/**
 * Deletes the key
 * @param k the key
 */
void fDeleteK(void * k){
    free(k);
    k = NULL;
}

/**
 * Deletes the value
 * @param v the value
 */
void fDeleteV(void * v){
    free(v);
    v = NULL;
}

/**
 * Returns the length of the key.
 * @param key the key
 * @return the length of the key.
 */
int hashCode(void * key){
    return (int) strlen(key);
}

/**
 * Compares k1 to k2
 * @param k1 the first key
 * @param k2 the second key
 * @return 0 if k1 is different than k2, anything else if not.
 */
int comparator(void * k1, void * k2){
    return strcmp(k1, k2) == 0;
}

/**
 * Transforms word into uppercase.
 * @param word the word to transform.
 */
void to_upper(char * word){
    for(int i = 0; i < BUFSIZE; i++){
        word[i] = (char) toupper(word[i]);
    }
}

/**
 * Inserts all the words found in the given file into a map and then allows the user to check how many times a word appears.
 * @param f the text file
 */
void evaluador(FILE * f){
    tMapeo m = NULL;
    crear_mapeo(&m, 101, hashCode, comparator);
    char line[BUFSIZE];
    char delimiters [] = " !,.;:ºª|/·#@$%&()?¿¡_-{}[]+*^<>\"\\\n";
    char * k = NULL;
    int * old_v = NULL;
    char * key = NULL;
    int * value = NULL;
    while(fgets(line, BUFSIZE-1, f) != NULL){
        to_upper(line);
        k = malloc(BUFSIZE);
        if(k == NULL){
            printf("There was a memory error when creating the key variable.");
            exit(-2);
        }
        key = strtok(line, delimiters);
        while(key != NULL){
            old_v = m_recuperar(m, key);
            strcpy(k, key);
            if(old_v == NULL){
                value = malloc(sizeof(int));
                if(value == NULL){
                    printf("There was a memory error when creating the value variable.");
                    exit(-2);
                }
                *value = 1;
                m_insertar(m, k, value);
            } else {
                // We thought of modifying the old_v doing "(*old_v)++;"
                // But we realized that the map already replaces values and, therefore, it would be better to use such property.
                value = malloc(sizeof(int));
                if(value == NULL){
                    printf("There was a memory error when creating the value variable.");
                    exit(-2);
                }
                *value = (*old_v) + 1;
                m_insertar(m, key, value);
                // free the memory allocated for the key and old value
                fDeleteK(k);
                fDeleteV(old_v);
            }
            k = malloc(BUFSIZE);
            if(k == NULL){
                printf("There was a memory error when creating the key variable.");
                exit(-2);
            }
            key = strtok(NULL, delimiters);
        }
    }
    fclose(f);
    char word [BUFSIZE];
    int choice = 0;
    int * word_count = NULL;
    while(choice != 2){
        printf("Choose one of the following options:\n");
        printf("1 - Choose a word to compute.\n");
        printf("2 - Exit the program.\n");
        printf("Your choice: ");
        scanf("%d", &choice);
        if(choice == 1){
            printf("Your word: ");
            scanf("%s", word);
            to_upper(word);
            word_count = m_recuperar(m, word);
            if(word_count == NULL){
                printf("The word \"%s\" was not found.\n\n", word);
            } else {
                printf("Word count for \"%s\": %d.\n\n", word, *word_count);
            }
        } else if(choice == 2){
            printf("\nExiting program...\n");
        } else {
            printf("Invalid choice.\n\n");
        }
    }
    m_destruir(&m, &fDeleteK, &fDeleteV);
    if(m != NULL){
        printf("There was an error destroying the evaluator.");
        exit(-2);
    }
}

