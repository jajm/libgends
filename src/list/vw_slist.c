#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vw_slist.h"

vw_slist_node_ptr vw_slist_node_add(vw_slist_node_ptr node, generic_ptr data, error_ptr *err)
{
	vw_slist_node_ptr newnode;

	if(node == NULL || data == NULL){
		error_set(err, -1, "Bad parameters");
		return NULL;
	}
	newnode = malloc(sizeof(struct vw_slist_node_t));
	if(newnode == NULL){
		error_set(err, -1, "Memory allocation error");
		return NULL;
	}
	newnode->data = data;
	newnode->next = node->next;
	node->next = newnode;
	
	error_free(err);
	return newnode;
}

s8 vw_slist_node_del(vw_slist_node_ptr node, error_ptr *err)
{
	vw_slist_node_ptr tmp;

	if(node == NULL){
		error_set(err, -1, "Bad parameters");
		return -1;
	}
	if(node->next == NULL){
		error_set(err, -1, "Node passed in parameter is the last node");
		return -1;
	}
	tmp = node->next->next;
	generic_free(&(node->next->data));
	free(node->next);
	node->next = tmp;
	
	error_free(err);
	return 0;
}
	

vw_slist_ptr vw_slist_new(error_ptr *err)
{
	vw_slist_ptr l;

	l = malloc(sizeof(struct vw_slist_t));
	if(l == NULL){
		error_set(err, -1, "Memory allocation error");
		return NULL;
	}
	l->first = NULL;
	l->last = NULL;
	l->curr = NULL;
	
	error_free(err);
	return l;
}

void vw_slist_begin(vw_slist_ptr l)
{
	if(l != NULL){
		l->curr = l->first;
	}
}

s8 vw_slist_next(vw_slist_ptr l)
{
	if(l == NULL || l->curr == NULL || l->curr->next == NULL)
		return -1;
	
	l->curr = l->curr->next;
	return 0;
}

vw_slist_node_ptr vw_slist_add_first(vw_slist_ptr l, generic_ptr data, error_ptr *err)
{
	vw_slist_node_ptr newnode;

	if(l == NULL || data == NULL){
		error_set(err, -1, "Bad parameters");
		return NULL;
	}
	newnode = malloc(sizeof(struct vw_slist_node_t));
	if(newnode == NULL){
		error_set(err, -1, "Memory allocation error");
		return NULL;
	}
	newnode->data = data;
	newnode->next = l->first;
	if(l->first == NULL){
		l->last = newnode;
		l->curr = newnode;
	}
	l->first = newnode;
	
	error_free(err);
	return newnode;
}

vw_slist_node_ptr vw_slist_add_last(vw_slist_ptr l, generic_ptr data, error_ptr *err)
{
	vw_slist_node_ptr tmp;

	if(l == NULL || data == NULL){
		error_set(err, -1, "Bad parameters");
		return NULL;
	}

	if(l->first == NULL){
		tmp = vw_slist_add_first(l, data, err);
	}else{
		tmp = vw_slist_node_add(l->last, data, err);
		if(tmp) l->last = tmp;
	}

	return tmp;
}

vw_slist_node_ptr vw_slist_add(vw_slist_ptr l, generic_ptr data, error_ptr *err)
{
	vw_slist_node_ptr tmp;

	if(l == NULL || data == NULL){
		error_set(err, -1, "Bad parameters");
		return NULL;
	}

	if(l->first == NULL)
		tmp = vw_slist_add_first(l, data, err);
	else
		tmp = vw_slist_node_add(l->curr, data, err);
	
	return tmp;
}

s8 vw_slist_del_first(vw_slist_ptr l, error_ptr *err)
{
	vw_slist_node_ptr tmp;

	if(l == NULL){
		error_set(err, -1, "Bad parameters");
		return -1;
	}
	if(l->first == NULL){
		error_set(err, -1, "Empty list");
		return -1;
	}
	
	if(l->curr == l->first) l->curr = l->curr->next;
	if(l->last == l->first) l->last = NULL;
	tmp = l->first->next;
	generic_free(&(l->first->data));
	free(l->first);
	l->first = tmp;

	error_free(err);
	return 0;
}

