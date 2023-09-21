#include "auxiliar.h"

pixel** recorte = NULL; // Variável global para armazenar o recorte
int recorteLinhas, recorteColunas;


void filtroV (pixel** novoArq, int x1, int y1, int x2, int y2, int max)
{
	for (int i = y1; i < y2; i++)
		for (int j = x1; j < x2; j++) 
		{
			if (novoArq[i][j].r <= (max * 2) /3)
				novoArq[i][j].r += (max * 1) /5;
		
			if (novoArq[i][j].g >= max /4)
				novoArq[i][j].g /= 2;
		
			if (novoArq[i][j].b >= max /4)
				novoArq[i][j].b /= 2;
		}
}

void filtroA (pixel** novoArq, int x1, int y1, int x2, int y2, int max)
{
	for (int i = y1; i < y2; i++)
		for (int j = x1; j < x2; j++) 
		{
			if (novoArq[i][j].b <= (max * 2) /3)
				novoArq[i][j].b += (max * 1) /5;
		
			if (novoArq[i][j].g >= max /4)
				novoArq[i][j].g /= 2;
		
			if (novoArq[i][j].r >= max /4)
				novoArq[i][j].r /= 2;
		}	
}

void rotaciona(pixel ***imagem, int *linhas, int *colunas) 
{
    int i, j;
    int novasLinhas = *colunas; // Troque as linhas e colunas
    int novasColunas = *linhas;

    // Aloque memória para a nova imagem após a rotação
    pixel **novaImagem = malloc(novasLinhas * sizeof(pixel *));
    
    for (i = 0; i < novasLinhas; ++i) 
        novaImagem[i] = malloc(novasColunas * sizeof(pixel));    

    // Realize a rotação
    for (i = 0; i < *linhas; ++i) 
        for (j = 0; j < *colunas; ++j) 
            novaImagem[j][*linhas - 1 - i] = (*imagem)[i][j];
        
    
    
    // Libere a memória da imagem original
    for (i = 0; i < *linhas; ++i) 
        free((*imagem)[i]);
    free(*imagem);

    // Atualize as dimensões da imagem com as novas dimensões
    *linhas = novasLinhas;
    *colunas = novasColunas;

    // Atualize a imagem original com a imagem rotacionada
    *imagem = novaImagem;
}

void espelho (pixel **imagem, int linhas, int colunas) 
{
    int i = 0, j = 0;
    pixel temp;
   

    for (i = 0; i < linhas; i++) 
    {
        for (j = 0; j < colunas / 2; j++) 
        {
            // Troca os pixels da esquerda com os da direita dentro da região de recorte
            temp = imagem[i][j];
            imagem[i][j] = imagem[i][colunas - 1 - j];
            imagem[i][colunas - 1 - j] = temp;
        }
    }
}



pixel** recorta(pixel** imagemOriginal, int* linhasOriginal, int* colunasOriginal, int x1, int y1, int x2, int y2)
{
    if (x1 >= 0 && y1 >= 0 && x2 < *colunasOriginal && y2 < *linhasOriginal) {
        int linhasRecortadas = y2 - y1 + 1;
        int colunasRecortadas = x2 - x1 + 1;

        // Aloca memória para a imagem recortada
        pixel** imagemRecortada = malloc(linhasRecortadas * sizeof(pixel*));
        for (int i = 0; i < linhasRecortadas; i++) {
            imagemRecortada[i] = malloc(colunasRecortadas * sizeof(pixel));
        }

        // Copia os pixels da imagem original para a imagem recortada
        for (int i = 0; i < linhasRecortadas; i++) {
            for (int j = 0; j < colunasRecortadas; j++) {
                imagemRecortada[i][j] = imagemOriginal[y1 + i][x1 + j];
            }
        }

        recorte = imagemRecortada;
        recorteLinhas = linhasRecortadas;
        recorteColunas = colunasRecortadas;
        return imagemRecortada;
    } 

    else 
    {
        printf("Coordenadas de recorte inválidas.\n");
        return NULL;
    }
}



void colar(pixel*** imagem, int* linhas, int* colunas, int max) 
{
    if (recorte == NULL) 
    {
        printf("Nenhum recorte disponível para colagem.\n");
        return;
    }

    int x, y;

    printf("Digite as coordenadas (x e y) onde deseja colar o recorte:\n");
    scanf(" %d %d", &x, &y);

    // Verifique se as coordenadas de colagem são válidas
    if (x < 0 || y < 0 || x + recorteLinhas > *linhas || y + recorteColunas > *colunas) 
    {
        printf("Coordenadas de colagem inválidas.\n");
        return;
    }

    // Cole o recorte na imagem
    for (int i = 0; i < recorteLinhas; i++) 
    {
        for (int j = 0; j < recorteColunas; j++) 
        {
            (*imagem)[x + i][y + j] = recorte[i][j];
        }
    }

    for (int i = 0; i < recorteLinhas; i++) 
    {
        free(recorte[i]);
    }
    free(recorte);

    recorte = NULL; // Redefina a variável de recorte para indicar que não há recorte ativo
}


