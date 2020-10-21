#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "mapeo/mapeo.h"

// The length of the longest recognized systematic name (for the protein titin).
// This also accounts for 4,218 of the longest word in the English dictionary in a single line (45 letters).
// 6,327 times the longest word in the Spanish dictionary (30 letters).
#define BUFSIZE 189819

/**
 * Deletes the key
 * @param k the key
 */
void fDeleteK(void * k){
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

int hashCode(void * key){
    return (int) strlen(key);
}

int comparator(void * k1, void * k2){
    return strcmp(k1, k2) == 0;
}

/**
 * Main program
 * @param argc the count of input arguments
 * @param argv a pointer to all the input arguments
 * @return 0 if the program executed normally, -1 if there was an error reading the input file, or -2 if there was an error executing the program.
 */
int main(int argc, char * argv []) {
    if(argc < 2){
        printf("There are not enough arguments. Please, pass the path of the file to read as an argument.");
        return -1;
    }
    FILE * f = fopen(*(argv+1), "r");
    if(f == NULL){
        printf("There was an error reading the text file.");
        return -1;
    }
    tMapeo m = NULL;
    crear_mapeo(&m, 101, hashCode, comparator);
    char buff[BUFSIZE];
    char buffcpy[BUFSIZE];
    char * delimiter = " ";
    char * key = NULL;
    int * old_v = NULL;
    int * value = NULL;
    while(fgets(buff, BUFSIZE-1, f) != NULL){
        strcpy(buffcpy, buff);
        key = strtok(buffcpy, delimiter);
        while(key != NULL){
            old_v = m_recuperar(m, key);
            if(old_v == NULL){
                value = malloc(sizeof(int));
                *value = 1;
            } else {
                value = malloc(sizeof(int));
                *value = (*old_v)+1;
                fDeleteV(old_v);
            }
            m_insertar(m, key, value);
            key = strtok(NULL, delimiter);
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
        return -2;
    }
    printf("Finished successfully.\n");
    return 0;
}
