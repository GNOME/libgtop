/*
 * This entire header file is a horrible workaround that probably shouldn't be used,
 * but exists until a better solution is found.
 */
#ifndef _RB_WORKAROUND_H_
#define _RB_WORKAOURND_H_

#define RBE_LEFT(elm, field)		(elm)->field.rbt_left
#define RBE_RIGHT(elm, field)		(elm)->field.rbt_right
#define RBE_PARENT(elm, field)		(elm)->field.rbt_parent
#define RBE_COLOR(elm, field)		(elm)->field.rbt_color

#define RBE_SET(elm, parent, field) do {				\
	RBE_PARENT(elm, field) = parent;				\
	RBE_LEFT(elm, field) = RBE_RIGHT(elm, field) = NULL;		\
	RBE_COLOR(elm, field) = RB_RED;					\
} while (0)

#define RBE_SET_BLACKRED(black, red, field) do {			\
	RBE_COLOR(black, field) = RB_BLACK;				\
	RBE_COLOR(red, field) = RB_RED;					\
} while (0)

#ifndef RBE_AUGMENT
#define RBE_AUGMENT(x)	do {} while (0)
#endif

#define RBE_ROTATE_LEFT(head, elm, tmp, field) do {			\
	(tmp) = RBE_RIGHT(elm, field);					\
	if ((RBE_RIGHT(elm, field) = RBE_LEFT(tmp, field))) {		\
		RBE_PARENT(RBE_LEFT(tmp, field), field) = (elm);	\
	}								\
	RBE_AUGMENT(elm);						\
	if ((RBE_PARENT(tmp, field) = RBE_PARENT(elm, field))) {	\
		if ((elm) == RBE_LEFT(RBE_PARENT(elm, field), field))	\
			RBE_LEFT(RBE_PARENT(elm, field), field) = (tmp);\
		else							\
			RBE_RIGHT(RBE_PARENT(elm, field), field) = (tmp);\
	} else								\
		(head)->rbh_root = (tmp);				\
	RBE_LEFT(tmp, field) = (elm);					\
	RBE_PARENT(elm, field) = (tmp);					\
	RBE_AUGMENT(tmp);						\
	if ((RBE_PARENT(tmp, field)))					\
		RBE_AUGMENT(RBE_PARENT(tmp, field));			\
} while (0)

#define RBE_ROTATE_RIGHT(head, elm, tmp, field) do {			\
	(tmp) = RBE_LEFT(elm, field);					\
	if ((RBE_LEFT(elm, field) = RBE_RIGHT(tmp, field))) {		\
		RBE_PARENT(RBE_RIGHT(tmp, field), field) = (elm);	\
	}								\
	RBE_AUGMENT(elm);						\
	if ((RBE_PARENT(tmp, field) = RBE_PARENT(elm, field))) {	\
		if ((elm) == RBE_LEFT(RBE_PARENT(elm, field), field))	\
			RBE_LEFT(RBE_PARENT(elm, field), field) = (tmp);\
		else							\
			RBE_RIGHT(RBE_PARENT(elm, field), field) = (tmp);\
	} else								\
		(head)->rbh_root = (tmp);				\
	RBE_RIGHT(tmp, field) = (elm);					\
	RBE_PARENT(elm, field) = (tmp);					\
	RBE_AUGMENT(tmp);						\
	if ((RBE_PARENT(tmp, field)))					\
		RBE_AUGMENT(RBE_PARENT(tmp, field));			\
} while (0)

/* Generates prototypes and inline functions */
#define	RBE_PROTOTYPE(name, type, field, cmp)				\
	RBE_PROTOTYPE_INTERNAL(name, type, field, cmp,)
#define	RBE_PROTOTYPE_STATIC(name, type, field, cmp)			\
	RBE_PROTOTYPE_INTERNAL(name, type, field, cmp, __attribute__((__unused__)) static)
