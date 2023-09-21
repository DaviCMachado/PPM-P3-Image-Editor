#include "editor.c"

int main (int argc, char const *argv[]) 
{
	int opt = 1, c;

	while (opt != 0)
	{
		editor();
		printf ("\n\nFim da edicao. \nO que fazer? \n(0) Sair \n(1) Editar Novamente\n");
		scanf (" %d", &opt);
		while ((c = getchar()) != '\n' && c != EOF) { }
	}

    return 0;
}

