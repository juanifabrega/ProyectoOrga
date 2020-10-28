/*
 * Evaluador (header)
 *
 * @author Juan Ignacio Fabrega
 * @author Franco Caramelli Poeta
 * @author Ezequiel Ramírez Beltrán
 * 10/28/20
 */

#ifndef PROYECTOORGA_EVALUADOR_H
#define PROYECTOORGA_EVALUADOR_H

// The length of the longest recognized systematic name (for the protein titin).
// This also accounts for 4,218 of the longest word in the English dictionary in a single line (45 letters).
// 6,327 times the longest word in the Spanish dictionary (30 letters).
#define BUFSIZE 189819

extern void evaluador(FILE * f);

#endif //PROYECTOORGA_EVALUADOR_H
