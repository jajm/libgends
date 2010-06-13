/*****************************************************************************
 * Copyright (C) 2010 Julian Maurice                                         *
 *                                                                           *
 * This file is part of libgends.                                            *
 *                                                                           *
 * libgends is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * libgends is distributed in the hope that it will be useful,               *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with libgends.  If not, see <http://www.gnu.org/licenses/>.         *
 *****************************************************************************/

/*****************************************************************************
 * Fichier		: iterator.h                                         *
 * Description Brève	: Gestion d'itérateurs                               *
 * Auteur		: Julian Maurice                                     *
 * Crée le		: 01/06/2010                                         *
 *****************************************************************************
 * Les fonctions ci-dessous définissent une interface pour la création       *
 * d'itérateurs sur n'importe quelle structure de données.                   *
 * La création d'un itérateur requiert l'existence d'un type correspondant   *
 * (voir types.h).                                                           *
 * Le type de l'itérateur doit posséder les fonctions suivantes:             *
 * - void *first(void *container);	Retourne un pointeur vers le 1er élt *
 * - void *next(void *container, void *pointer);    Retourne un pointeur     *
 *                                                  vers l'élt suivant       *
 * - void *get(void *container, void *pointer);	    Retourne un pointeur     *
 *                                                  vers la donnée           *
 * - s8 end(void *container, void *pointer);	Retourne 1 si tous les élts  *
 *						ont été itérés, 0 sinon      *
 *****************************************************************************/
 
#ifndef iterator_h_included
#define iterator_h_included

typedef struct {
	char *type_name;	/* Le type de l'itérateur */
	void *container;	/* Le conteneur sur lequel on va itérer */
	void *pointer;		/* La position courante de l'itérateur */
} iterator_t;

#ifdef __cplusplus
extern "C" {
#endif

/* Crée un nouvel itérateur et initialise sa position au début du conteneur */
iterator_t *iterator_new(const char *type_name, void *container);
/* Réinitialise l'itérateur à la première position */
s8 iterator_reset(iterator_t *it);
/* Déplace l'itérateur sur la position suivante */
s8 iterator_next(iterator_t *it);
/* Retourne la donnée de l'élément pointé */
void *iterator_get(iterator_t *it);
/* Retourne 1 si tous les éléments ont été parcourus, 0 sinon */
s8 iterator_end(iterator_t *it);
/* Libère la mémoire */
void iterator_free(iterator_t *it);

#ifdef __cplusplus
}
#endif

#endif /* Not iterator_h_included */

