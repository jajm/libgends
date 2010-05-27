#ifndef funcs_h_included
#define funcs_h_included

#include <stdint.h>
#include "basic_types.h"

typedef intptr_t (*func_ptr_t)(void *, ...);

typedef struct {
	char *name;	/* Nom de la fonction */
	func_ptr_t ptr;	/* Pointeur de fonction */
} func_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Le nom des fonctions suivantes sont AMHA suffisamment explicites pour pouvoir
 * se passer de commentaires */
func_t *func_new(const char *name, func_ptr_t ptr);
char *func_get_name(func_t *func);
func_ptr_t func_get_ptr(func_t *func);
void func_free(func_t *func);

typedef struct func_list_node_s {
	func_t *func;
	struct func_list_node_s *next;
} func_list_node_t, *funcs_t;


func_list_node_t *funcs_add(funcs_t *funcs, func_t *func);
void funcs_del(funcs_t *head, const char *name);
void funcs_free(funcs_t head);
func_t *funcs_get(funcs_t head, const char *name);
func_ptr_t funcs_get_ptr(funcs_t funcs, const char *name);

/* Appelle la fonction nommée 'func_name', si elle est présente dans la liste
 * 'funcs', avec les arguments passés en paramètres.
 * Retourne la valeur de retour de la fonction appelée */
#define funcs_call(funcs, name, arg1, ...) 	\
		(funcs_get_ptr(funcs, name))(arg1, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* Not funcs_h_included */