#define RBE_PROTOTYPE_INTERNAL(name, type, field, cmp, attr)		\
attr void name##_RBE_INSERT_COLOR(struct name *, struct type *);	\
attr void name##_RBE_REMOVE_COLOR(struct name *, struct type *, struct type *);\
attr struct type *name##_RBE_REMOVE(struct name *, struct type *);	\
attr struct type *name##_RBE_INSERT(struct name *, struct type *);	\
attr struct type *name##_RBE_FIND(struct name *, struct type *);	\
attr struct type *name##_RBE_NFIND(struct name *, struct type *);	\
attr struct type *name##_RBE_NEXT(struct type *);			\
attr struct type *name##_RBE_PREV(struct type *);			\
attr struct type *name##_RBE_MINMAX(struct name *, int);		\
									\

/* Main rb operation.
 * Moves node close to the key of elm to top
 */
#define	RBE_GENERATE(name, type, field, cmp)				\
	RBE_GENERATE_INTERNAL(name, type, field, cmp,)
#define	RBE_GENERATE_STATIC(name, type, field, cmp)			\
	RBE_GENERATE_INTERNAL(name, type, field, cmp, __attribute__((__unused__)) static)
#define RBE_GENERATE_INTERNAL(name, type, field, cmp, attr)		\
attr void								\
name##_RBE_INSERT_COLOR(struct name *head, struct type *elm)		\
{									\
	struct type *parent, *gparent, *tmp;				\
	while ((parent = RBE_PARENT(elm, field)) &&			\
	    RBE_COLOR(parent, field) == RB_RED) {			\
		gparent = RBE_PARENT(parent, field);			\
		if (parent == RBE_LEFT(gparent, field)) {		\
			tmp = RBE_RIGHT(gparent, field);		\
			if (tmp && RBE_COLOR(tmp, field) == RB_RED) {	\
				RBE_COLOR(tmp, field) = RB_BLACK;	\
				RBE_SET_BLACKRED(parent, gparent, field);\
				elm = gparent;				\
				continue;				\
			}						\
			if (RBE_RIGHT(parent, field) == elm) {		\
				RBE_ROTATE_LEFT(head, parent, tmp, field);\
				tmp = parent;				\
				parent = elm;				\
				elm = tmp;				\
			}						\
			RBE_SET_BLACKRED(parent, gparent, field);	\
			RBE_ROTATE_RIGHT(head, gparent, tmp, field);	\
		} else {						\
			tmp = RBE_LEFT(gparent, field);			\
			if (tmp && RBE_COLOR(tmp, field) == RB_RED) {	\
				RBE_COLOR(tmp, field) = RB_BLACK;	\
				RBE_SET_BLACKRED(parent, gparent, field);\
				elm = gparent;				\
				continue;				\
			}						\
			if (RBE_LEFT(parent, field) == elm) {		\
				RBE_ROTATE_RIGHT(head, parent, tmp, field);\
				tmp = parent;				\
				parent = elm;				\
				elm = tmp;				\
			}						\
			RBE_SET_BLACKRED(parent, gparent, field);	\
			RBE_ROTATE_LEFT(head, gparent, tmp, field);	\
		}							\
	}								\
	RBE_COLOR(head->rbh_root, field) = RB_BLACK;			\
}									\
									\
