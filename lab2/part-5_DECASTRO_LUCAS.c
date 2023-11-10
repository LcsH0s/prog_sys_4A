#include <stdlib.h>
#include <stdio.h>


int main ()
{
	int status_code = system ("env | wc -l");
    printf("Code de retour : %d\n", status_code);
}