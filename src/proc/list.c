#include "list.h"

struct coorList* listInit()
{
    struct coorList *l = malloc(sizeof(struct coorList));
    l->next = NULL;
    l->data = NULL;
    return l;
}

void listAdd(struct coorList *l, uchar *data, size_t X, size_t Y)
{
    struct coorList *aux = malloc(sizeof(struct coorList));
    aux->next = l->next;
    l->next = aux;
    aux->data = data;
    aux->X = X;
    aux->Y = Y;
}

void listReverse(struct coorList *l)
{
    struct coorList *new = malloc(sizeof(struct coorList));
    new->next = NULL;
    struct coorList *ptr = l;

    while (l->next != NULL) {
        l = l->next;

        if (l->X < 3 || l->Y < 3) {
            struct coorList *tmp = l;
            l = l->next;
            free(tmp->data);
            free(tmp);
        } else
            listAdd(new, l->data, l->X, l->Y);
    }

    free(ptr);
}

int listLen(struct coorList *l)
{
    if (l->next)
        return 1 + listLen(l->next);
    else
        return 0;
}

void listFree(struct coorList *l)
{
    if (l->next)
        listFree(l->next);

    free(l->data);
    free(l);
}