attr void								\
name##_RBE_REMOVE_COLOR(struct name *head, struct type *parent, struct type *elm) \
{									\
	struct type *tmp;						\
	while ((elm == NULL || RBE_COLOR(elm, field) == RB_BLACK) &&	\
	    elm != RB_ROOT(head)) {					\
		if (RBE_LEFT(parent, field) == elm) {			\
			tmp = RBE_RIGHT(parent, field);			\
			if (RBE_COLOR(tmp, field) == RB_RED) {		\
				RBE_SET_BLACKRED(tmp, parent, field);	\
				RBE_ROTATE_LEFT(head, parent, tmp, field);\
				tmp = RBE_RIGHT(parent, field);		\
			}						\
			if ((RBE_LEFT(tmp, field) == NULL ||		\
			    RBE_COLOR(RBE_LEFT(tmp, field), field) == RB_BLACK) &&\
			    (RBE_RIGHT(tmp, field) == NULL ||		\
			    RBE_COLOR(RBE_RIGHT(tmp, field), field) == RB_BLACK)) {\
				RBE_COLOR(tmp, field) = RB_RED;	\
				elm = parent;				\
				parent = RBE_PARENT(elm, field);	\
			} else {					\
				if (RBE_RIGHT(tmp, field) == NULL ||	\
				    RBE_COLOR(RBE_RIGHT(tmp, field), field) == RB_BLACK) {\
					struct type *oleft;		\
					if ((oleft = RBE_LEFT(tmp, field)))\
						RBE_COLOR(oleft, field) = RB_BLACK;\
					RBE_COLOR(tmp, field) = RB_RED;\
					RBE_ROTATE_RIGHT(head, tmp, oleft, field);\
					tmp = RBE_RIGHT(parent, field);	\
				}					\
				RBE_COLOR(tmp, field) = RBE_COLOR(parent, field);\
				RBE_COLOR(parent, field) = RB_BLACK;	\
				if (RBE_RIGHT(tmp, field))		\
					RBE_COLOR(RBE_RIGHT(tmp, field), field) = RB_BLACK;\
				RBE_ROTATE_LEFT(head, parent, tmp, field);\
				elm = RB_ROOT(head);			\
				break;					\
			}						\
		} else {						\
			tmp = RBE_LEFT(parent, field);			\
			if (RBE_COLOR(tmp, field) == RB_RED) {		\
				RBE_SET_BLACKRED(tmp, parent, field);	\
				RBE_ROTATE_RIGHT(head, parent, tmp, field);\
				tmp = RBE_LEFT(parent, field);		\
			}						\
			if ((RBE_LEFT(tmp, field) == NULL ||		\
			    RBE_COLOR(RBE_LEFT(tmp, field), field) == RB_BLACK) &&\
			    (RBE_RIGHT(tmp, field) == NULL ||		\
			    RBE_COLOR(RBE_RIGHT(tmp, field), field) == RB_BLACK)) {\
				RBE_COLOR(tmp, field) = RB_RED;	\
				elm = parent;				\
				parent = RBE_PARENT(elm, field);	\
			} else {					\
				if (RBE_LEFT(tmp, field) == NULL ||	\
				    RBE_COLOR(RBE_LEFT(tmp, field), field) == RB_BLACK) {\
					struct type *oright;		\
					if ((oright = RBE_RIGHT(tmp, field)))\
						RBE_COLOR(oright, field) = RB_BLACK;\
					RBE_COLOR(tmp, field) = RB_RED;	\
					RBE_ROTATE_LEFT(head, tmp, oright, field);\
					tmp = RBE_LEFT(parent, field);	\
				}					\
				RBE_COLOR(tmp, field) = RBE_COLOR(parent, field);\
				RBE_COLOR(parent, field) = RB_BLACK;	\
				if (RBE_LEFT(tmp, field))		\
					RBE_COLOR(RBE_LEFT(tmp, field), field) = RB_BLACK;\
				RBE_ROTATE_RIGHT(head, parent, tmp, field);\
				elm = RB_ROOT(head);			\
				break;					\
			}						\
		}							\
	}								\
	if (elm)							\
		RBE_COLOR(elm, field) = RB_BLACK;			\
}									\
									\
