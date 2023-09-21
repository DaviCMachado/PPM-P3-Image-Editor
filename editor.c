#include "auxiliar.h"
#include "funcoes.c"

void editor ()
{
	char in[30], out[30]; 
	int i = 0, j = 0;

    printf("\nEscreva o nome do arquivo que deseja editar: (Escreva "".ppm"" no final)\n");
	fgets(in, sizeof(in), stdin);
	in[strlen(in)-1] = '\0'; 

	printf("\nQual nome voce deseja dar ao novo arquivo editado? (Escreva "".ppm"" no final)\n");
	fgets(out, sizeof(out), stdin);
	out[strlen(out)-1] = '\0'; 


   FILE *entradaArq = fopen(in, "r");
	if (!entradaArq) 
	{
    	printf("Erro ao abrir o arquivo de entrada!\n");
    	return;
	}

	FILE *novoArq = fopen(out, "w"); 
	
	if (!novoArq) 
	{
    	printf("Erro ao criar o arquivo de saída!\n");
    	fclose(entradaArq);
    	return;
	}

	// Usuario escolhe quais efeitos aplicar

    printf("\nOpcoes de Edicao:   (A selecao para editar so afetara os efeitos de Filtro\n");
	printf("\n\t%-25s\t%-25s\t%-25s\n","[1] Filtro Avermelhado", "[2] Filtro Azulado", "[3] Rotação para a direita" );
	printf("\t%-25s\t%-25s\t%-25s\n","[4] Espelhamento", "[5] Recortar", "[6] Colar Recorte" );
	printf("\t%-25s\t%-25s\t%-35s\n","[7] Alterar Escala", "[8] Selecao para editar", "[9] Colar Imagem Externa" );

    printf("\nEscolha as edicoes desejadas: Digite (sim/nao) para cada opcao... \n");
	int choices[efeitos]; 
	char ch[50];
	
	// Escolha de efeitos do usuario

	for (i = 0; i < efeitos; ++i)
	{
		printf("[%2d] : ", i+1);
		scanf(" %c", &ch);

		if(ch[0] == 's' || ch[0] == 'S')
			choices[i] = 1;	
		else
			choices[i] = 0;
		
		int c;
		while ((c = getchar()) != '\n' && c != EOF) { }
	}

	

	int colunas, linhas, max;
	char buffer[256];	

	// Lendo a header do arquivo e verificando se eh do tipo P3

	fgets(buffer, sizeof(buffer), entradaArq);

	if (buffer[1] != '3')
	{
		printf("\nArquivo nao eh do tipo P3!\n\n");
		return;
	}
    
	do{
		fgets(buffer, 256, entradaArq);
	}while(buffer[0] == '#');
	
	sscanf(buffer, "%d %d", &colunas, &linhas, &max);
		
	do{
		fgets(buffer, 256, entradaArq);
	}while(buffer[0] == '#');
	
	sscanf(buffer, "%d", &max);



	// Avisar o usuario que a imagem esta sendo processada
	printf("\nNova imagem sendo criada, Aguarde...\n");


	// Alocacao para imagem e possivel recorte
	pixel** colors = malloc(linhas * sizeof(pixel *));
	pixel** imagem_recortada = malloc(linhas * sizeof(pixel *));
	
	for (i = 0; i < linhas; ++i)
    	colors[i] = malloc(colunas * sizeof(pixel));

	for (i = 0; i < linhas; ++i)
    	imagem_recortada[i] = malloc(colunas * sizeof(pixel));

	// leitura dos pixels
	for (i = 0; i < linhas; i++) 
	{
    	for (j = 0; j < colunas; j++) 
		{
        	fscanf(entradaArq, "%d ", &colors[i][j].r); // leia o valor do componente vermelho
			
			if (colors[i][j].r < 0) 
			{
				colors[i][j].r = 0;
			} 
			else if (colors[i][j].r > max) 
			{
				colors[i][j].r = max;
			}
        	
			fscanf(entradaArq, "%d ", &colors[i][j].g); // leia o valor do componente verde
			
			if (colors[i][j].g < 0) 
			{
				colors[i][j].g = 0;
			} 
			else if (colors[i][j].g > max) 
			{
				colors[i][j].g = max;
			}
        	
			fscanf(entradaArq, "%d ", &colors[i][j].b); // leia o valor do componente azul
    
			if (colors[i][j].b < 0) 
			{
				colors[i][j].b = 0;
			} 
			
			else if (colors[i][j].b > max) 
			{
				colors[i][j].b = max;
			}
		}
	}
		
	fclose (entradaArq);


	// array de funções
	void (*edita[efeitos])(pixel **, int, int, int, int, int );
	edita[0] = &filtroV; ///
	edita[1] = &filtroA; ///
	

	double fatorHorizontal;
	double fatorVertical;   
	int edit_x1 = 0, edit_y1 = 0, edit_x2 = colunas, edit_y2 = linhas;
	
	// aplicando as funções caso o usuario tenha-as selecionado
	
	if (choices[7])
	{
		printf("\n      Ilustracao da imagem em pixels:\n");
		for (int i = 0; i <= linhas; i += (int) ((1.0/4.0) * linhas))
		{
			for (int j = 0; j <= colunas; j += (int) ((1.0/4.0) * colunas))
				printf("  %d:%d", j, i);
			printf("\n");
		}

		printf("\nEscolha de quanto a quanto voce quer editar: (x1, y1, x2, y2)\n");
		scanf(" %d %d %d %d", &edit_x1, &edit_y1, &edit_x2, &edit_y2);

		if (edit_x1 < 0 || edit_x2 < 0 || edit_y1 < 0 || edit_y2 < 0 || edit_x1 > colunas 
		|| edit_x2 > colunas || edit_y1 > linhas || edit_y2 > linhas)
		{
			printf ("\nNumeros invalidos!\n");
			exit(1);
		}
	}

	if (choices[3]) // espelhamento
		espelho (colors, linhas, colunas); //call

	if (choices[0]) // filtro vermelho
		edita[0](colors, edit_x1, edit_y1, edit_x2, edit_y2, max); //call

	if (choices[1]) //filtro azul
		edita[1](colors, edit_x1, edit_y1, edit_x2, edit_y2, max); //call

	if (choices[2])  	// rotacionar
	{
		rotaciona(&colors, &linhas, &colunas); //call
	}

	if (choices[6]) 	// alterar escala
	{
		printf("\nQue escala horizontal voce deseja aplicar?\n");
		scanf(" %lf", &fatorHorizontal);
		printf("\nQue escala vertical voce deseja aplicar?\n");
		scanf(" %lf", &fatorVertical);
		escala(&colors, &linhas, &colunas, fatorHorizontal, fatorVertical);
	}

	if (choices[4] && !choices[5])
	{
		int x1, x2, y1, y2;

		printf("\n      Ilustracao da imagem em pixels:\n");
		for (int i = 0; i <= linhas; i += (int) ((1.0/4.0) * linhas))
		{
			for (int j = 0; j <= colunas; j += (int) ((1.0/4.0) * colunas))
				printf("  %d:%d", j, i);
			printf("\n");
		}

		printf("\n(x1,y1) e (x2,y2) de onde voce quer recortar:\n");
		scanf(" %d %d %d %d", &x1, &y1, &x2, &y2);

		if (x2 == colunas)
			x2--;
	
		if (x1 == colunas)
			x1--;

		if (y2 == linhas)
			y2--;

		if (y1 == linhas)
			y1--;

		if (x1 >= 0 && x1 < colunas && y1 >= 0 && y1 < linhas &&
			x2 >= 0 && x2 < colunas && y2 >= 0 && y2 < linhas &&
			x1 <= x2 && y1 <= y2)
		{
			colors = recorta (colors, &linhas, &colunas, x1, y1, x2, y2);
		}

		else
		{ 
			printf("Escolha de recorte invalida");
		}

		linhas = y2 - y1 + 1;
		colunas = x2 - x1 + 1;
	}

	else if (choices[4]) 	// recortar
	{
		int x1, x2, y1, y2;

		printf("\n      Ilustracao da imagem em pixels:\n");
		for (int i = 0; i <= linhas; i += (int) ((1.0/4.0) * linhas))
		{
			for (int j = 0; j <= colunas; j += (int) ((1.0/4.0) * colunas))
				printf("  %d:%d", j, i);
			printf("\n");
		}

		printf("\nx1 e y1 de onde voce quer recortar:\n");
		scanf(" %d %d", &x1, &y1);
		printf("\nx2 e y2 de onde voce quer recortar:\n");
		scanf(" %d %d", &x2, &y2);

		if (x2 == colunas)
			x2--;
	
		if (x1 == colunas)
			x1--;

		if (y2 == linhas)
			y2--;

		if (y1 == linhas)
			y1--;

		if (x1 >= 0 && x1 < colunas && y1 >= 0 && y1 < linhas &&
			x2 >= 0 && x2 < colunas && y2 >= 0 && y2 < linhas &&
			x1 <= x2 && y1 <= y2)
		{
			imagem_recortada = recorta (colors, &linhas, &colunas, x1, y1, x2, y2);
		}

		else
		{ 
			printf("Escolha de recorte invalida");
		}
	}

	if (choices[5]) // colar
	{
		colar(&colors, &linhas, &colunas, max);
	}

	if (choices[8])
	{
		char ext[30];
		
		printf("\nEscreva o nome do arquivo que deseja usar para colar: (Escreva \".ppm\" no final)\n");
		fgets(ext, sizeof(ext), stdin);
		ext[strlen(ext)-1] = '\0'; 

		int resultado = colar_Imagem_Externa(&colors, &linhas, &colunas, ext);

		if (resultado)
		{
			printf("Imagem externa colada com sucesso!\n");
		}
		else
		{
			printf("Erro ao colar a imagem externa.\n");
		}

	}

	
			/* Escrevendo o novo arquivo */

	//header
	fprintf(novoArq, "P3\n#Created by Kiri\n%d %d\n%d\n", colunas, linhas, max);
	
	// Escrever os dados de cor
	for (i = 0; i < linhas; i++) 
	{
		for (j = 0; j < colunas; j++) 
		{
			fprintf(novoArq, "%d %d %d ", colors[i][j].r, colors[i][j].g, colors[i][j].b);
		}
		fprintf(novoArq, "\n"); // Pule uma linha após cada linha de pixels
	}

	printf("\n%s criado com sucesso!\n", out);

	fclose(novoArq); 

	libera(colors, linhas); // Liberar a memória alocada para a matriz colors
	libera(imagem_recortada, linhas); // Liberar a memória alocada para a imagem recortada

}