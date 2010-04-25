#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vw_slist.h"

vw_slist_node_t *vw_slist_node_add(vw_slist_node_t *node, generic_t *data)
{
	vw_slist_node_t *newnode;

	if(node == NULL || data == NULL){
		Error("Bad parameters");
		return NULL;
	}
	newnode = malloc(sizeof(vw_slist_node_t));
	if(newnode == NULL){
		Error("Memory allocation error");
		return NULL;
	}
	newnode->data = data;
	newnode->next = node->next;
	node->next = newnode;
	
	return newnode;
}

s8 vw_slist_node_del(vw_slist_node_t *node)
{
	vw_slist_node_t *tmp;

	if(node == NULL){
		Error("Bad parameters");
		return -1;
	}
	if(node->next == NULL){
		Error("Node passed in parameter is the last node");
		return -1;
	}
	tmp = node->next->next;
	free(node->next);
	node->next = tmp;
	
	return 0;
}

s8 vw_slist_node_free(vw_slist_node_t *node)
{
	vw_slist_node_t *tmp;

	if(node == NULL){
		Error("Bad parameters");
		return -1;
	}
	if(node->next == NULL){
		Error("Node passed in parameter is the last node");
		return -1;
	}
	generic_free(node->next->data);
	tmp = node->next->next;
	free(node->next);
	node->next = tmp;

	return 0;
}
	

vw_slist_t *vw_slist_new(void)
{
	vw_slist_t *l;

	l = malloc(sizeof(vw_slist_t));
	if(l == NULL){
		Error("Memory allocation error");
		return NULL;
	}
	l->first = NULL;
	l->last = NULL;
	l->curr = NULL;
	
	return l;
}

void vw_slist_begin(vw_slist_t *l)
{
	if(l != NULL){
		l->curr = l->first;
	}
}

s8 vw_slist_next(vw_slist_t *l)
{
	if(l == NULL || l->curr == NULL)
		return -1;
	
	l->curr = l->curr->next;
	return 0;
}

s8 vw_slist_empty(vw_slist_t *l)
{
	if(l == NULL || l->first == NULL)
		return 1;
	return 0;
}

s8 vw_slist_end(vw_slist_t *l)
{
	if(l == NULL || l->curr == NULL)
		return 1;
	return 0;
}

vw_slist_node_t *vw_slist_add_first(vw_slist_t *l, generic_t *data)
{
	vw_slist_node_t *newnode;

	if(l == NULL || data == NULL){
		Error("Bad parameters");
		return NULL;
	}
	newnode = malloc(sizeof(vw_slist_node_t));
	if(newnode == NULL){
		Error("Memory allocation error");
		return NULL;
	}
	newnode->data = data;
	newnode->next = l->first;
	if(l->first == NULL){
		l->last = newnode;
	}
	l->first = newnode;
	
	return newnode;
}

vw_slist_node_t *vw_slist_add_last(vw_slist_t *l, generic_t *data)
{
	vw_slist_node_t *tmp;

	if(l == NULL || data == NULL){
		Error("Bad parameters");
		return NULL;
	}

	if(l->first == NULL){
		tmp = vw_slist_add_first(l, data);
	}else{
		tmp = vw_slist_node_add(l->last, data);
		if(tmp) l->last = tmp;
	}

	return tmp;
}

vw_slist_node_t *vw_slist_add(vw_slist_t *l, generic_t *data)
{
	vw_slist_node_t *tmp;

	if(l == NULL || data == NULL){
		Error("Bad parameters");
		return NULL;
	}

	if(l->first == NULL)
		tmp = vw_slist_add_first(l, data);
	else
		tmp = vw_slist_node_add(l->curr, data);
	
	return tmp;
}