attr struct type *							\
name##_RBE_REMOVE(struct name *head, struct type *elm)			\
{									\
	struct type *child, *parent, *old = elm;			\
	int color;							\
	if (RBE_LEFT(elm, field) == NULL)				\
		child = RBE_RIGHT(elm, field);				\
	else if (RBE_RIGHT(elm, field) == NULL)				\
		child = RBE_LEFT(elm, field);				\
	else {								\
		struct type *left;					\
		elm = RBE_RIGHT(elm, field);				\
		while ((left = RBE_LEFT(elm, field)))			\
			elm = left;					\
		child = RBE_RIGHT(elm, field);				\
		parent = RBE_PARENT(elm, field);			\
		color = RBE_COLOR(elm, field);				\
		if (child)						\
			RBE_PARENT(child, field) = parent;		\
		if (parent) {						\
			if (RBE_LEFT(parent, field) == elm)		\
				RBE_LEFT(parent, field) = child;	\
			else						\
				RBE_RIGHT(parent, field) = child;	\
			RBE_AUGMENT(parent);				\
		} else							\
			RB_ROOT(head) = child;				\
		if (RBE_PARENT(elm, field) == old)			\
			parent = elm;					\
		(elm)->field = (old)->field;				\
		if (RBE_PARENT(old, field)) {				\
			if (RBE_LEFT(RBE_PARENT(old, field), field) == old)\
				RBE_LEFT(RBE_PARENT(old, field), field) = elm;\
			else						\
				RBE_RIGHT(RBE_PARENT(old, field), field) = elm;\
			RBE_AUGMENT(RBE_PARENT(old, field));		\
		} else							\
			RB_ROOT(head) = elm;				\
		RBE_PARENT(RBE_LEFT(old, field), field) = elm;		\
		if (RBE_RIGHT(old, field))				\
			RBE_PARENT(RBE_RIGHT(old, field), field) = elm;	\
		if (parent) {						\
			left = parent;					\
			do {						\
				RBE_AUGMENT(left);			\
			} while ((left = RBE_PARENT(left, field)));	\
		}							\
		goto color;						\
	}								\
	parent = RBE_PARENT(elm, field);				\
	color = RBE_COLOR(elm, field);					\
	if (child)							\
		RBE_PARENT(child, field) = parent;			\
	if (parent) {							\
		if (RBE_LEFT(parent, field) == elm)			\
			RBE_LEFT(parent, field) = child;		\
		else							\
			RBE_RIGHT(parent, field) = child;		\
		RBE_AUGMENT(parent);					\
	} else								\
		RB_ROOT(head) = child;					\
color:									\
	if (color == RB_BLACK)						\
		name##_RBE_REMOVE_COLOR(head, parent, child);		\
	return (old);							\
}									\
									\
/* Inserts a node into the RB tree */					\
attr struct type *							\
name##_RBE_INSERT(struct name *head, struct type *elm)			\
{									\
	struct type *tmp;						\
	struct type *parent = NULL;					\
	int comp = 0;							\
	tmp = RB_ROOT(head);						\
	while (tmp) {							\
		parent = tmp;						\
		comp = (cmp)(elm, parent);				\
		if (comp < 0)						\
			tmp = RBE_LEFT(tmp, field);			\
		else if (comp > 0)					\
			tmp = RBE_RIGHT(tmp, field);			\
		else							\
			return (tmp);					\
	}								\
	RBE_SET(elm, parent, field);					\
	if (parent != NULL) {						\
		if (comp < 0)						\
			RBE_LEFT(parent, field) = elm;			\
		else							\
			RBE_RIGHT(parent, field) = elm;			\
		RBE_AUGMENT(parent);					\
	} else								\
		RB_ROOT(head) = elm;					\
	name##_RBE_INSERT_COLOR(head, elm);				\
	return (NULL);							\
}									\
									\
/* Finds the node with the same key as elm */				\
attr struct type *							\
name##_RBE_FIND(struct name *head, struct type *elm)			\
{									\
	struct type *tmp = RB_ROOT(head);				\
	int comp;							\
	while (tmp) {							\
		comp = cmp(elm, tmp);					\
		if (comp < 0)						\
			tmp = RBE_LEFT(tmp, field);			\
		else if (comp > 0)					\
			tmp = RBE_RIGHT(tmp, field);			\
		else							\
			return (tmp);					\
	}								\
	return (NULL);							\
}									\
									\
/* Finds the first node greater than or equal to the search key */	\
attr struct type *							\
name##_RBE_NFIND(struct name *head, struct type *elm)			\
{									\
	struct type *tmp = RB_ROOT(head);				\
	struct type *res = NULL;					\
	int comp;							\
	while (tmp) {							\
		comp = cmp(elm, tmp);					\
		if (comp < 0) {						\
			res = tmp;					\
			tmp = RBE_LEFT(tmp, field);			\
		}							\
		else if (comp > 0)					\
			tmp = RBE_RIGHT(tmp, field);			\
		else							\
			return (tmp);					\
	}								\
	return (res);							\
}									\
									\
