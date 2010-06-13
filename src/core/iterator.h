#ifndef iterator_h_included
#define iterator_h_included

/* Le type de l'itérateur doit posséder les fonctions suivantes:
 * - void *first(void *container);	Retourne un pointeur vers le 1er élt
 * - void *next(void *container, void *pointer);	Retourne un pointeur
 *							vers l'élt suivant
 * - void *get(void *container, void *pointer);		Retourne un pointeur vers la donnée
 * - s8 end(void *container, void *pointer);	Retourne 1 si tous les élts
 *						ont été itérés, 0 sinon
 */

typedef struct {
	char *type_name;
	void *container;
	void *pointer;
} iterator_t;

iterator_t *iterator_new(const char *type_name, void *container);
s8 iterator_reset(iterator_t *it);
s8 iterator_next(iterator_t *it);
void *iterator_get(iterator_t *it);
s8 iterator_end(iterator_t *it);
void iterator_free(iterator_t *it);


#endif /* Not iterator_h_included */