s8 vw_slist_del_first(vw_slist_t *l)
{
	vw_slist_node_t *tmp;

	if(l == NULL){
		Error("Bad parameters");
		return -1;
	}
	if(l->first == NULL){
		Error("Empty list");
		return -1;
	}
	
	if(l->curr == l->first) l->curr = l->curr->next;
	if(l->last == l->first) l->last = NULL;
	tmp = l->first->next;
	free(l->first);
	l->first = tmp;

	return 0;
}

s8 vw_slist_del_last(vw_slist_t *l)
{
	s8 ret;
	vw_slist_node_t *tmp;

	if(l == NULL){
		Error("Bad parameters");
		return -1;
	}
	if(l->last == NULL){
		Error("Empty list");
		return -1;
	}
	if(l->first == l->last){
		ret = vw_slist_del_first(l);
	}else if(l->first->next == l->last){
		ret = vw_slist_node_del(l->first);
		if(!ret){
			l->last = l->first;
			l->curr = l->first;
		}
	}else if(l->curr->next == l->last){
		ret = vw_slist_node_del(l->curr);
		if(!ret){
			l->last = l->curr;
		}
	}else{
		tmp = l->first;
		while(tmp->next != l->last && tmp->next != NULL)
			tmp = tmp->next;
		if(tmp->next == NULL){
			Error("Last node isn't in the list");
			ret = -1;
		}else{
			ret = vw_slist_node_del(tmp);
			if(!ret){
				if(l->curr == l->last) l->curr = tmp;
				l->last = tmp;
			}
		}
	}
	
	return ret;
}


s8 vw_slist_del(vw_slist_t *l)
{
	vw_slist_node_t *tmp;
	s8 ret;

	if(l == NULL){
		Error("Bad parameters");
		return -1;
	}
	if(l->curr == NULL){
		Error("Current position not set or at the end of the list");
		return -1;
	}
	if(l->first == l->curr){
		ret = vw_slist_del_first(l);
	}else if(l->first->next == l->curr){
		ret = vw_slist_node_del(l->first);
		if(!ret){
			if(l->curr == l->last){
				l->curr = l->first;
				l->last = l->first;
			}else{
				l->curr = l->curr->next;
			}
		}
	}else{
		tmp = l->first;
		while(tmp->next != l->curr && tmp->next != NULL)
			tmp = tmp->next;
		if(tmp->next == NULL){
			Error("Current node isn't in the list");
			ret = -1;
		}else{
			ret = vw_slist_node_del(tmp);
			if(!ret){
				if(l->curr == l->last){
					l->curr = tmp;
					l->last = tmp;
				}else{
					l->curr = l->curr->next;
				}
			}
		}
	}

	return ret;
}

s8 vw_slist_free_first(vw_slist_t *l)
{
	vw_slist_node_t *tmp;

	if(l == NULL){
		Error("Bad parameters");
		return -1;
	}
	if(l->first == NULL){
		Error("Empty list");
		return -1;
	}
	
	if(l->curr == l->first) l->curr = l->curr->next;
	if(l->last == l->first) l->last = NULL;
	tmp = l->first->next;
	generic_free(l->first->data);
	free(l->first);
	l->first = tmp;

	return 0;
}

s8 vw_slist_free_last(vw_slist_t *l)
{
	s8 ret;
	vw_slist_node_t *tmp;

	if(l == NULL){
		Error("Bad parameters");
		return -1;
	}
	if(l->last == NULL){
		Error("Empty list");
		return -1;
	}
	if(l->first == l->last){
		ret = vw_slist_free_first(l);
	}else if(l->first->next == l->last){
		ret = vw_slist_node_free(l->first);
		if(!ret){
			l->last = l->first;
			l->curr = l->first;
		}
	}else if(l->curr->next == l->last){
		ret = vw_slist_node_free(l->curr);
		if(!ret){
			l->last = l->curr;
		}
	}else{
		tmp = l->first;
		while(tmp->next != l->last && tmp->next != NULL)
			tmp = tmp->next;
		if(tmp->next == NULL){
			Error("Last node isn't in the list");
			ret = -1;
		}else{
			ret = vw_slist_node_free(tmp);
			if(!ret){
				if(l->curr == l->last) l->curr = tmp;
				l->last = tmp;
			}
		}
	}
	
	return ret;
}


