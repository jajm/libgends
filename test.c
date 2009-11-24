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
	generic_t g, h;
	truc a, b;

	a.a = 1; a.b = 2;
	a.c = 'c'; a.d = 4.001;

	generic_init(&g);
	generic_init(&h);

	generic_affect(&g, sizeof(truc), &a);
	generic_copy(&h, &g);
	b = *((truc *)generic_data(&h));

	printf("size of h : %d Bytes\n", generic_size(&h));
	printf("value of h :\n");
	printf("\ta = %d\n\tb = %d\n\tc = %c\n\td = %f\n",
		b.a, b.b, b.c, b.d);

	generic_delete(&h);
	generic_delete(&g);

	return 0;
}