s8 vw_slist_del_last(vw_slist_ptr l, error_ptr *err)
{
	s8 ret;
	vw_slist_node_ptr tmp;

	if(l == NULL){
		error_set(err, -1, "Bad parameters");
		return -1;
	}
	if(l->last == NULL){
		error_set(err, -1, "Empty list");
		return -1;
	}
	if(l->first == l->last){
		ret = vw_slist_del_first(l, err);
	}else if(l->first->next == l->last){
		ret = vw_slist_node_del(l->first, err);
		if(!ret){
			l->last = l->first;
			l->curr = l->first;
		}
	}else if(l->curr->next == l->last){
		ret = vw_slist_node_del(l->curr, err);
		if(!ret){
			l->last = l->curr;
		}
	}else{
		tmp = l->first;
		while(tmp->next != l->last && tmp->next != NULL)
			tmp = tmp->next;
		if(tmp->next == NULL){
			error_set(err, -1, "Last node isn't in the list");
			ret = -1;
		}else{
			ret = vw_slist_node_del(tmp, err);
			if(!ret){
				if(l->curr == l->last) l->curr = tmp;
				l->last = tmp;
			}
		}
	}
	
	return ret;
}


s8 vw_slist_del(vw_slist_ptr l, error_ptr *err)
{
	vw_slist_node_ptr tmp;
	s8 ret;

	if(l == NULL){
		error_set(err, -1, "Bad parameters");
		return -1;
	}
	if(l->curr == NULL){
		error_set(err, -1, "Empty list");
		return -1;
	}
	if(l->first == l->curr){
		ret = vw_slist_del_first(l, err);
	}else if(l->first->next == l->curr){
		ret = vw_slist_node_del(l->first, err);
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
			error_set(err, -1, "Current node isn't in the list");
			ret = -1;
		}else{
			ret = vw_slist_node_del(tmp, err);
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

generic_ptr vw_slist_get_first(vw_slist_ptr l, error_ptr *err)
{
	if(l == NULL){
		error_set(err, -1, "Bad parameters");
		return NULL;
	}
	if(l->first == NULL){
		error_set(err, -1, "Empty list");
		return NULL;
	}

	return l->first->data;
}

generic_ptr vw_slist_get_last(vw_slist_ptr l, error_ptr *err)
{
	if(l == NULL){
		error_set(err, -1, "Bad parameters");
		return NULL;
	}
	if(l->last == NULL){
		error_set(err, -1, "Empty list");
		return NULL;
	}

	return l->last->data;
}

generic_ptr vw_slist_get(vw_slist_ptr l, error_ptr *err)
{
	if(l == NULL){
		error_set(err, -1, "Bad parameters");
		return NULL;
	}
	if(l->curr == NULL){
		error_set(err, -1, "Empty list");
		return NULL;
	}

	return l->curr->data;
}

generic_ptr vw_slist_pop_first(vw_slist_ptr l, error_ptr *err)
{
	generic_ptr g;

	g = vw_slist_get_first(l, err);
	if(g) vw_slist_del_first(l, err);

	return g;
}

generic_ptr vw_slist_pop_last(vw_slist_ptr l, error_ptr *err)
{
	generic_ptr g;

	g = vw_slist_get_last(l, err);
	if(g) vw_slist_del_last(l, err);

	return g;
}

generic_ptr vw_slist_pop(vw_slist_ptr l, error_ptr *err)
{
	generic_ptr g;

	g = vw_slist_get(l, err);
	if(g) vw_slist_del(l, err);

	return g;
}

vw_slist_node_ptr vw_slist_chk(vw_slist_ptr l, generic_ptr data, error_ptr *err)
{
	generic_ptr g;
	vw_slist_node_ptr node = NULL;

	if(l == NULL || data == NULL){
		error_set(err, -1, "Bad parameters");
		return NULL;
	}

	vw_slist_begin(l);
	do{
		g = vw_slist_get(l, err);
		if(generic_cmp(g, data) == 0){
			node = l->curr;
			break;
		}
	}while(vw_slist_next(l) == 0);

	return node;
}

void vw_slist_print(vw_slist_ptr l)
{
	struct vw_slist_node_t *tmp;
	
	if(l != NULL){	
		tmp = l->first;
		while(tmp != NULL){
			printf("%p -> ", tmp->data);
			tmp = tmp->next;
		}
		printf("NULL\n");
	}
}

void vw_slist_free(vw_slist_ptr *l)
{
	vw_slist_node_ptr tmp, tmp2;
	if(*l != NULL){
		tmp = (*l)->first;
		while(tmp != NULL){
			tmp2 = tmp->next;
			generic_free(&(tmp->data));
			free(tmp);
			tmp = tmp2;
		}
		free(l);
		*l = NULL;
	}
}
