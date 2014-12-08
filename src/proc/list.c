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