int colar_Imagem_Externa (pixel*** imagem, int* linhas, int* colunas, char* arquivoImagem) 
{
    FILE* arquivo = fopen(arquivoImagem, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo de imagem externa.\n");
        return 0;
    }

    // leitura do arquivo
    int extColunas, extLinhas, max;
	char buffer[256];

    fgets(buffer, sizeof(buffer), arquivo);

	if (buffer[1] != '3')
	{
		printf("\nArquivo nao eh do tipo P3!\n\n");
		return 0;
	}
    
	do{
		fgets(buffer, 256, arquivo);
	}while(buffer[0] == '#');
	
	sscanf(buffer, "%d %d", &extColunas, &extLinhas, &max);
		
	do{
		fgets(buffer, 256, arquivo);
	}while(buffer[0] == '#');
	
	sscanf(buffer, "%d", &max);

    // usuario escolhe aonde colar

    int x, y, x_max, y_max;

    y_max = (*linhas) - extLinhas;
    x_max = (*colunas) - extColunas;

    printf("\nEscolha x e y aonda ira colar: (%d:%d eh o limite em que voce pode colar)\n", x_max, y_max);
    scanf(" %d %d", &x, &y);

    // Verifica se a imagem externa cabe na imagem atual
    if (extLinhas + y > *linhas || extColunas + x > *colunas) 
    {
        printf("A imagem externa não cabe na imagem atual.\n");
        fclose(arquivo);
        return 0;
    }

    // Lê e cola a imagem externa na imagem atual
    for (int i = y; i < y + extLinhas; i++) 
        for (int j = x; j < x + extColunas; j++)       
            fscanf(arquivo, "%d %d %d", &(*imagem)[i][j].r, &(*imagem)[i][j].g, &(*imagem)[i][j].b);
        
    


    fclose(arquivo);
    return 1;
}
   



void escala(pixel*** imagem, int* linhas, int* colunas, double fatorHorizontal, double fatorVertical) {
    int novaLargura = (int)(*colunas * fatorHorizontal);
    int novaAltura = (int)(*linhas * fatorVertical);

    // Crie uma nova matriz para armazenar a imagem ampliada
    pixel** imagemAmpliada = malloc(novaAltura * sizeof(pixel*));
    
    for (int i = 0; i < novaAltura; i++) 
        imagemAmpliada[i] = malloc(novaLargura * sizeof(pixel));
    

    for (int i = 0; i < novaAltura; i++) 
    {
        for (int j = 0; j < novaLargura; j++) 
        {
            double origX = (double)j / fatorHorizontal;
            double origY = (double)i / fatorVertical;

            int x1 = (int)origX;
            int y1 = (int)origY;
            int x2 = x1 + 1;
            int y2 = y1 + 1;

            // Verifique os limites para evitar acesso fora da matriz original
            if (x1 >= *colunas - 1) x1 = *colunas - 2;
            if (y1 >= *linhas - 1) y1 = *linhas - 2;
            if (x2 >= *colunas - 1) x2 = *colunas - 1;
            if (y2 >= *linhas - 1) y2 = *linhas - 1;

            double deltaX = origX - x1;
            double deltaY = origY - y1;

            // Interpolação bilinear
         
            double interpValueR = (1 - deltaX) * (1 - deltaY) * (*imagem)[y1][x1].r +
                    deltaX * (1 - deltaY) * (*imagem)[y1][x2].r +
                    (1 - deltaX) * deltaY * (*imagem)[y2][x1].r +
                    deltaX * deltaY * (*imagem)[y2][x2].r;
                    imagemAmpliada[i][j].r = (int)interpValueR;

            double interpValueG = (1 - deltaX) * (1 - deltaY) * (*imagem)[y1][x1].g +
                    deltaX * (1 - deltaY) * (*imagem)[y1][x2].g +
                    (1 - deltaX) * deltaY * (*imagem)[y2][x1].g +
                    deltaX * deltaY * (*imagem)[y2][x2].g;
                    imagemAmpliada[i][j].g = (int)interpValueG;

            double interpValueB = (1 - deltaX) * (1 - deltaY) * (*imagem)[y1][x1].b +
                    deltaX * (1 - deltaY) * (*imagem)[y1][x2].b +
                    (1 - deltaX) * deltaY * (*imagem)[y2][x1].b +
                    deltaX * deltaY * (*imagem)[y2][x2].b;
                    imagemAmpliada[i][j].b = (int)interpValueB;
            
        }
    }

    // Libere a memória da matriz original
    for (int i = 0; i < *linhas; i++) {
        free((*imagem)[i]);
    }
    free(*imagem);

    // Atualize as dimensões da imagem original
    *linhas = novaAltura;
    *colunas = novaLargura;

    // Substitua a matriz original pela matriz ampliada
    *imagem = imagemAmpliada;
}

void libera(pixel **colors, int linhas) 
{
    for (int i = 0; i < linhas; ++i) 
        free(colors[i]); // Libere a memória alocada para cada linha da matriz
    
    free(colors); // Libere a memória alocada para a matriz de ponteiros
}