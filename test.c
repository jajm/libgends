#include <stdio.h>
#include <string.h>
#include "generic.h"

typedef struct{
	int a, b;
	char c;
	double d;
} truc;

int main()
{
	generic_t g = NULL, h = NULL;
	truc a, b;

	a.a = 1; a.b = 2;
	a.c = 'c'; a.d = 4.001;

	g = generic(sizeof(truc), &a);
	if(g == NULL) puts("a");
	generic_copy(&h, g);
	if(h == NULL) puts("b");
	b = *((truc *)generic_data(h));

	printf("size of h : %d Bytes\n", generic_size(h));
	printf("value of h :\n");
	printf("\ta = %d\n\tb = %d\n\tc = %c\n\td = %f\n",
		b.a, b.b, b.c, b.d);

	generic_free(&h);
	generic_free(&g);

	return 0;
}

