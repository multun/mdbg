#include "mlist.h"


void mlist_init(s_mlist *list)
{
    list->size = 0;
    list->head = NULL;
}


void mlist_insert(s_mlist *list, s_mlist_e *elem, s_mlist_e **ip)
{
    list->size++;

    if (*ip)
        (*ip)->prev = &elem->next;
    elem->next = (*ip);
    elem->prev = ip;
    *ip = elem;
}


void mlist_add(s_mlist *list, s_mlist_e *elem)
{
    mlist_insert(list, elem, &list->head);
}


void mlist_remove(s_mlist *list, s_mlist_e *elem)
{
    list->size--;

    if (elem->next)
        elem->next->prev = elem->prev;
    *elem->prev = elem->next;
}


void *mlist_find(s_mlist *list, f_mlist_pred pred, size_t off, void *data)
{
    for (s_mlist_e *cur = list->head; cur; cur = cur->next)
    {
        void *cont = ((char*)cur) - off;
        if (pred(cont, data))
            return cont;
    }

    return NULL;
}
