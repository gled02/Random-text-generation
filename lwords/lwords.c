//  Partie implantation du module lwords

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lwords.h"

#define FUN_SUCCESS 0
#define FUN_FAILURE 1

typedef struct clwords clwords;

struct clwords {
  const void *value;
  clwords *next;
};

struct lwords {
  clwords *head;
  clwords *tail;
  size_t length;
};

lwords *lwords_empty(void) {
  lwords *s = malloc(sizeof *s);
  if (s == NULL) {
    return NULL;
  }
  s->head = NULL;
  s->tail = NULL;
  s->length = 0;
  return s;
}

bool lwords_is_empty(const lwords *s) {
  return s->length == 0;
}

size_t lwords_length(const lwords *s) {
  return s->length;
}

const void *lwords_add_tail(lwords *s, const void *xptr) {
  if (xptr == NULL) {
    return NULL;
  }
  clwords *p = malloc(sizeof *p);
  if (p == NULL) {
    return NULL;
  }
  p->value = xptr;
  p->next = NULL;
  if (s->head == NULL) {
    s->head = p;
  } else {
    s->tail->next = p;
  }
  s->tail = p;
  s->length += 1;
  return xptr;
}

const void *lwords_add_head(lwords *s, const void *xptr) {
  if (xptr == NULL) {
    return NULL;
  }
  clwords *p = malloc(sizeof *p);
  if (p == NULL) {
    return NULL;
  }
  p->value = xptr;
  p->next = s->head;
  s->head = p;
  if (s->tail == NULL) {
    s->tail = p;
  }
  s->length += 1;
  return xptr;
}

lwords *lwords_reverse(lwords *src) {
  lwords *dest = lwords_empty();
  if (dest == NULL) {
    return NULL;
  }
  clwords *p = src->head;
  while (p != NULL) {
    if (lwords_add_head(dest, p->value) == NULL) {
      lwords_dispose(&dest);
      return NULL;
    }
    p = p->next;
  }
  return dest;
}

lwords *lwords_key(const void *xptr, size_t order) {
  if (xptr == NULL) {
    return NULL;
  }
  lwords *s = lwords_empty();
  if (s == NULL) {
    return NULL;
  }
  while (order != 0) {
    if (lwords_add_tail(s, xptr) == NULL) {
      lwords_dispose(&s);
      return NULL;
    }
    order -= 1;
  }
  return s;
}

lwords *lwords_replace(const lwords *s, const void *xptr) {
  if (xptr == NULL) {
    return NULL;
  }
  if (lwords_is_empty(s)) {
    return NULL;
  }
  lwords *p = lwords_empty();
  if (p == NULL) {
    return NULL;
  }
  clwords **pp = &(p->head);
  clwords *q = s->head->next;
  while (q != NULL) {
    *pp = malloc(sizeof **pp);
    if (*pp == NULL) {
      lwords_dispose(&p);
      return NULL;
    }
    (*pp)->value = q->value;
    p->tail = *pp;
    (*pp)->next = NULL;
    p->length += 1;
    if (p->length == 1) {
      p->head = *pp;
    }
    pp = (&(*pp)->next);
    q = q->next;
  }
  if (lwords_add_tail(p, xptr) == NULL) {
    lwords_dispose(&p);
    return NULL;
  }
  return p;
}

const void *lwords_index_value(const lwords *s, size_t k) {
  if (k > lwords_length(s)) {
    return NULL;
  }
  clwords *p = s->head;
  size_t j = 0;
  while (p != NULL && j < k) {
    p = p->next;
    ++j;
  }
  return p->value;
}

int lwords_comparison(lwords *s1, lwords *s2) {
  clwords *p1 = s1->head;
  clwords *p2 = s2->head;
  while (p1 != NULL && p2 != NULL && strcmp(p1->value, p2->value) == 0) {
    p1 = p1->next;
    p2 = p2->next;
  }
  if (p1 != NULL && p2 != NULL) {
    return strcmp(p1->value, p2->value) > 0 ? 1 : -1;
  }
  if (p1 != NULL && p2 == NULL) {
    return 1;
  }
  if (p1 == NULL && p2 != NULL) {
    return -1;
  }
  return 0;
}

size_t lwords_hashfun(const lwords *s) {
  clwords *p = s->head;
  size_t h = 0;
  while (p != NULL) {
    const char *c = (char *) p->value;
    for (const unsigned char *t = (const unsigned char *) c; *t != '\0'; ++t) {
      h = 37 * h + *t;
    }
    p = p->next;
  }
  return h;
}

int lwords_fput(const lwords *s, FILE *stream) {
  clwords *p = s->head;
  while (p != NULL) {
    if (fprintf(stream, "%s\t", (char *) p->value) < 0) {
      return FUN_FAILURE;
    }
    p = p->next;
  }
  return FUN_SUCCESS;
}

void lwords_dispose(lwords **sptr) {
  if (*sptr == NULL) {
    return;
  }
  clwords *p = (*sptr)->head;
  while (p != NULL) {
    clwords *t = p;
    p = p->next;
    free(t);
  }
  free(*sptr);
  *sptr = NULL;
}
