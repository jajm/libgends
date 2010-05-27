#ifndef dlist_h_included
#define dlist_h_included

#include "basic_types.h"
#include "types.h"

typedef struct dlist_node_s {
	struct dlist_node_s *prev;
	void *data;
	struct dlist_node_s *next;
} dlist_node_t;

/* Ajoute un nœud contenant data avant node
 * Retourne l'adresse du nouveau nœud */
dlist_node_t *dlist_node_add_before(dlist_node_t *node, void *data);
/* Ajoute un nœud contenant data après node
 * Retourne l'adresse du nouveau nœud */
dlist_node_t *dlist_node_add_after(dlist_node_t *node, void *data);
/* Retourne un pointeur vers la donnée de node */
void *dlist_node_get(dlist_node_t *node);
/* Supprime le nœud node et fait pointer data vers la donnée du nœud
 * supprimé */
void dlist_node_pop(dlist_node_t *node, void **data);
/* Supprime le nœud node et libère la mémoire occupée par la donnée
 * à l'aide de free_f */
void dlist_node_del(dlist_node_t *node, func_ptr_t free_f);


typedef struct {
	char *type_name;
	dlist_node_t *first;
	dlist_node_t *last;
	dlist_node_t *curr;
} dlist_t;

/* Crée une nouvelle liste et retourne l'adresse de cette liste */
dlist_t *dlist_new(const char *type_name);

/* Initialise la position courante au début de la liste */
void dlist_begin(dlist_t *l);
/* Initialise la position courante à la fin de la liste */
void dlist_finish(dlist_t *l);

/* Déplace la position courante vers le nœud suivant */
/* Retourne -1 si le nœeud suivant n'existe pas (fin de liste), 0 sinon */
s8 dlist_next(dlist_t *l);
/* Déplace la position courante vers le nœud précédent */
/* Retourne -1 si le nœud précédent d'existe pas (début de liste), 0 sinon */
s8 dlist_prev(dlist_t *l);

/* Teste si la liste est vide (0 = non vide, 1 = vide) */
s8 dlist_empty(dlist_t *l);

/* Teste si la position courante est à la fin de la liste */
/* (après le dernier élément) */
/* Retourne 1 si c'est la fin, 0 sinon */
s8 dlist_end(dlist_t *l);
/* Teste si la position courante est au début de la liste
 * (avant le premier élément)
 * Retourne 1 si c'est le début, 0 sinon */
s8 dlist_beginning(dlist_t *l);
/* Pour l'instant ces deux fonctions sont exactement les mêmes, car il n'y a
 * pas de moyen de savoir si la position courante est avant le premier, ou
 * après le dernier élément. Seul un test d'egalité à NULL est effectué. */

/* ========================================================================= */
/*                             Fonctions d'ajout                             */
/* ========================================================================= */

/* Ajoute un nœud à la liste en première position */
/* Retourne l'adresse du nouveau nœud */
dlist_node_t *dlist_add_first(
	dlist_t *l,		/* Liste à modifier */
	void *data		/* Donnée à associer au nœud.
				   Ne pas libérer la mémoire. */
);

/* Ajoute un nœud à la liste en dernière position */
/* Retourne l'adresse du nouveau nœud */
dlist_node_t *dlist_add_last(
	dlist_t *l,		/* Liste à modifier */
	void *data		/* Donnée à associer au nœud.
				   Ne pas libérer la mémoire. */
);

/* Ajoute un nœud à la liste après la position courante */
/* Retourne l'adresse du nouveau nœud */
dlist_node_t *dlist_add(
	dlist_t *l,		/* Liste à modifier */
	void *data		/* Donnée à associer au nœud.
				   Ne pas libérer la mémoire */
);

/* ========================================================================= */
/*                          Fonctions de suppression                         */
/* ========================================================================= */

/* --------- Supprime un nœud et renvoie la donnée qu'il contient ---------- */
/* Supprime le premier nœud de la liste */
void *dlist_pop_first(dlist_t *l);

/* Supprime le dernier nœud de la liste */
void *dlist_pop_last(dlist_t *l);

/* Supprime le nœud de la liste à la position courante */
void *dlist_pop(dlist_t *l);

/* ---------- Supprime un nœud ainsi que la donnée qu'il contient ---------- */
/* Supprime le premier nœud de la liste */
s8 dlist_del_first(dlist_t *l);

/* Supprime le dernier nœud de la liste */
s8 dlist_del_last(dlist_t *l);

/* Supprime le nœud de la liste à la position courante */
s8 dlist_del(dlist_t *l);


/* ========================================================================= */
/*                  Fonctions de récupération des données                    */
/* ========================================================================= */

/* Récupère la donnée du premier nœud de la liste */
void *dlist_get_first(dlist_t *l);

/* Récupère la donnée du dernier nœud de la liste */
void *dlist_get_last(dlist_t *l);

/* Récupère la donnée du nœud de la liste à la position courante */
void *dlist_get(dlist_t *l);


/* ========================================================================= */
/*                             Autres fonctions                              */
/* ========================================================================= */

/* Vérification de la présence d'un élément dans une liste */
dlist_node_t *dlist_chk(dlist_t *l, void *data);

/* Libération de la mémoire */
void dlist_free(dlist_t *l);

void dlist_destroy(dlist_t *l);

#endif /* Not dlist_h_included */

