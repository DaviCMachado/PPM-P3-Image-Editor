#ifndef AUX_H
#define AUX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define efeitos 9

typedef struct esc
{
    int r, g, b;

}pixel;


void filtroV (pixel **, int, int, int, int, int ); 
void filtroA (pixel **, int, int, int, int, int ); 
void rotaciona (pixel ***, int *, int *); 
void espelho (pixel **, int, int ); 
pixel** recorta (pixel **, int *, int *, int, int, int, int);
void colar (pixel ***, int *, int *, int);
void escala (pixel ***, int*, int*, double, double ); 
int colar_Imagem_Externa (pixel*** imagem, int* linhas, int* colunas, char* arquivoImagem);
void libera (pixel **colors, int linhas);





#endif