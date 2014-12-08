#include "list.h"

void listAdd(struct coorList *l, uchar *data, size_t X, size_t Y)
{
    printf("[INFO] listAdd()\n");
    struct coorList *aux = malloc(sizeof(struct coorList));
    aux->next = l->next;
    l->next = aux;
    l = l->next;
    l->data = data;
    l->X = X;
    l->Y = Y;
}

void reverse(struct coorList *l)
{
    struct coorList *aux = malloc(sizeof(struct coorList));
    struct coorList *tmp = aux;
    struct coorList *h = l;
    while (h->next != NULL) {
        while (h->next != NULL) {
            h = h->next;
        }
        if (h->X != 0 && h->Y != 0) {
            aux->next = h;
            aux = aux->next;
        }
        free(h);
        h = l;
    }
    l->next = tmp;
}