s8 vw_slist_free(vw_slist_t *l)
{
	vw_slist_node_t *tmp;
	s8 ret;

	if(l == NULL){
		Error("Bad parameters");
		return -1;
	}
	if(l->curr == NULL){
		Error("Empty list");
		return -1;
	}
	if(l->first == l->curr){
		ret = vw_slist_free_first(l);
	}else if(l->first->next == l->curr){
		ret = vw_slist_node_free(l->first);
		if(!ret){
			if(l->curr == l->last){
				l->curr = l->first;
				l->last = l->first;
			}else{
				l->curr = l->curr->next;
			}
		}
	}else{
		tmp = l->first;
		while(tmp->next != l->curr && tmp->next != NULL)
			tmp = tmp->next;
		if(tmp->next == NULL){
			Error("Current node isn't in the list");
			ret = -1;
		}else{
			ret = vw_slist_node_free(tmp);
			if(!ret){
				if(l->curr == l->last){
					l->curr = tmp;
					l->last = tmp;
				}else{
					l->curr = l->curr->next;
				}
			}
		}
	}

	return ret;
}

generic_t *vw_slist_get_first(vw_slist_t *l)
{
	if(l == NULL){
		Error("Bad parameters");
		return NULL;
	}
	if(l->first == NULL){
		Error("Empty list");
		return NULL;
	}

	return l->first->data;
}

generic_t *vw_slist_get_last(vw_slist_t *l)
{
	if(l == NULL){
		Error("Bad parameters");
		return NULL;
	}
	if(l->last == NULL){
		Error("Empty list");
		return NULL;
	}

	return l->last->data;
}

generic_t *vw_slist_get(vw_slist_t *l)
{
	if(l == NULL){
		Error("Bad parameters");
		return NULL;
	}
	if(l->curr == NULL){
		Error("Empty list");
		return NULL;
	}

	return l->curr->data;
}

generic_t *vw_slist_pop_first(vw_slist_t *l)
{
	generic_t *g;

	g = vw_slist_get_first(l);
	if(g) vw_slist_del_first(l);

	return g;
}

generic_t *vw_slist_pop_last(vw_slist_t *l)
{
	generic_t *g;

	g = vw_slist_get_last(l);
	if(g) vw_slist_del_last(l);

	return g;
}

generic_t *vw_slist_pop(vw_slist_t *l)
{
	generic_t *g;

	g = vw_slist_get(l);
	if(g) vw_slist_del(l);

	return g;
}

vw_slist_node_t *vw_slist_chk(vw_slist_t *l, generic_t *data)
{
	generic_t *g;
	vw_slist_node_t *node = NULL;
	vw_slist_node_t *tmp;

	if(l == NULL || data == NULL){
		Error("Bad parameters");
		return NULL;
	}
	
	tmp = l->first;
	while(tmp){
		if(generic_cmp(tmp->data, data) == 0){
			node = tmp;
		}
		tmp = tmp->next;
	}

	return node;
}

void vw_slist_print(vw_slist_t *l)
{
	vw_slist_node_t *tmp;
	
	if(l != NULL){	
		tmp = l->first;
		while(tmp != NULL){
			printf("%p -> ", tmp->data);
			tmp = tmp->next;
		}
		printf("NULL\n");
	}
}

void vw_slist_free_list(vw_slist_t *l)
{
	vw_slist_node_t *tmp, *tmp2;
	if(l != NULL){
		tmp = l->first;
		while(tmp != NULL){
			tmp2 = tmp->next;
			generic_free(tmp->data);
			free(tmp);
			tmp = tmp2;
		}
		free(l);
	}
}
