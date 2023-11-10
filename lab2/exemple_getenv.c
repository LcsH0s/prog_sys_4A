#include <stdlib.h>
#include <stdio.h>
void main (void)
{
	char *ptr;	/* récupère le résultat de getenv() */

	ptr=getenv("PATH");
	printf("PATH = %s\n", ptr);
}