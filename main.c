/*
 * Main file
 *
 * @author Juan Ignacio Fabrega
 * @author Franco Caramelli Poeta
 * @author Ezequiel Ramírez Beltrán
 * 10/28/20
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "evaluador.h"

/**
 * Main program
 * @param argc the count of input arguments
 * @param argv a pointer to all the input arguments
 * @return 0 if the program executed normally, or -1 if there was an error reading the input file.
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
    evaluador(f);
    printf("Finished successfully.\n");
    return 0;
}
