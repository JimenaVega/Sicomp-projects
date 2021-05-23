# include <stdio.h>
# include <string.h>

int main(int argc, char const *argv[])
{
	/* code */
	int iValor = 7;
	char buffer_uno[8], buffer_dos[8];

	strcpy(buffer_uno, "UNO");
	strcpy(buffer_dos, "DOS");

	printf("[ANTES] buffer_dos está en %p y contiene el valor \'%s\'\n", buffer_dos, buffer_dos);
	printf("[ANTES] buffer_uno está en %p y contiene el valor \'%s\'\n", buffer_uno, buffer_uno);
	printf("[ANTES] iValor está en %p y contiene el valor %d (0x%08x)\n", &iValor, iValor, iValor);
	
	printf("\n[STRCPY copiando %d bytes en posición de memoria de buffer_dos\n\n", strlen(argv[1]));
	strcpy(buffer_dos, argv[1]);

	printf("[DESPUES] buffer_dos está en %p y contiene el valor \'%s\'\n", buffer_dos, buffer_dos);
	printf("[DESPUES] buffer_uno está en %p y contiene el valor \'%s\'\n", buffer_uno, buffer_uno);
	printf("[DESPUES] iValor está en %p y contiene el valor %d (0x%08x)\n", &iValor, iValor, iValor);
	
	return 0;
}