/* ARGSUSED */								\
attr struct type *							\
name##_RBE_NEXT(struct type *elm)					\
{									\
	if (RBE_RIGHT(elm, field)) {					\
		elm = RBE_RIGHT(elm, field);				\
		while (RBE_LEFT(elm, field))				\
			elm = RBE_LEFT(elm, field);			\
	} else {							\
		if (RBE_PARENT(elm, field) &&				\
		    (elm == RBE_LEFT(RBE_PARENT(elm, field), field)))	\
			elm = RBE_PARENT(elm, field);			\
		else {							\
			while (RBE_PARENT(elm, field) &&		\
			    (elm == RBE_RIGHT(RBE_PARENT(elm, field), field)))\
				elm = RBE_PARENT(elm, field);		\
			elm = RBE_PARENT(elm, field);			\
		}							\
	}								\
	return (elm);							\
}									\
									\
/* ARGSUSED */								\
attr struct type *							\
name##_RBE_PREV(struct type *elm)					\
{									\
	if (RBE_LEFT(elm, field)) {					\
		elm = RBE_LEFT(elm, field);				\
		while (RBE_RIGHT(elm, field))				\
			elm = RBE_RIGHT(elm, field);			\
	} else {							\
		if (RBE_PARENT(elm, field) &&				\
		    (elm == RBE_RIGHT(RBE_PARENT(elm, field), field)))	\
			elm = RBE_PARENT(elm, field);			\
		else {							\
			while (RBE_PARENT(elm, field) &&		\
			    (elm == RBE_LEFT(RBE_PARENT(elm, field), field)))\
				elm = RBE_PARENT(elm, field);		\
			elm = RBE_PARENT(elm, field);			\
		}							\
	}								\
	return (elm);							\
}									\
									\
attr struct type *							\
name##_RBE_MINMAX(struct name *head, int val)				\
{									\
	struct type *tmp = RB_ROOT(head);				\
	struct type *parent = NULL;					\
	while (tmp) {							\
		parent = tmp;						\
		if (val < 0)						\
			tmp = RBE_LEFT(tmp, field);			\
		else							\
			tmp = RBE_RIGHT(tmp, field);			\
	}								\
	return (parent);						\
}

#define RBE_NEGINF	-1
#define RBE_INF	1

#define RBE_INSERT(name, x, y)	name##_RBE_INSERT(x, y)
#define RBE_REMOVE(name, x, y)	name##_RBE_REMOVE(x, y)
#define RBE_FIND(name, x, y)	name##_RBE_FIND(x, y)
#define RBE_NFIND(name, x, y)	name##_RBE_NFIND(x, y)
#define RBE_NEXT(name, x, y)	name##_RBE_NEXT(y)
#define RBE_PREV(name, x, y)	name##_RBE_PREV(y)
#define RBE_MIN(name, x)		name##_RBE_MINMAX(x, RBE_NEGINF)
#define RBE_MAX(name, x)		name##_RBE_MINMAX(x, RBE_INF)

#define RBE_FOREACH(x, name, head)					\
	for ((x) = RBE_MIN(name, head);					\
	     (x) != NULL;						\
	     (x) = name##_RBE_NEXT(x))

#define RBE_FOREACH_SAFE(x, name, head, y)				\
	for ((x) = RBE_MIN(name, head);					\
	    ((x) != NULL) && ((y) = name##_RBE_NEXT(x), 1);		\
	     (x) = (y))

#define RBE_FOREACH_REVERSE(x, name, head)				\
	for ((x) = RBE_MAX(name, head);					\
	     (x) != NULL;						\
	     (x) = name##_RBE_PREV(x))

#define RBE_FOREACH_REVERSE_SAFE(x, name, head, y)			\
	for ((x) = RBE_MAX(name, head);					\
	    ((x) != NULL) && ((y) = name##_RBE_PREV(x), 1);		\
	     (x) = (y))

#endif /* _RB_WORKAROUND_H_ */
